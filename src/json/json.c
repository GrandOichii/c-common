// TODO add float support
// TODO throws errors when feeding empty object / array

#include "json.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../stack/stack.h"

#define MAX_STRING_SIZE 1024

#define CTX_READING_VALUE 0
#define CTX_READING_OBJECT_KEY 1
#define CTX_READING_OBJECT_VALUE 2
#define CTX_READING_OBJECT_KEY_VALUE 3
#define CTX_READING_ARRAY 4

#define JSON_STRING 0
#define JSON_DOUBLE 1
#define JSON_OBJECT 2
#define JSON_ARRAY 3

const char* read_string(const char* string, int* pos) {
    // TODO add escape
    char cur;
    int start = *pos;
    char* result = malloc(sizeof(char) * MAX_STRING_SIZE);
    while ((cur = string[*pos]) != '\0') {
        (*pos)++;
        switch(cur) {
        case '\"':
            result[*pos - start - 1] = '\0';
            return result;
        default:
            result[*pos - start - 1] = cur;
            break;
        }
    }
    printf("String brace not closed!\n");
}

int ignore_ch(const char c) {
    switch (c) {
        case '\t':
            return 1;
        case ' ':
            return 1;
        case '\n':
            return 1;
    }
    return 0;
}

char next_ch(const char* string, int* pos) {
    char result;
    
    do {
        result = string[*pos];
        ++(*pos);
    } while (!result != '\0' || ignore_ch(result));

    return result;
}

int is_number(char c) {
    // TODO add double
    return (c >= '0' && c <= '9') || c == '.';
}

int try_read_number(const char* string, int* pos, double* result, char first) {
    int p = *pos;
    if (!is_number(first)) return 1;
    
    char* result_s = malloc(sizeof(char) * MAX_STRING_SIZE);

    result_s[0] = first;
    while (is_number(string[p])) {
        result_s[p - *pos + 1] = string[p];
        ++p;
    }
    result_s[p - *pos + 1] = '\0';

    *pos = p;

    *result = strtod(result_s, NULL);
    free(result_s);
    return 0; 
}


int do_parse(const char* string, int* pos, stack* braces, json_node* result) {
    json_node* res = malloc(sizeof(json_node));
    int ctx = CTX_READING_VALUE;
    char cur;

    while ((cur = next_ch(string, pos)) != '\0') {

        switch (ctx) {

        case CTX_READING_VALUE:
            double num_value; 
            int err = try_read_number(string, pos, &num_value, cur);
            if (!err) {
                res->type = JSON_DOUBLE;
                res->double_val = num_value;
                goto exit_success;
            }
            switch(cur) {
            case '\"':
                const char* str = read_string(string, pos);
                res->type = JSON_STRING;
                res->string_val = str;
                goto exit_success;
            case '{':
                res->type = JSON_OBJECT;
                ctx = CTX_READING_OBJECT_KEY_VALUE;
                continue;
            case '[':
                res->type = JSON_ARRAY;
                ctx = CTX_READING_ARRAY;
                continue;
            default:
                printf("Err: unexpected character %c at %d\n", cur, *pos);
                goto exit_fail;
            }

        case CTX_READING_OBJECT_KEY_VALUE:
            json_kvp* kvp = malloc(sizeof(json_kvp));
            const char* key = read_string(string, pos);
            char next = next_ch(string, pos);
            if (next != ':') {
                printf("Err: failed to locate semicolon after key declaration at pos %d\n", *pos);
                goto exit_fail;
            }
            json_node* value = malloc(sizeof(json_node));

            int perr = do_parse(string, pos, braces, value);
            if (perr) {
                goto exit_fail;
            }

            kvp->string_key = key;
            kvp->value = value;

            res->children[res->child_count] = *kvp;
            ++res->child_count;

            next = next_ch(string, pos);
            if (next == '}') goto exit_success;
            if (next == ',') continue;

            printf("Err: unexpected character when reading object key values at pos %c\n", next);
            goto exit_fail;

        case CTX_READING_ARRAY:
            // TODO
            --(*pos);

            json_kvp* akvp = malloc(sizeof(json_kvp));
            json_node* avalue = malloc(sizeof(json_node));
            int aerr = do_parse(string, pos, braces, avalue);
            if (aerr) {
                return aerr;
            }
            akvp->int_key = res->child_count;
            akvp->value = avalue;

            res->children[res->child_count] = *akvp;
            ++res->child_count;

            next = next_ch(string, pos);
            if (next == ']') goto exit_success;
            if (next == ',') continue;

            printf("Err: unexpected character when reading array values at pos %c\n", next);
            goto exit_fail;
        }

    }

exit_success:
    *result = *res;
    return 0;
exit_fail:
    free(res);
    return 1;
}

int parse(const char* string, json_node* result) {
    int pos = 0;
    stack* braces = new_stack();
    int res = do_parse(string, &pos, braces, result);
    
    stack_free(braces);

    return res;
}

const char* create_indent(int indent, int layer) {
    int size = indent * layer;
    char* result = malloc(sizeof(char) * size + 1);
    for (int i = 0; i < size; i++) result[i] = ' ';
    result[size] = '\0';
    return result;
}

const char* stringify_node(const json_node* json, int indent, int layer);

const char* stringify_kvp(const json_kvp* kvp, JSON_TYPE parent_type, int indent, int layer) {
    char* result;
    const char* vstring = stringify_node(kvp->value, indent, layer);

    switch (parent_type){
    case JSON_OBJECT:
        result = malloc(sizeof(char) * MAX_STRING_SIZE);
        result[0] = '\0';
        if (indent) {
            strcat(result, create_indent(indent, layer));
        }
        strcat(result, "\"");
        strcat(result, kvp->string_key);
        strcat(result, indent == 0 ? "\":" : "\": ");
        strcat(result, vstring);
        // free(vstring);
        return result;
        break;
    default:
        if (indent) {
            vstring = strcat(create_indent(indent, layer), vstring);
        }
        return vstring;
        break;
    }
}

const char* stringify_node(const json_node* json, int indent, int layer) {
    char* result = malloc(sizeof(char) * 20);
    switch (json->type){
    case JSON_STRING:
        result[0] = '\"';
        result[1] = '\0';
        strcat(result, json->string_val);
        strcat(result, "\"");
        return result;
        break;
    case JSON_DOUBLE:
        sprintf(result, "%f", json->double_val);
        return result; 
    case JSON_OBJECT:
        result[0] = '{';
        result[1] = '\0';
        for (int i = 0; i < json->child_count; i++) {
            if (i != 0) {
                strcat(result, ",");
            }
            if (indent) {
                strcat(result, "\n");
            }
            strcat(result, stringify_kvp(&json->children[i], json->type, indent, layer+1));
        }
        if (indent) {
            strcat(result, "\n");
            strcat(result, create_indent(indent, layer));
        }
        strcat(result, "}");
        return result;
    case JSON_ARRAY:
        result[0] = '[';
        result[1] = '\0';
        for (int i = 0; i < json->child_count; i++) {
            if (i != 0) {
                strcat(result, ",");
            }
            if (indent) {
                strcat(result, "\n");
            }
            strcat(result, stringify_kvp(&json->children[i], json->type, indent, layer+1));
        }
        if (indent) {
            strcat(result, "\n");
            strcat(result, create_indent(indent, layer));
        }
        strcat(result, "]");
        return result;
    default:
        return "err";
        break;
    }
}

const char* stringify(const json_node* json) {
    return stringify_node(json, 0, 0);
}

const char* stringify_pretty(const json_node* json, int indent) {
    return stringify_node(json, indent, 0);
}


