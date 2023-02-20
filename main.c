#include <curl/curl.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/stat.h>

#include <wchar.h>
#include <locale.h>

#define DEBUG_MODE 1

#include "./lib/list.h"
#include "./lib/array.h"
#include "./lib/utilities.h"

#include "./lib/simple_curl.h"
#include "./lib/manga.h"
#include "./lib/mal.h"
#include "./lib/gui.h"


int main(int argc, char *argv[]) {
    argv = argv;
    argc = argc;
    setlocale(LC_CTYPE, "");

    if (DEBUG_MODE) printf("\n");

    // Create base directory to save mangas
    if (mkdir(SAVE_PATH, 0777) == -1 && errno != EEXIST) {
        printf("Error creating directory 'mangas'");
        return 1;
    }

    init_gui();

    /*
    // cSpell:ignore kaguya sama
    char *string = malloc(100 * sizeof(char));
    printf("Enter manga name: ");
    fgets(string, 100, stdin);
    string[strlen(string) - 1] = '\0';

    ARRAY results = mal_query(string);
    mal_lookup((QUERY) results->data[0]);

    array_destroy_struct(results, (void *) query_destroy);

    string_destroy(string);
    */


    /*
    string = string_create("Love is war");
    results = mal_query(string);
    query_print((QUERY) results->data[0]);
    */

    /*
    scan_directory();

    scan_rss((MANGA) mangas->data[66]);
    scan_manga((MANGA) mangas->data[66]);
    prepare_chapter((MANGA) mangas->data[66]);
    download_chapter((MANGA) mangas->data[66]);

    scan_rss((MANGA) mangas->data[1]);
    scan_manga((MANGA) mangas->data[1]);
    prepare_chapter((MANGA) mangas->data[1]);
    download_chapter((MANGA) mangas->data[1]);
    
    array_destroy(all_genres);
    array_destroy_struct(mangas, (void *) manga_destroy);
    */

    return 0;
}