#include "stack.h"

#include <stdlib.h>

#define MAX_SIZE 1024

stack* new_stack() {
    stack* result = malloc(sizeof(stack));

    result->cur = 0;
    result->values = malloc(sizeof(local_type) * MAX_SIZE);

    return result;
}

void stack_free(stack* s) {
    free(s->values);
    free(s);
}

void stack_push(stack* s, local_type value) {
    s->values[++s->cur] = value;
}

local_type stack_pop(stack* s) {
    local_type result = s->values[s->cur];
    --s->cur;
    return result;
}

local_type stack_peek(stack* s) {
    local_type result = s->values[s->cur];
    return result;
}


// * linked-list based implementation, unfinished
// #include "stack.h"

// #include <stdlib.h>

// stack* new_stack() {
//     stack* result = malloc(sizeof(stack));

//     result->cur = 0;
//     result->values = new_list();

//     return result;
// }

// void stack_free(stack* s) {
//     list_free(s->values);
//     free(s);
// }

// void stack_push(stack* s, local_type value) {
//     ++s->cur;
//     list_add(s->values, value);
// }

// local_type stack_pop(stack* s) {
//     list_node* result_node = s->values->root->prev;
//     local_type result = result_node->value;
    
//     s->values->root->prev->prev = 0;

//     return result;
// }