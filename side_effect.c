/*
// Created by figurantpp on 31/01/2021.
*/

#include "side_effect.h"

unsigned long side_effect_value = SIDE_EFFECT_INITIAL_VALUE;

void increase_side_effect_by(int count)
{
    side_effect_value *= 2;
    side_effect_value += count;
}

void increase_side_effect_value(void)
{
    increase_side_effect_by(5);
}

void decrease_side_effect_value_by(int count)
{
    side_effect_value -= count;
    side_effect_value /= 2;
}

void decrease_side_effect_value(void)
{
    decrease_side_effect_value_by(5);
}

void *side_effect_object_init(void)
{
    static int current_id = 0;

    struct SideEffectObject *object = malloc(sizeof(struct SideEffectObject));

    if (object)
    {
        current_id++;

        object->id = current_id;

        increase_side_effect_by(object->id);
    }

    return object;
}

int side_effect_delete_count;


void side_effect_object_delete(struct SideEffectObject *object)
{
    decrease_side_effect_value_by(object->id);

    free(object);

    side_effect_delete_count++;
}

