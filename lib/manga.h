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

    char *db_name;
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

CHAPTER chapter_create();
void chapter_destroy(CHAPTER chapter);
void chapter_print(CHAPTER chapter);


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

    manga->db_name = NULL;
    manga->url = NULL;
    manga->image_url = NULL;
    manga->rss_url = NULL;

    manga->chapters_count = 0;
    manga->total_chapters = 0;
    manga->latest_chapter = 0;

    manga->chapters = NULL;

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
    
    if (manga->db_name != NULL) { free(manga->db_name); }
    if (manga->url != NULL) { free(manga->url); }
    if (manga->image_url != NULL) { free(manga->image_url); }
    if (manga->rss_url != NULL) { free(manga->rss_url); }

    array_destroy_struct(manga->chapters, (void *) chapter_destroy);

    free(manga);
}

void manga_print(MANGA manga) {
    if (manga == NULL) { return; }

    printf("{\n");
    printf("    \"name\": \"%s\",\n"                  , manga->name);
    printf("    \"alternative_names\": \"%s\",\n"     , manga->alternative_names);
    printf("    \"authors\": \"%s\",\n"               , manga->authors);
    printf("    \"genres\": \"%s\",\n"                , manga->genres);
    printf("    \"type\": \"%s\",\n"                  , manga->type);
    printf("    \"released\": \"%s\",\n"              , manga->released);
    printf("    \"official_translation\": \"%s\",\n"  , manga->official_translation);
    printf("    \"status\": \"%s\",\n"                , manga->status);
    printf("    \"description\": \"%s\",\n"           , manga->description);
    
    printf("    \"db_name\": \"%s\",\n"               , manga->db_name);
    printf("    \"url\": \"%s\",\n"                   , manga->url);
    printf("    \"image_url\": \"%s\"\n"              , manga->image_url);
    printf("    \"rss_url\": \"%s\"\n"                , manga->rss_url);

    printf("    \"chapters_count\": %d,\n"            , manga->chapters_count);
    printf("    \"total_chapters\": %d,\n"            , manga->total_chapters);
    printf("    \"latest_chapter\": %.3f,\n"          , manga->latest_chapter);

    printf("}\n");
}

CHAPTER chapter_create() {
    CHAPTER chapter = (CHAPTER) malloc(sizeof(struct CHAPTER));
    if (chapter == NULL) { return NULL; }

    chapter->name = NULL;
    chapter->pub_date = NULL;
    chapter->url = NULL;
    chapter->number = 0;

    return chapter;
}

void chapter_destroy(CHAPTER chapter) {
    if (chapter == NULL) { return; }

    if (chapter->name != NULL) { free(chapter->name); }
    if (chapter->pub_date != NULL) { free(chapter->pub_date); }
    if (chapter->url != NULL) { free(chapter->url); }

    free(chapter);
}

void chapter_print(CHAPTER chapter) {
    if (chapter == NULL) { return; }

    printf("{\n");
    printf("    \"name\": \"%s\",\n"                  , chapter->name);
    printf("    \"pub_date\": \"%s\",\n"              , chapter->pub_date);
    printf("    \"url\": \"%s\",\n"                   , chapter->url);
    printf("    \"number\": %.3f\n"                   , chapter->number);
    printf("}\n");
}

void scan_directory() {
    void *data = simple_curl(DIRECTORY_URL);
    if (data == NULL) { return; }

    char *json_genres = string_truncate(data, "\"AllGenres\":[\"", "\"]");

    all_genres = tokenize(json_genres, "\",\"");

    // if (DEBUG_MODE) {
    //     printf("Number of genres: %d\n", all_genres->size);
    //     for (int i = 0; i < all_genres->size; i++) {
    //         printf("%s\n", (char *) all_genres->data[i]);
    //     }
    // }

    char *json_mangas = string_truncate(data, "\"Directory\":[", "]};");

    ARRAY json_mangas_array = tokenize(json_mangas, "},{");

    // if (DEBUG_MODE) {
    //     for (int i = 0; i < json_mangas_array->size; i++) {
    //         printf("%s\n", (char *) json_mangas_array->data[i]);
    //     }
    // }

    mangas = array_create_size(json_mangas_array->size, 100);

    for (int i = 0; i < json_mangas_array->size; i++) {
        MANGA manga = manga_create();

        manga->name = string_truncate((char *) json_mangas_array->data[i], "\"s\":\"", "\"");
        deci_unicode_converter(manga->name);

        manga->status = string_truncate((char *) json_mangas_array->data[i], "\"st\":\"", "\"");

        char *temp = string_truncate(json_mangas_array->data[i], "\"g\":[", "]");
        ARRAY genres = tokenize(temp, ",");

        for (int i = 0; i < genres->size; i++) {
            int genre_index = atoi(genres->data[i]);
            char *genre = all_genres->data[genre_index];
            if (manga->genres == NULL) {
                manga->genres = string_create(genre);
            } else {
                char *point = manga->genres;
                manga->genres = concat_strings(3, manga->genres, ", ", genre);
                destroy_strings(1, point);
            }
        }

        manga->db_name = string_truncate((char *) json_mangas_array->data[i], "\"i\":\"", "\"");
        manga->url = concat_strings(2, MANGA_URL, manga->db_name);
        manga->rss_url = concat_strings(3, RSS_URL, manga->db_name, ".xml");

        destroy_strings(1, temp);
        array_destroy(genres);

        array_add(mangas, manga);

        // if (DEBUG_MODE) {
        //     manga_print(manga);
        //     break;
        // }
    }

    destroy_strings(3, data, json_genres, json_mangas);
    array_destroy(json_mangas_array);

    return;
}

void scan_rss(MANGA manga) {
    if (manga == NULL) { return; }

    void *data = simple_curl(manga->rss_url);
    if (data == NULL) { return; }

    // Image processing
    char *image_xml = string_truncate(data, "<image>", "</image>");
    manga->image_url = string_truncate(image_xml, "<url>", "</url>");

    // Chapters processing
    char *item_xml = string_truncate(data, "</image>", "</channel>");
    ARRAY items = tokenize(item_xml, "</item>");

    manga->total_chapters = items->size - 1;
    manga->chapters = array_create_size(items->size, 10);

    for (int i = manga->total_chapters - 1; i >= 0; i--) {
        CHAPTER chapter = chapter_create();

        chapter->name = string_truncate(items->data[i], "<title>", "</title>");
        html_unicode_converter(chapter->name);
        deci_unicode_converter(chapter->name);

        chapter->url = string_truncate(items->data[i], "<link>", "</link>");        
        chapter->pub_date = string_truncate(items->data[i], "<pubDate>", "</pubDate>");

        char *temp = concat_strings(3, ">", manga->db_name, "-");
        char *number = string_truncate(items->data[i], temp, "</");
        chapter->number = atof(number);

        destroy_strings(2, temp, number);

        array_add(manga->chapters, chapter);
    }

    manga->latest_chapter = ((CHAPTER) manga->chapters->data[manga->total_chapters - 1])->number;

    for (int i = manga->total_chapters - 1; i > 0; i--) {
        CHAPTER chapter = (CHAPTER) manga->chapters->data[i];
        if (chapter->number == (int) chapter->number) {
            manga->chapters_count = chapter->number;
            break;
        }
    }

    array_destroy(items);
    destroy_strings(3, data, image_xml, item_xml);

    return;
}

void scan_manga(MANGA manga) {
    if (manga == NULL) { return; }

    manga_print(manga);

    void *data = simple_curl(manga->url);
    char *temp;

    // Alternate Name(s) processing
    char *alt_name_xml = string_truncate(data, "Alternate Name(s):</span>", "</i>");
    if (alt_name_xml != NULL) {
        remove_all_inplace(alt_name_xml, '\r');
        remove_all_inplace(alt_name_xml, '\n');
        remove_all_inplace(alt_name_xml, '\t');
        string_trim_inplace(alt_name_xml);

        manga->alternative_names = string_create(alt_name_xml);
    }

    // Author(s) processing
    char *author_xml = string_truncate(data, "Author(s):</span>", "</i>");
    if (author_xml != NULL) {
        remove_all_inplace(author_xml, '\r');
        remove_all_inplace(author_xml, '\n');
        remove_all_inplace(author_xml, '\t');
        string_trim_inplace(author_xml);
        
        ARRAY authors = tokenize(author_xml, ">, <");
        for (int i = 0; i < authors->size; i++) {
            temp = string_truncate((char *) authors->data[i], "'>", "<");
            if (manga->authors == NULL) {
                manga->authors = string_create(temp);
            } else {
                char *point = manga->authors;
                manga->authors = concat_strings(3, manga->authors, ", ", temp);
                destroy_strings(1, point);
            }
            destroy_strings(1, temp);
        }

        array_destroy(authors);
    }
    
    // Type processing
    char *type_xml = string_truncate(data, "Type:</span>", "</i>");
    if (type_xml != NULL) {
        remove_all_inplace(type_xml, '\r');
        remove_all_inplace(type_xml, '\n');
        remove_all_inplace(type_xml, '\t');
        string_trim_inplace(type_xml);

        manga->type = string_truncate(type_xml, ">", "</a>");
    }

    // Released processing
    char *released_xml = string_truncate(data, "Released:</span>", "</i>");
    if (released_xml != NULL) {
        remove_all_inplace(released_xml, '\r');
        remove_all_inplace(released_xml, '\n');
        remove_all_inplace(released_xml, '\t');
        string_trim_inplace(released_xml);

        manga->released = string_truncate(released_xml, ">", "</a>");
    }

    // Official Translation processing
    char *official_translation_xml = string_truncate(data, "Official Translation:</span>", "</i>");
    if (official_translation_xml != NULL) {
        remove_all_inplace(official_translation_xml, '\r');
        remove_all_inplace(official_translation_xml, '\n');
        remove_all_inplace(official_translation_xml, '\t');
        string_trim_inplace(official_translation_xml);

        manga->official_translation = string_truncate(official_translation_xml, ">", "</a>");
    }

    // Description processing
    char *description_xml = string_truncate(data, "Description:</span>", "</i>");
    if (description_xml != NULL) {
        remove_all_inplace(description_xml, '\r');
        remove_all_inplace(description_xml, '\n');
        remove_all_inplace(description_xml, '\t');
        string_trim_inplace(description_xml);

        manga->description = string_truncate(description_xml, ">", "</div>");
        html_unicode_converter(manga->description);
        deci_unicode_converter(manga->description);
    }

    manga_print(manga);

    destroy_strings(7, 
                    data, 
                    alt_name_xml, 
                    author_xml, 
                    type_xml, 
                    released_xml, 
                    official_translation_xml, 
                    description_xml);

    return;
}

#endif