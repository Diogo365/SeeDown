#include <curl/curl.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DEBUG_MODE 1

#include "./lib/list.h"
#include "./lib/array.h"
#include "./lib/utilities.h"

#include "./lib/simple_curl.h"
#include "./lib/manga.h"


int main(int argc, char *argv[]) {
    argv = argv;
    argc = argc;

    if (DEBUG_MODE) printf("\n");

    scan_directory();

    return 0;
}