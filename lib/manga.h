#ifndef MANGA_H
#define MANGA_H

#include <stdio.h>
#include <stdlib.h>

#include "./array.h"

/* Disclaimer
 * This manga structure is based on the information provided by MangaSee.
 * If you want to adopt another website, you will need to change this structure.
 */

// MangaSee base URLs
#define BASE_URL        "https://mangasee123.com/"
#define DIRECTORY_URL   "https://mangasee123.com/directory/"
#define MANGA_URL       "https://mangasee123.com/manga/"
#define RSS_URL         "https://mangasee123.com/rss/"

struct CHAPTER {
    char *name;
    char *pub_date;

    char *url;

    float number;
};

struct MANGA {
    char *name;
    char *alternative_names;
    char *authors;
    char *genres;
    char *type;                 // Manga, Manhwa, Manhua, Doujinshi, OEL, One-Shot
    char *released;
    char *official_translation;
    char *status;
    char *description;

    char *url;
    char *image_url;
    char *rss_url;

    int chapters_count;         // Number of chapters
    int total_chapters;         // Total number of chapters (With .5 chapters)
    float latest_chapter;

    ARRAY chapters;
};

typedef struct MANGA *MANGA;
typedef struct CHAPTER *CHAPTER;

// Declarations

MANGA manga_create();
void manga_destroy(MANGA manga);
void manga_print(MANGA manga);


// Global variables

ARRAY all_genres;
ARRAY mangas;


// Implementations

MANGA manga_create() {
    MANGA manga = (MANGA) malloc(sizeof(struct MANGA));
    if (manga == NULL) { return NULL; }

    manga->name = NULL;
    manga->alternative_names = NULL;
    manga->authors = NULL;
    manga->genres = NULL;
    manga->type = NULL;
    manga->released = NULL;
    manga->official_translation = NULL;
    manga->status = NULL;
    manga->description = NULL;

    manga->url = NULL;
    manga->image_url = NULL;
    manga->rss_url = NULL;

    manga->chapters_count = 0;
    manga->total_chapters = 0;
    manga->latest_chapter = 0;

    manga->chapters = array_create(10);

    return manga;
}

void manga_destroy(MANGA manga) {
    if (manga == NULL) { return; }

    if (manga->name != NULL) { free(manga->name); }
    if (manga->alternative_names != NULL) { free(manga->alternative_names); }
    if (manga->authors != NULL) { free(manga->authors); }
    if (manga->genres != NULL) { free(manga->genres); }
    if (manga->type != NULL) { free(manga->type); }
    if (manga->released != NULL) { free(manga->released); }
    if (manga->official_translation != NULL) { free(manga->official_translation); }
    if (manga->status != NULL) { free(manga->status); }
    if (manga->description != NULL) { free(manga->description); }
    
    if (manga->url != NULL) { free(manga->url); }
    if (manga->image_url != NULL) { free(manga->image_url); }
    if (manga->rss_url != NULL) { free(manga->rss_url); }

    array_destroy(manga->chapters);

    free(manga);
}

void manga_print(MANGA manga) {
    if (manga == NULL) { return; }

    printf("{\n");
    printf("\t\"name\": \"%s\",\n"                  , manga->name);
    printf("\t\"alternative_names\": \"%s\",\n"     , manga->alternative_names);
    printf("\t\"authors\": \"%s\",\n"               , manga->authors);
    printf("\t\"genres\": \"%s\",\n"                , manga->genres);
    printf("\t\"type\": \"%s\",\n"                  , manga->type);
    printf("\t\"released\": \"%s\",\n"              , manga->released);
    printf("\t\"official_translation\": \"%s\",\n"  , manga->official_translation);
    printf("\t\"status\": \"%s\",\n"                , manga->status);
    printf("\t\"description\": \"%s\",\n"           , manga->description);
    
    printf("\t\"url\": \"%s\",\n"                   , manga->url);
    printf("\t\"image_url\": \"%s\"\n"              , manga->image_url);
    printf("\t\"rss_url\": \"%s\"\n"                , manga->rss_url);

    printf("\t\"chapters_count\": %d,\n"            , manga->chapters_count);
    printf("\t\"total_chapters\": %d,\n"            , manga->total_chapters);
    printf("\t\"latest_chapter\": %f,\n"            , manga->latest_chapter);

    printf("}\n");
}

void scan_directory() {
    void *data = simple_curl(DIRECTORY_URL);
    if (data == NULL) { return; }

    char *json_genres = string_truncate(data, "\"AllGenres\":[\"", "\"]");

    all_genres = tokenize(json_genres, "\",\"");

    if (DEBUG_MODE) {
        printf("Number of genres: %d\n", all_genres->size);
        for (int i = 0; i < all_genres->size; i++) {
            printf("%s\n", (char *) all_genres->data[i]);
        }
    }

    char *json_mangas = string_truncate(data, "\"Directory\":[", "]};");

    ARRAY json_mangas_array = tokenize(json_mangas, "},{");

    if (DEBUG_MODE && false) {
        for (int i = 0; i < json_mangas_array->size; i++) {
            printf("%s\n", (char *) json_mangas_array->data[i]);
        }
    }

    mangas = array_create_size(json_mangas_array->size, 100);

    for (int i = 0; i < json_mangas_array->size; i++) {
        MANGA manga = manga_create();

        char *temp = string_truncate((char *) json_mangas_array->data[i], "\"i\":\"", "\"");
        manga->url = concat_strings(3, MANGA_URL, temp, "/");
        destroy_strings(1, temp);

        manga->name = string_truncate((char *) json_mangas_array->data[i], "\"s\":\"", "\"");
        manga->status = string_truncate((char *) json_mangas_array->data[i], "\"st\":\"", "\"");

        temp = string_truncate(json_mangas_array->data[i], "\"g\":[", "]");
        ARRAY genres = tokenize(temp, ",");

        for (int i = 0; i < genres->size; i++) {
            int genre_index = atoi(genres->data[i]);
            char *genre = all_genres->data[genre_index];
            if (manga->genres == NULL) {
                manga->genres = create_string(genre);
            } else {
                manga->genres = concat_strings(3, manga->genres, ", ", genre);
            }
        }

        destroy_strings(1, temp);
        array_destroy(genres);

        array_add(mangas, manga);
    }

    manga_print(mangas->data[0]);

    destroy_strings(2, data, json_genres, json_mangas);
    array_destroy(all_genres);
    array_destroy(json_mangas_array);

    return;
}

#endif