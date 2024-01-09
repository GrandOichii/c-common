#include "json.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../stack/stack.h"

#define MAX_STRING_SIZE 1024

#define CTX_READING_VALUE 0
#define CTX_READING_STRING 1

int parse(const char* string, json_node* result) {
    // TODO add escape character support
    json_node* r = malloc(sizeof(r));

    int ctx = CTX_READING_VALUE;

    int pos = 0;
    stack* braces_stack = new_stack();
    int string_start;
    char* string_buf = malloc(sizeof(char) * MAX_STRING_SIZE);
    while (string[pos] != '\0') {
        char cur = string[pos];
        // printf("%c\n", cur);

        pos++;

        char last;

        // switch (cur) {
        // case '{':
        //     stack_push(braces_stack, cur);
        //     break;
        // case '[':
        //     stack_push(braces_stack, cur);
        //     break;
        // case '}':
        //     last = stack_pop(braces_stack);
        //     if (last != '{') {
        //         printf("Error: brace %c not closed (cur: %d)\n", last, pos);
        //         return 1;
        //     }
        //     break;
        // case ']':
        //     last = stack_pop(braces_stack);
        //     if (last != '[') {
        //         printf("Error: brace %c not closed (cur: %d)\n", last, pos);
        //         return 1;
        //     }
        //     break;
        // case '\"':
        //     last = stack_peek(braces_stack);
        //     if (last == '\"') {
        //         stack_pop(braces_stack);
        //         break;
        //     }
        //     stack_push(braces_stack, cur);
        //     break;
        // default:

            

        // }

        switch(ctx) {
            case CTX_READING_VALUE:
                switch (cur) {
                case '\"':
                    stack_push(braces_stack, cur);
                    ctx = CTX_READING_STRING;
                    string_start = pos;
                    continue;
                case '\t':
                    // printf("Ingoring tab character at %d\n", pos);
                    continue;
                case ' ':
                    // printf("Ingoring space character at %d\n", pos);
                    continue;
                case '\n':
                    // printf("Ingoring newline character at %d\n", pos);
                    continue;
                }
                // TODO return error?
                break;
            case CTX_READING_STRING:
                switch (cur) {
                case '\"':
                    stack_pop(braces_stack);
                    string_buf[pos-string_start-1] = '\0';
                    printf("Ended reading string %s (from %d to %d)\n", string_buf, string_start, pos);
                    free(string_buf);
                    string_buf = malloc(sizeof(char) * MAX_STRING_SIZE);
                    ctx = CTX_READING_VALUE;
                    break;
                default:
                    string_buf[pos-string_start-1] = cur;
                    continue;
                }
                break;
            }


    }

    if (braces_stack->cur != 0) {
        printf("Error: brace %c not closed\n", stack_pop(braces_stack));
    }

    printf("All good :)\n");
    stack_free(braces_stack);
    free(string_buf);

    result = r;
    return 0;
}