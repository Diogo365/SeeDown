#ifndef MAL_H
#define MAL_H

#include "array.h"
#include "utilities.h"
#include "simple_curl.h"

/* Disclaimer
 * This following structures are based on the information provided by MyAnimeList.
 * If you want to adopt another website, you will need to change these structures.
 */

// MyAnimeList base URLs
#define QUERY_URL "https://myanimelist.net/manga.php?cat=manga&q="

struct QUERY {
    char *url;

    char *title;
    char *description;
    char *type;
    char *vol;
    char *score;

    bool details;

    ARRAY alternative_titles;

    char *synopsis;
    char *background;
    // char *related; // Removed (Too complex to parse)

    // Information
    // char *type; // Already defined
    char *volumes;
    char *chapters;
    char *status;
    char *published;
    char *genres;
    char *themes;
    char *demographic;
    char *serialization;
    char *authors;

    // Statistics
    // char *score; // Already defined
    char *rank;
    char *popularity;
    char *members;
    char *favorites;
};

typedef struct QUERY *QUERY;

// Declarations

QUERY query_create();
void query_destroy(QUERY query);
void query_print(QUERY query);

ARRAY mal_query(char *string, int limit);
ARRAY mal_parse(ARRAY rows, int limit);

// Implementations

QUERY query_create() {
    QUERY query = (QUERY) malloc(sizeof(struct QUERY));
    if (query == NULL) { return NULL; }

    query->url = NULL;

    query->title = NULL;
    query->description = NULL;
    query->type = NULL;
    query->vol = NULL;
    query->score = NULL;

    query->details = false;

    query->alternative_titles = NULL;

    query->synopsis = NULL;
    query->background = NULL;
    // query->related = NULL;

    // Information
    query->type = NULL;
    query->volumes = NULL;
    query->chapters = NULL;
    query->status = NULL;
    query->published = NULL;
    query->genres = NULL;
    query->themes = NULL;
    query->demographic = NULL;
    query->serialization = NULL;
    query->authors = NULL;

    // Statistics
    query->score = NULL;
    query->rank = NULL;
    query->popularity = NULL;
    query->members = NULL;
    query->favorites = NULL;

    return query;
}

void query_destroy(QUERY query) {
    if (query == NULL) { return; }

    if (query->url != NULL) { free(query->url); }

    if (query->title != NULL) { free(query->title); }
    if (query->description != NULL) { free(query->description); }
    if (query->type != NULL) { free(query->type); }
    if (query->vol != NULL) { free(query->vol); }
    if (query->score != NULL) { free(query->score); }

    if (query->alternative_titles != NULL) { array_destroy(query->alternative_titles); }

    if (query->synopsis != NULL) { free(query->synopsis); }
    if (query->background != NULL) { free(query->background); }
    // if (query->related != NULL) { free(query->related); }

    // Information
    // if (query->type != NULL) { free(query->type); }
    if (query->volumes != NULL) { free(query->volumes); }
    if (query->chapters != NULL) { free(query->chapters); }
    if (query->status != NULL) { free(query->status); }
    if (query->published != NULL) { free(query->published); }
    if (query->genres != NULL) { free(query->genres); }
    if (query->themes != NULL) { free(query->themes); }
    if (query->demographic != NULL) { free(query->demographic); }
    if (query->serialization != NULL) { free(query->serialization); }
    if (query->authors != NULL) { free(query->authors); }

    // Statistics
    // if (query->score != NULL) { free(query->score); }
    if (query->rank != NULL) { free(query->rank); }
    if (query->popularity != NULL) { free(query->popularity); }
    if (query->members != NULL) { free(query->members); }
    if (query->favorites != NULL) { free(query->favorites); }

    free(query);
}

void query_print(QUERY query) {
    if (query == NULL) { return; }

    printf("{\n");
    printf("    \"url\": \"%s\"\n", query->url);
    printf("    \"title\": \"%s\",\n", query->title);
    printf("    \"description\": \"%s\",\n", query->description);
    printf("    \"type\": \"%s\",\n", query->type);
    printf("    \"vol\": \"%s\",\n", query->vol);
    printf("    \"score\": \"%s\",\n", query->score);

    if (query->alternative_titles != NULL) {
        printf("    \"alternative_titles\": [\n");
        for (int i = 0; i < query->alternative_titles->size; i++) {
            char *title = (char *) query->alternative_titles->data[i];
            printf("        \"%s\",\n", title);
        }
        printf("    ],\n");
    }

    printf("    \"synopsis\": \"%s\",\n", query->synopsis);
    printf("    \"background\": \"%s\",\n", query->background);
    // printf("    \"related\": \"%s\",\n", query->related);
    
    // Information
    printf("    \"type\": \"%s\",\n", query->type);
    printf("    \"volumes\": \"%s\",\n", query->volumes);
    printf("    \"chapters\": \"%s\",\n", query->chapters);
    printf("    \"status\": \"%s\",\n", query->status);
    printf("    \"published\": \"%s\",\n", query->published);
    printf("    \"genres\": \"%s\",\n", query->genres);
    printf("    \"themes\": \"%s\",\n", query->themes);
    printf("    \"demographic\": \"%s\",\n", query->demographic);
    printf("    \"serialization\": \"%s\",\n", query->serialization);
    printf("    \"authors\": \"%s\",\n", query->authors);

    // Statistics
    printf("    \"score\": \"%s\",\n", query->score);
    printf("    \"rank\": \"%s\",\n", query->rank);
    printf("    \"popularity\": \"%s\",\n", query->popularity);
    printf("    \"members\": \"%s\",\n", query->members);
    printf("    \"favorites\": \"%s\",\n", query->favorites);

    printf("}\n");
}


// Related to search queries done by the user

ARRAY mal_query(char *input, int limit) {
    // printf("Querying MAL for '%s'...\n", string);
    char *string = string_to_url(string_create(input));

    char *url = string_concat(false, 2, QUERY_URL, string);

    void *data = simple_curl(url);
    if (data == NULL) { return NULL; }

    data = remove_all(data, '\r');
    data = remove_all(data, '\n');
    data = remove_all(data, '\t');
    data = string_trim(data, true);

    char *table = string_truncate(data, "<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\" width=\"100%\">", "</table>", false);
    if (table == NULL) { return NULL; }

    ARRAY rows = tokenize(table, "</tr>");
    if (rows == NULL) { return NULL; }

    ARRAY queries = mal_parse(rows, limit);

    strings_destroy(3, url, data, table);
    array_destroy(rows);

    return queries;
}

ARRAY mal_parse(ARRAY rows, int limit) {
    // printf("Parsing MAL results...\n");
    ARRAY queries = array_create(0);

    for (int i = 1; i < rows->size - 1 && i <= limit; i++) {
        char *row = (char *) rows->data[i];
        if (row == NULL) { continue; }

        QUERY query = query_create();

        query->title = string_truncate(row, "<strong>", "</strong>", false);
        query->title = deci_unicode_converter(query->title);
        query->title = html_unicode_converter(query->title);

        query->description = string_truncate(row, "<div class=\"pt4\">", "<a", false);
        query->description = deci_unicode_converter(query->description);
        query->description = html_unicode_converter(query->description);

        query->type = string_trim(string_truncate(row, "\"45\">", "</td>", false), true);

        query->vol = string_trim(string_truncate(row, "\"40\">", "</td>", false), true);

        query->score = string_trim(string_truncate(strstr(row, "div"), "\"50\">", "</td>", false), true);

        query->url = string_truncate(row, "href=\"", "\"", false);

        array_add(queries, query);
    }

    // printf("Found %d results.\n", queries->size);

    return queries;
}


// Related to detailed lookup of a specific manga

QUERY mal_lookup(QUERY query) {
    if (query == NULL) { return NULL; }
    if (query->details) { return query; }

    // printf("Looking up '%s'...\n", query->title);

    void *data = simple_curl(query->url);
    if (data == NULL) { return NULL; }

    data = remove_all(data, '\r');
    data = remove_all(data, '\n');
    data = remove_all(data, '\t');
    data = string_trim(data, true);

    char *pointer = NULL;


    // Alternative titles

    char *alt_titles = string_truncate(data, "<h2>Alternative Titles</h2>", "<h2>", false);
    pointer = alt_titles;

    ARRAY alternative_titles = array_create(0);

    while ((pointer = strstr(pointer, "\"dark_text\">")) != NULL) {
        char *language = string_truncate(pointer, "\"dark_text\">", "</span>", false);
        char *title = string_truncate(pointer, "</span>", "</div>", false);
        title = deci_unicode_converter(title);
        title = html_unicode_converter(title);
        title = string_trim(title, true);

        char *alt_title = string_concat(false, 3, language, " ", title);

        array_add(alternative_titles, alt_title);

        pointer = pointer + 1;

        strings_destroy(2, language, title);
    }

    query->alternative_titles = alternative_titles;


    // Synopsis

    char *synopsis  = string_truncate(data, "</div>Synopsis", "<h2", false);
    query->synopsis = string_truncate(synopsis, "\"description\">", "</span>", false);
    query->synopsis = deci_unicode_converter(query->synopsis);
    query->synopsis = html_unicode_converter(query->synopsis);
    query->synopsis = string_trim(query->synopsis, true);
    query->synopsis = replace_all(query->synopsis, "<br />", "\n");
    query->synopsis = html_remove_italic(query->synopsis);
    query->synopsis = html_remove_bold(query->synopsis);

    // Background

    char *background  = string_truncate(data, "</div>Background", "<h2", false);
    query->background = string_truncate(background, ">", "<div", false);
    query->background = deci_unicode_converter(query->background);
    query->background = html_unicode_converter(query->background);
    query->background = string_trim(query->background, true);
    query->background = replace_all(query->background, "<br />", "\n");
    query->background = html_remove_italic(query->background);
    query->background = html_remove_bold(query->background);


    // INFORMATION

    char *information = string_truncate(data, "<h2>Information</h2>", "<br />", false);

    query->volumes    = string_trim( string_truncate( information, "Volumes:</span>"  , "</div>", false), true);
    query->chapters   = string_trim( string_truncate( information, "Chapters:</span>" , "</div>", false), true);
    query->status     = string_trim( string_truncate( information, "Status:</span>"   , "</div>", false), true);
    query->published  = string_trim( string_truncate( information, "Published:</span>", "</div>", false), true);

    // Genres

    char *genres = string_truncate(information, "Genres:</span>", "</div>", false);
    if (genres != NULL) {
        pointer = genres;

        query->genres = string_create("");
        while ((pointer = strstr(pointer, "title=\"")) != NULL) {
            char *temp = string_truncate(pointer, "title=\"", "\"", false);
            query->genres = string_concat(true, 3, query->genres, temp, ", ");

            string_destroy(temp);

            pointer++;
        }

        if (query->genres[strlen(query->genres) - 2] == ',') {
            query->genres[strlen(query->genres) - 2] = '\0';
        }   
    }
    
    // Themes

    char *themes = string_truncate(information, "Themes:</span>", "</div>", false);
    if (themes != NULL) {
        pointer = themes;

        query->themes = string_create("");
        while ((pointer = strstr(pointer, "title=\"")) != NULL) {
            char *temp = string_truncate(pointer, "title=\"", "\"", false);
            query->themes = string_concat(true, 3, query->themes, temp, ", ");

            string_destroy(temp);

            pointer++;
        }

        if (query->themes[strlen(query->themes) - 2] == ',') {
            query->themes[strlen(query->themes) - 2] = '\0';
        }
    }

    // Demographic
    
    char *demographic = string_truncate(information, "Demographic:</span>", "</div>", false);
    if (demographic != NULL) {
        pointer = demographic;

        query->demographic = string_create("");
        while ((pointer = strstr(pointer, "title=\"")) != NULL) {
            char *temp = string_truncate(pointer, "title=\"", "\"", false);
            query->demographic = string_concat(true, 3, query->demographic, temp, ", ");

            string_destroy(temp);

            pointer++;
        }

        if (query->demographic[strlen(query->demographic) - 2] == ',') {
            query->demographic[strlen(query->demographic) - 2] = '\0';
        }
    }

    // Serialization

    char *serialization = string_truncate(information, "Serialization:</span>", "</div>", false);
    if (serialization != NULL) {
        pointer = serialization;

        query->serialization = string_create("");
        while ((pointer = strstr(pointer, "title=\"")) != NULL) {
            char *temp = string_truncate(pointer, "title=\"", "\"", false);
            query->serialization = string_concat(true, 3, query->serialization, temp, ", ");

            string_destroy(temp);

            pointer++;
        }

        if (query->serialization[strlen(query->serialization) - 2] == ',') {
            query->serialization[strlen(query->serialization) - 2] = '\0';
        }
    }

    // Authors
    
    char *authors = string_truncate(information, "Authors:</span>", "<br", false);
    if (authors != NULL) {
        pointer = authors;

        query->authors = string_create("");
        while ((pointer = strstr(pointer, "<a")) != NULL) {

            char *author = string_truncate(pointer, "\">", "</", false);

            char *role = NULL;

            pointer = strstr(pointer, "</a>");
            if (strstr(pointer, "<a") != NULL) {
                role = string_truncate(pointer, "</a>", "<a", false);
            } else {
                role = string_truncate(pointer, "</a>", "</div>", false);
            }

            query->authors = string_concat(true, 3, query->authors, author, role);

            strings_destroy(2, author, role);
        }

        if (query->authors[strlen(query->authors) - 2] == ',')
            query->authors[strlen(query->authors) - 2] = '\0';
    }

    query->authors = string_trim(query->authors, true);
    query->authors = html_unicode_converter(query->authors);
    query->authors = deci_unicode_converter(query->authors);


    // Statistics
    
    char *statistics = string_truncate(data, "<h2>Statistics</h2>", "<br", false);

    query->rank         = remove_all( string_trim( string_truncate(statistics, "Ranked:</span>"       , "<sup>" , false), true), "\"");
    query->popularity   = remove_all( string_trim( string_truncate(statistics, "Popularity:</span>"   , "</div>", false), true), "\"");
    query->members      = remove_all( string_trim( string_truncate(statistics, "Members:</span>"      , "</div>", false), true), "\"");
    query->favorites    = remove_all( string_trim( string_truncate(statistics, "Favorites:</span>"    , "</div>", false), true), "\"");

    strings_destroy(11,
                    data,
                    alt_titles,
                    synopsis,
                    background,
                    information,
                    genres,
                    themes,
                    demographic,
                    serialization,
                    authors,
                    statistics);

    return query;
}

#endif // MAL_H