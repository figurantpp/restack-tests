

/*
  Created by figurantpp on 25/01/2021.
*/


#include <stdlib.h>


#include "restack/restack.h"

#include <cutest/CuTest.h>
#include <stdio.h>

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

void test_stack(CuTest *test)
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

    CuAssertIntEquals(test, test_object_value, 0);
    CuAssertIntEquals(test, test_object_delete_count, TEST_SIZE);
}


void test_isolated_push(CuTest *test)
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

    CuAssertIntEquals(test, test_object_value, 0);
}

void test_null_push(CuTest *test)
{
    RestackResource resources[3];
    ResourceStack stack[1];
    restack_init(stack, resources, 3);

    test_object_value = 0;

    restack_push(stack, NULL, test_object_delete);
    restack_push(stack, NULL, test_object_delete);
    restack_push(stack, NULL, test_object_delete);

    restack_delete(stack);

    CuAssertIntEquals(test, test_object_value, 0);
}

int main(void)
{
    CuSuite *spec = CuSuiteNew();
    CuString *output;
    CuSuite *suite;

    SUITE_ADD_TEST(spec, test_stack);
    SUITE_ADD_TEST(spec, test_isolated_push);
    SUITE_ADD_TEST(spec, test_null_push);

    output = CuStringNew();
    suite = CuSuiteNew();

    CuSuiteAddSuite(suite, spec);

    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);

    puts(output->buffer);

    return 0;
}
