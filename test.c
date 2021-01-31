

/*
  Created by figurantpp on 25/01/2021.
*/


#include <stdlib.h>


#include "restack/restack.h"


#include <munit/munit.h>

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

MunitResult test_stack(const MunitParameter parameters[], void *data)
{
    struct TestObject *objects[TEST_SIZE];

    RestackResource resources[TEST_SIZE];
    ResourceStack stack[1];
    size_t i;

    restack_init_macro(stack, resources);

    (void)parameters;
    (void)data;


    test_object_value = 0;
    test_object_delete_count = 0;

    for (i = 0; i < TEST_SIZE; i++)
    {
        objects[i] = test_object_init();
        restack_push(stack, objects[i], test_object_delete);
    }

    restack_delete(stack);

    munit_assert_long(test_object_value, == , 0);
	munit_assert_long(test_object_delete_count, == , TEST_SIZE);

    return MUNIT_OK;
}


MunitResult test_isolated_push(const MunitParameter parameters[], void *data)
{
    ResourceStack stack[1];

    RestackResource resources[3];


    restack_init(stack, resources, 3);

    (void)parameters;
    (void)data;


    test_object_value = 0;

    increase_test_object_value();
    increase_test_object_value();
    increase_test_object_value();

    restack_push_isolated(stack, decrease_test_object_value);
    restack_push_isolated(stack, decrease_test_object_value);
    restack_push_isolated(stack, decrease_test_object_value);

    restack_delete(stack);

    munit_assert_long(test_object_value, == , 0);

    return MUNIT_OK;
}

MunitResult test_null_push(const MunitParameter parameters[], void *data)
{
    RestackResource resources[3];
    ResourceStack stack[1];
    restack_init(stack, resources, 3);

    (void)parameters;
    (void)data;

    test_object_value = 0;

    restack_push(stack, NULL, test_object_delete);
    restack_push(stack, NULL, test_object_delete);
    restack_push(stack, NULL, test_object_delete);

    restack_delete(stack);

    munit_assert_long(test_object_value, ==, 0);

    return MUNIT_OK;
}

int main(int argc, char * const * argv)
{
    MunitTest tests[] = {
        { "Default Stack Test", test_stack, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
        { "Isolated Push Test", test_isolated_push, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
        { "Null Push Test", test_null_push, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
        { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
    };

    MunitSuite suite = {
        "restack tests",
        NULL,
        NULL,
        1,
        MUNIT_SUITE_OPTION_NONE
    };

    suite.tests = tests;


    return munit_suite_main(&suite, NULL, argc, argv);

}
