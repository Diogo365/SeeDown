#ifndef SIMPLE_CURL_H
#define SIMPLE_CURL_H

#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char *memory;
    size_t size;
} MemoryStruct;

size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t real_size = size * nmemb;
    MemoryStruct *mem = (MemoryStruct *) userp;

    mem->memory = realloc(mem->memory, mem->size + real_size + 1);
    if (mem->memory == NULL) {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    memcpy(&(mem->memory[mem->size]), contents, real_size);
    mem->size += real_size;
    mem->memory[mem->size] = 0;

    return real_size;
}

void Curl(void *chunk, char *url) {
    CURL *curl_handle;
    CURLcode res;

    curl_handle = curl_easy_init();
    if (curl_handle) {
        curl_easy_setopt(curl_handle, CURLOPT_URL, url);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *) chunk);
        res = curl_easy_perform(curl_handle);
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        curl_easy_cleanup(curl_handle);
    }
}

/*
 * Returns a pointer to a string containing the contents of the url.
 * The string is allocated with malloc() and should be freed with free().
 */
void *simple_curl(char *url) {
    MemoryStruct chunk;
    chunk.memory = malloc(1);
    chunk.size = 0;

    Curl(&chunk, url);

    return chunk.memory;
}

#endif