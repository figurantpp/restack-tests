

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


static unsigned long test_object_value = 0;

#define TEST_INITIAL_VALUE 0xbeef

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
    Restack stack[1];
    size_t i;

    restack_init_macro(stack, resources);

    test_object_value = TEST_INITIAL_VALUE;
    test_object_delete_count = 0;

    for (i = 0; i < TEST_SIZE; i++)
    {
        objects[i] = test_object_init();
        restack_push(stack, objects[i], test_object_delete);
    }

    restack_delete(stack);

    CuAssertIntEquals(test, test_object_value, TEST_INITIAL_VALUE);
    CuAssertIntEquals(test, test_object_delete_count, TEST_SIZE);
}


void test_isolated_push(CuTest *test)
{
    Restack stack[1];

    RestackResource resources[3];


    restack_init(stack, resources, 3);


    test_object_value = TEST_INITIAL_VALUE;

    increase_test_object_value();
    increase_test_object_value();
    increase_test_object_value();

    restack_push_call(stack, decrease_test_object_value);
    restack_push_call(stack, decrease_test_object_value);
    restack_push_call(stack, decrease_test_object_value);

    restack_delete(stack);

    CuAssertIntEquals(test, test_object_value, TEST_INITIAL_VALUE);
}

void test_null_push(CuTest *test)
{
    RestackResource resources[3];
    Restack stack[1];
    restack_init(stack, resources, 3);

    test_object_value = TEST_INITIAL_VALUE;

    restack_push(stack, NULL, test_object_delete);
    restack_push(stack, NULL, test_object_delete);
    restack_push(stack, NULL, test_object_delete);

    restack_delete(stack);

    CuAssertIntEquals(test, test_object_value, TEST_INITIAL_VALUE);
}


void test_overflow_handler(void)
{
    test_object_value = 0xfeedbeef;
}

void test_overflow(CuTest *test)
{
    struct TestObject *objects[2];

    RestackResource resources[1];
    Restack stack;

    restack_init_macro(&stack, resources);

    test_object_value = TEST_INITIAL_VALUE;

    restack_set_overflow_handler(test_overflow_handler);

    objects[0] = test_object_init();
    objects[1] = test_object_init();

    restack_push(&stack, objects[0], test_object_delete);

    /*
     * On overflow, the overflow object should not be deleted, no buffer overflow shall occur.
     * An optional error message may be emitted.
     */
    restack_push(&stack, objects[1], test_object_delete);

    restack_delete(&stack);

    CuAssert(test, "Test value shall be unmatched", test_object_value != TEST_INITIAL_VALUE);

    /* Restack shouldn't have the last object so we have to do it ourselves */

    test_object_delete(objects[1]);
}

int main(void)
{
    CuSuite *spec = CuSuiteNew();
    CuString *output;
    CuSuite *suite;

    SUITE_ADD_TEST(spec, test_stack);
    SUITE_ADD_TEST(spec, test_isolated_push);
    SUITE_ADD_TEST(spec, test_null_push);
    SUITE_ADD_TEST(spec, test_overflow);

    output = CuStringNew();
    suite = CuSuiteNew();

    CuSuiteAddSuite(suite, spec);

    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);

    puts(output->buffer);

    return 0;
}
