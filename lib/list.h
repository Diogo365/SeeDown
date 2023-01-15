#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct LIST_NODE {
    void *data;

    struct LIST_NODE *next;
};

typedef struct LIST_NODE *LIST_NODE;

struct list {
    int size;

    LIST_NODE head;
    LIST_NODE tail;
};

typedef struct list *LIST;


// Node related operations
LIST_NODE list_node_new(void *data);
LIST_NODE list_node_destroy(LIST_NODE node);


// List related operations
LIST list_create();
LIST list_append(LIST list, void *data);
LIST list_copy(LIST list);
LIST list_insert(LIST list, void *data, int index);
LIST list_reverse(LIST list);
LIST list_sort(LIST list, int (*compare)(void *, void *));
LIST list_clear(LIST list);
LIST list_destroy(LIST list);

void *list_pop(LIST list, int index);
void *list_remove(LIST list, void *data, int (*compare)(void *, void *));

int list_count(LIST list);

void list_print(LIST list, void (*print)(void *));


// Queue related operations
LIST list_enqueue(LIST list, void *data);
LIST list_dequeue(LIST list);
void *list_peek(LIST list);


// Implementations
LIST_NODE list_node_new(void *data) {
    LIST_NODE node = (LIST_NODE) malloc(sizeof(LIST_NODE));

    node->data = data;
    node->next = NULL;

    return node;
}

LIST_NODE list_node_destroy(LIST_NODE node) {
    free(node);

    return NULL;
}



LIST list_create() {
    LIST list = (LIST) malloc(sizeof(struct list));

    list->size = 0;
    list->head = NULL;
    list->tail = NULL;

    return list;
}

LIST list_append(LIST list, void *data) {
    LIST_NODE node = list_node_new(data);

    if (list->head == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        list->tail = node;
    }

    list->size++;

    return list;
}

LIST list_copy(LIST list) {
    LIST copy_list = list_create();

    LIST_NODE node = list->head;

    while (node != NULL) {
        list_append(copy_list, node->data);
        node = node->next;
    }

    return copy_list;
}

LIST list_insert(LIST list, void *data, int index) {
    LIST_NODE node = list_node_new(data);

    if (index < 0) {
        return list;
    } else if (index == 0) {
        node->next = list->head;
        list->head = node;
    } else {
        LIST_NODE prev = list->head;

        for (int i = 0; i < index - 1; i++) {
            if (prev->next == NULL) { return NULL; }

            prev = prev->next;
        }

        node->next = prev->next;
        prev->next = node;
    }

    list->size++;

    return list;
}

LIST list_reverse(LIST list) {
    LIST_NODE prev = NULL;
    LIST_NODE node = list->head;
    LIST_NODE next = NULL;

    while (node != NULL) {
        next = node->next;
        node->next = prev;
        prev = node;
        node = next;
    }

    list->head = prev;

    return list;
}

LIST list_sort(LIST list, int (*compare)(void *, void *)) {
    LIST_NODE node = list->head;

    while (node != NULL) {
        LIST_NODE next = node->next;

        while (next != NULL) {
            if (compare(node->data, next->data) > 0) {
                void *data = node->data;
                node->data = next->data;
                next->data = data;
            }

            next = next->next;
        }

        node = node->next;
    }

    return list;
}

LIST list_clear(LIST list) {
    LIST_NODE node = list->head;

    while (node != NULL) {
        LIST_NODE next = node->next;
        list_node_destroy(node);
        node = next;
    }

    list->size = 0;
    list->head = NULL;
    list->tail = NULL;

    return list;
}

LIST list_destroy(LIST list) {
    list_clear(list);
    free(list);

    return NULL;
}



void *list_pop(LIST list, int index) {
    LIST_NODE node = list->head;

    if (index == 0) {
        list->head = node->next;
    } else {
        LIST_NODE prev = list->head;

        for (int i = 0; i < index - 1; i++) {
            if (prev->next == NULL) { return NULL; }

            prev = prev->next;
        }

        node = prev->next;
        prev->next = node->next;
    }

    list->size--;

    void *data = node->data;
    list_node_destroy(node);

    return data;
}

void *list_remove(LIST list, void *data, int (*compare)(void *, void *)) {
    LIST_NODE node = list->head;

    if (compare(node->data, data) == 0) {
        list->head = node->next;
    } else {
        LIST_NODE prev = list->head;

        while (prev->next != NULL) {
            if (prev->next == NULL) { return NULL; }

            if (compare(prev->next->data, data) == 0) {
                node = prev->next;
                prev->next = node->next;
                break;
            }

            prev = prev->next;
        }
    }

    list->size--;

    void *node_data = node->data;
    list_node_destroy(node);

    return node_data;
}



int list_count(LIST list) {
    return list->size;
}



void list_print(LIST list, void (*print)(void *)) {
    LIST_NODE node = list->head;

    if (node == NULL) {
        printf("{}\n");
    }

    for (int i = 1; node != NULL; i++) {
        printf("Node %d: {\n", i);
        print(node->data);
        printf("}\n");
        node = node->next;
    }
}



LIST list_enqueue(LIST list, void *data) {
    return list_append(list, data);
}

LIST list_dequeue(LIST list) {
    return list_pop(list, 0);
}

void *list_peek(LIST list) {
    return list->head->data;
}
#endif