#ifndef STACK_H
#define STACK_H

#include "../list/list.h"

typedef struct stack
{
    // list* values;
    int* values;
    int cur;

} stack;

stack* new_stack();
void stack_free(stack* s);
void stack_push(stack* s, local_type value);
local_type stack_pop(stack* s);
local_type stack_peek(stack* s);

#endif