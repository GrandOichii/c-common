#include "list.h"

#include <stdlib.h>

list* new_list() {
    list* result = malloc(sizeof(list));
    result->root = 0;
    return result;
}

void list_free(list* l) {
    // TODO
    list_node* node = l->root;
    list_node* next;
    while (1) {
        if (!node) break;
        next = node->next;
        // node = node->next;
        free(node);
        node = next;
    }
    free(l);
}

void list_add(list* l, local_type value) {
    list_node* result = malloc(sizeof(list_node));
    result->next = 0;
    result->prev = 0;
    result->value = value;

    if (!l->root) {
        l->root = result;
        return;
    }

    list_node* parent = l->root;
    while (parent->next) parent = parent->next;
    parent->next = result;
    result->prev = parent;
}

int list_size(list* l) {
    int result = 0;
    list_node* node = l->root;
    while(node) {
        ++result;
        node = node->next;
    }
    return result;
}

local_type list_get(list* l, int index) {
    // ? add check
    list_node* result = l->root;
    while(index--) {
        result = result->next;
    }
    return result->value;
}

void list_insert(list* l, local_type value, int pos) {
    list_node* result = malloc(sizeof(list_node));
    result->next = 0;
    result->prev = 0;
    result->value = value;

    if (!pos) {
        if (l->root) {
            result->prev = l->root->prev;
            result->next = l->root;
        }
        l->root = result;
        return;
    }

    list_node* parent = l->root;
    while(pos--) {
        parent = parent->next;
    }

    // 0 1 3 [4]
    if (parent->prev) {
        parent->prev->next = result;
        result->prev = parent->prev;
    }
    
    parent->prev = result;
    result->next = parent;
    
}

void list_remove(list* l, int pos) {
    if (!pos) {
        list_node* new_root = l->root->next;
        if (!new_root) {
            l->root = 0;
            return;
        }
        new_root->prev = l->root->prev;
        new_root->next = l->root->next;
        l->root = new_root;
        return;
    }

    list_node* target = l->root;
    while (pos--) {
        target = target->next;
    }
    list_node* prev = target->prev;
    list_node* next = target->next;

    prev->next = next;
    if (next) {
        l->root->prev = next;
        next->prev = prev;
    }

    free(target);
}
