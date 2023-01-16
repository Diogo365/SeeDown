#include <curl/curl.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <wchar.h>
#include <locale.h>

#define DEBUG_MODE 1

#include "./lib/list.h"
#include "./lib/array.h"
#include "./lib/utilities.h"

#include "./lib/simple_curl.h"
#include "./lib/manga.h"


int main(int argc, char *argv[]) {
    argv = argv;
    argc = argc;
    setlocale(LC_CTYPE, "");

    if (DEBUG_MODE) printf("\n");

    scan_directory();

    scan_rss((MANGA) mangas->data[66]);

    array_destroy(all_genres);
    array_destroy_struct(mangas, (void *) manga_destroy);

    return 0;
}