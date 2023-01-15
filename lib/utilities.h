#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

typedef char *multi_tok_t;

// Declarations
char *create_string(char *string);
char *lowercase(char *string);
char *uppercase(char *string);
char *trim(char *string);
char *remove_newline(char *string);
char *replace_all(char *string, char find, char replace);
char *concat_strings(int count, ...);
char *string_truncate(char *string, char *start, char *end);

void destroy_strings(int count, ...);

multi_tok_t init();
char *multi_tok(char *input, char *delimiter);

FILE *open_file(char *filename, char *mode);
void close_file(FILE *file, char *filename);

// Implementations

multi_tok_t s = NULL;

char *create_string(char *string) {
    char *str = (char *) malloc(strlen(string) + 1);
    strcpy(str, string);
    return str;
}

char *lowercase(char *string) {
    char *str = create_string(string);

    for (int i = 0; i < (int) strlen(str); i++) {
        str[i] = tolower(str[i]);
    }

    return str;
}

char *uppercase(char *string) {
    char *str = create_string(string);

    for (int i = 0; i < (int) strlen(str); i++) {
        str[i] = toupper(str[i]);
    }

    return str;
}

char *trim(char *string) {
    int i = 0;
    while (string[i] == ' ') { i++; }

    int j = strlen(string) - 1;
    while (string[j] == ' ') { j--; }

    char *trimmed = (char *) malloc(j - i + 2);
    strncpy(trimmed, string + i, j - i + 1);
    trimmed[j - i + 1] = '\0';

    return trimmed;
}

char *remove_newline(char *string) {
    char *trimmed = trim(string);
    trimmed[strlen(trimmed) - 1] = '\0';
    return trimmed;
}

char *replace_all(char *string, char find, char replace) {
    char *str = create_string(string);

    for (int i = 0; i < (int) strlen(str); i++) {
        if (str[i] == find) {
            str[i] = replace;
        }
    }

    return str;
}

char *concat_strings(int count, ...) {
    va_list args;
    va_start(args, count);

    int length = 0;
    for (int i = 0; i < count; i++) {
        length += strlen(va_arg(args, char *));
    }

    char *str = (char *) malloc(length + 1);
    str[0] = '\0';

    va_start(args, count);
    for (int i = 0; i < count; i++) {
        strcat(str, va_arg(args, char *));
    }

    return str;
}

char *string_truncate(char *string, char *start, char *end) {
    char *start_index = strstr(string, start);
    char *end_index = strstr(start_index + strlen(start), end);

    if (start_index == NULL || end_index == NULL) {
        return NULL;
    }

    int start_offset = strlen(start);
    // int end_offset = strlen(end);

    int length = end_index - start_index - start_offset;
    char *truncated = (char *) malloc(length + 1);
    strncpy(truncated, start_index + start_offset, length);
    truncated[length] = '\0';

    return truncated;
}


void destroy_strings(int count, ...) {
    va_list args;
    va_start(args, count);

    for (int i = 0; i < count; i++) {
        free(va_arg(args, char *));
    }
}


multi_tok_t init() {
    return NULL;
}

char *multi_tok(char *input, char *delimiter) {
    if (input != NULL)
        s = input;

    if (s == NULL)
        return s;

    char *end = strstr(s, delimiter);
    if (end == NULL) {
        char *temp = s;
        s = NULL;
        return temp;
    }

    char *temp = s;

    *end = '\0';
    s = end + strlen(delimiter);
    return temp;
}


FILE *open_file(char *filename, char *mode) {
    FILE *file = fopen(filename, mode);

    if (file == NULL) {
        printf("Error: Could not open file '%s'.\n", filename);
        exit(1);
    }

    return file;
}

void close_file(FILE *file, char *filename) {
    if (fclose(file) != 0) {
        printf("Error: Could not close file '%s'.\n", filename);
        exit(1);
    }
}



// This is dependent on the ARRAY.H library
#ifndef ARRAY_H
#include "./array.h"
#endif

#ifdef ARRAY_H

ARRAY tokenize(char *string, char *delimiter);

char *join_tokens(ARRAY array, char *delimiter);

ARRAY tokenize(char *string, char *delimiter) {
    ARRAY array = array_create(1);

    char *copy = remove_newline(string);
    char *token = multi_tok(copy, delimiter);

    while (token != NULL) {
        array_add(array, (void *) create_string(token));
        token = multi_tok(NULL, delimiter);
    }

    destroy_strings(1, copy);

    return array;
}

char *join_tokens(ARRAY array, char *delimiter) {
    char *str = create_string("");

    for (int i = 0; i < array->size; i++) {
        str = concat_strings(3, str, (char *) array->data[i], delimiter);
    }

    str[strlen(str) - strlen(delimiter)] = '\0';

    return str;
}


#endif


#endif