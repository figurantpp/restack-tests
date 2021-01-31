/*
// Created by figurantpp on 31/01/2021.
*/

#include <cutest/CuTest.h>

#include "restack/restack.h"
#include "side_effect.h"

#define TEST_SIZE 10

void test_stack(CuTest *test)
{
    struct SideEffectObject *objects[TEST_SIZE];

    RestackResource resources[TEST_SIZE];
    Restack stack[1];
    size_t i;

    restack_init_macro(stack, resources);

    side_effect_value = SIDE_EFFECT_INITIAL_VALUE;
    side_effect_delete_count = 0;

    for (i = 0; i < TEST_SIZE; i++)
    {
        objects[i] = side_effect_object_init();
        restack_push(stack, objects[i], side_effect_object_delete);
    }

    restack_delete(stack);

    CuAssertIntEquals(test, side_effect_value, SIDE_EFFECT_INITIAL_VALUE);
    CuAssertIntEquals(test, side_effect_delete_count, TEST_SIZE);
}


void test_isolated_push(CuTest *test)
{
    Restack stack[1];

    RestackResource resources[3];


    restack_init(stack, resources, 3);


    side_effect_value = SIDE_EFFECT_INITIAL_VALUE;

    increase_side_effect_value();
    increase_side_effect_value();
    increase_side_effect_value();

    restack_push_call(stack, decrease_side_effect_value);
    restack_push_call(stack, decrease_side_effect_value);
    restack_push_call(stack, decrease_side_effect_value);

    restack_delete(stack);

    CuAssertIntEquals(test, side_effect_value, SIDE_EFFECT_INITIAL_VALUE);
}

void test_null_push(CuTest *test)
{
    RestackResource resources[3];
    Restack stack[1];
    restack_init(stack, resources, 3);

    side_effect_value = SIDE_EFFECT_INITIAL_VALUE;

    restack_push(stack, NULL, side_effect_object_delete);
    restack_push(stack, NULL, side_effect_object_delete);
    restack_push(stack, NULL, side_effect_object_delete);

    restack_delete(stack);

    CuAssertIntEquals(test, side_effect_value, SIDE_EFFECT_INITIAL_VALUE);
}


void test_overflow_handler(void)
{
    side_effect_value = 0xfeedbeef;
}

void test_overflow(CuTest *test)
{
    struct SideEffectObject *objects[2];

    RestackResource resources[1];
    Restack stack;

    restack_init_macro(&stack, resources);

    side_effect_value = SIDE_EFFECT_INITIAL_VALUE;

    restack_set_overflow_handler(test_overflow_handler);

    restack_emit_errors(0);

    objects[0] = side_effect_object_init();
    objects[1] = side_effect_object_init();

    restack_push(&stack, objects[0], side_effect_object_delete);

    /*
     * On overflow, the overflow object should not be deleted, 
     * no buffer overflow shall occur.
     * An optional error message may be emitted.
     */
    restack_push(&stack, objects[1], side_effect_object_delete);


    restack_delete(&stack);

    CuAssert(test, "Test value shall be unmatched",
            side_effect_value != SIDE_EFFECT_INITIAL_VALUE);

    /* Restack shouldn't have the last object so we have to do it ourselves */

    side_effect_object_delete(objects[1]);
}

CuSuite *get_restack_suite()
{
    CuSuite *spec = CuSuiteNew();

    SUITE_ADD_TEST(spec, test_stack);
    SUITE_ADD_TEST(spec, test_isolated_push);
    SUITE_ADD_TEST(spec, test_null_push);
    SUITE_ADD_TEST(spec, test_overflow);

    return spec;
}

