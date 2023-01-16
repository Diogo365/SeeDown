#ifndef ARRAY_H
#define ARRAY_H

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

struct ARRAY {
    int size;
    int step;
    int capacity;
    void **data;
};

typedef struct ARRAY *ARRAY;

ARRAY array_create(int step);
ARRAY array_create_size(int size, int step);
ARRAY array_increase(ARRAY array);
ARRAY array_reduce(ARRAY array);
ARRAY array_add(ARRAY array, void *data);
ARRAY array_add_at(ARRAY array, void *data, int index);
ARRAY array_remove(ARRAY array, int index);
ARRAY array_clear(ARRAY array);
ARRAY array_destroy(ARRAY array);

void array_print(ARRAY array, void (*print)(void *data));


ARRAY array_create(int step) {
    step = step > 0 ? step : 1;

    ARRAY array = (ARRAY) malloc(sizeof(struct ARRAY));

    array->size = 0;
    array->step = step;
    array->capacity = step;
    array->data = (void **) malloc(sizeof(void *) * step);

    return array;
}

ARRAY array_create_size(int size, int step) {
    step = step > 0 ? step : 1;

    ARRAY array = (ARRAY) malloc(sizeof(struct ARRAY));

    array->size = 0;
    array->step = step;
    array->capacity = size;
    array->data = (void **) malloc(sizeof(void *) * (size));

    return array;
}

ARRAY array_increase(ARRAY array) {
    array->capacity += array->step;

    void **data = (void **) malloc(sizeof(void *) * array->capacity);
    memcpy(data, array->data, sizeof(void *) * array->size);
    free(array->data);
    array->data = data;

    return array;
}

ARRAY array_reduce(ARRAY array) {
    array->capacity -= array->step;

    void **data = (void **) malloc(sizeof(void *) * array->capacity);
    memcpy(data, array->data, sizeof(void *) * array->size);
    free(array->data);
    array->data = data;

    return array;
}

ARRAY array_add(ARRAY array, void *data) {
    if (array->size == array->capacity)
        array = array_increase(array);

    array->data[array->size] = data;
    array->size++;

    return array;
}

ARRAY array_add_at(ARRAY array, void *data, int index) {
    if (index < 0 || index > array->size)
        return array;

    if (array->size == array->capacity)
        array = array_increase(array);

    for (int i = array->size; i > index; i--)
        array->data[i] = array->data[i - 1];

    array->data[index] = data;
    array->size++;

    return array;
}

ARRAY array_remove(ARRAY array, int index) {
    if (index < 0 || index >= array->size)
        return array;

    for (int i = index; i < array->size - 1; i++)
        array->data[i] = array->data[i + 1];

    array->size--;

    if (array->size < array->capacity - array->step)
        array = array_reduce(array);

    return array;
}

ARRAY array_clear(ARRAY array) {
    int step = array->step;

    array_destroy(array);

    array = array_create(step);

    return array;
}

ARRAY array_destroy(ARRAY array) {
    if (array == NULL)
        return NULL;

    for (int i = 0; i < array->size; i++)
        free(array->data[i]);

    free(array->data);
    free(array);

    return NULL;
}

ARRAY array_destroy_struct(ARRAY array, void (*destroy)(void *data)) {
    if (array == NULL || destroy == NULL)
        return NULL;

    for (int i = 0; i < array->size; i++)
        destroy(array->data[i]);

    free(array->data);
    free(array);

    return NULL;
}


void array_print(ARRAY array, void (*print)(void *data)) {
    printf("[");

    for (int i = 0; i < array->size; i++) {
        print(array->data[i]);

        if (i < array->size - 1)
            printf(", ");
    }

    printf("]");
}

#endif