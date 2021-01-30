

/*
  Created by figurantpp on 25/01/2021.
*/


#include <malloc.h>

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "restack/restack.h"

struct TestObject
{
    int id;
};


static long test_object_value = 0;

void increase_test_object_value_by(int count)
{
    test_object_value *= 2;
    test_object_value += count;
}

void increase_test_object_value(void)
{
    increase_test_object_value_by(5);
}

void decrease_test_object_value_by(int count)
{
    test_object_value -= count;
    test_object_value /= 2;
}

void decrease_test_object_value(void)
{
    decrease_test_object_value_by(5);
}


void *test_object_init(void)
{
    static int current_id = 0;

    struct TestObject *object = malloc(sizeof(struct TestObject));

    if (object)
    {
        current_id++;

        object->id = current_id;

        increase_test_object_value_by(object->id);
    }

    return object;
}

int test_object_delete_count;


void test_object_delete(struct TestObject *object)
{
    decrease_test_object_value_by(object->id);

    free(object);

    test_object_delete_count++;
}

#define TEST_SIZE 10

void test_stack(void)
{
    struct TestObject *objects[TEST_SIZE];

    RestackResource resources[TEST_SIZE];
    ResourceStack stack[1];
    size_t i;

    restack_init_macro(stack, resources);

    test_object_value = 0;
    test_object_delete_count = 0;

    for (i = 0; i < TEST_SIZE; i++)
    {
        objects[i] = test_object_init();
        restack_push(stack, objects[i], test_object_delete);
    }

    restack_delete(stack);

    CU_ASSERT_EQUAL(test_object_value, 0)
    CU_ASSERT_EQUAL(test_object_delete_count, TEST_SIZE)
}


void test_isolated_push(void)
{
    ResourceStack stack[1];

    RestackResource resources[3];


    restack_init(stack, resources, 3);

    test_object_value = 0;

    increase_test_object_value();
    increase_test_object_value();
    increase_test_object_value();

    restack_push_isolated(stack, decrease_test_object_value);
    restack_push_isolated(stack, decrease_test_object_value);
    restack_push_isolated(stack, decrease_test_object_value);

    restack_delete(stack);

    CU_ASSERT_EQUAL(test_object_value, 0)
}

void test_null_push(void)
{
    RestackResource resources[3];
    ResourceStack stack[1];
    restack_init(stack, resources, 3);

    test_object_value = 0;

    restack_push(stack, NULL, test_object_delete);
    restack_push(stack, NULL, test_object_delete);
    restack_push(stack, NULL, test_object_delete);

    restack_delete(stack);

    CU_ASSERT_EQUAL(test_object_value, 0)
}

int main(void)
{
    int exit = 0;
    CU_Suite *suite;

    if (CU_initialize_registry() != CUE_SUCCESS)
    {
        fprintf(stderr, "Failed to initialize CUnit\n");
        exit = 1;
    }

    if (exit == 0)
    {
        suite = CU_add_suite("restack_main_tests", NULL, NULL);

        if (suite == NULL)
        {
            fprintf(stderr, "Failed to start suite\n");
            exit = 2;
        }
    }

    if (exit == 0)
    {
        if (!CU_add_test(suite, "ResourceStack default operations test", test_stack)
            ||
            !CU_add_test(suite, "ResourceStack isolated push test", test_isolated_push)
            ||
            !CU_add_test(suite, "ResourceStack push with null test", test_null_push) )
        {
            fprintf(stderr, "Failed to start tests\n");
            exit = 3;
        }
    }

    if (exit == 0)
    {
        CU_basic_run_tests();
    }

    CU_cleanup_registry();

    return exit;
}
