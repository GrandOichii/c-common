#ifndef LIST_H
#define LIST_H

typedef int local_type;

typedef struct list_node {
    local_type value;
    struct list_node* prev;
    struct list_node* next;
} list_node;

typedef struct list {

    list_node* root;

} list;

list* new_list();
void list_free(list* l);
void list_add(list* l, local_type value);
int list_size(list* l);
local_type list_get(list* l, int index);
void list_insert(list* l, local_type value, int pos);
void list_remove(list* l, int pos);

#endif