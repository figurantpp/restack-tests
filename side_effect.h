/*
// Created by figurantpp on 31/01/2021.
*/

#ifndef RESTACK_TESTS_SIDE_EFFECT_H
#define RESTACK_TESTS_SIDE_EFFECT_H

#define SIDE_EFFECT_INITIAL_VALUE 0xbeef

struct SideEffectObject
{
    int id;
};


extern unsigned long side_effect_value;
extern int side_effect_delete_count;

void increase_side_effect_by(int count);

void increase_side_effect_value(void);

void decrease_side_effect_value_by(int count);

void decrease_side_effect_value(void);

void *side_effect_object_init(void);

void side_effect_object_delete(struct SideEffectObject *object);

#endif /* RESTACK_TESTS_SIDE_EFFECT_H */

