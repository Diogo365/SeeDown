#include "../mal.h"


void search_init() {

    wclear(stdscr);
    wrefresh(stdscr);

    box(stdscr, 0, 0);
    mvprintw(0, 2, "SeeDown");

    int search_row = 2;
    int search_col = 52;

    int search_y = stdscr->_maxy / 3;
    int search_x = stdscr->_maxx / 2 - 25;

    wrefresh(stdscr);

    bool exit = false;
    int search_engine = 0; // 0 = MangaSee, 1 = MAL

    char search_string[50] = {0};

    while (!exit) {

        rectangle(search_y, search_x, search_y + search_row, search_x + search_col);

        if (search_engine == 0) {
            mvwprintw(stdscr, search_y, search_x + 1, " Search MangaSee ");
        } else {
            mvwprintw(stdscr, search_y, search_x + 1, " Search MyAnimeList ");
        }

        mvwprintw(stdscr, search_y + 1, search_x + 1, "%50s", " ");
        mvwprintw(stdscr, search_y + 1, search_x + 1, "%s", search_string);

        int key = getch();

        switch (key) {

        case 27: // Escape key
            exit = true;
            break;


        case 9: // Tab key
            search_engine = (search_engine) ? 0 : 1;
            break;


        case 10: // Enter key
            if (strlen(search_string) > 0) {
                if (search_engine) {
                    mvwprintw(stdscr, search_y + 2, search_x + 1, "%s", " Looking up on MyAnimeList... ");
                    wrefresh(stdscr);
                    ARRAY results = mal_query(search_string, 5);

                    if (results->size > 0) {
                        for (int i = 0; i < results->size; i++) {
                            QUERY entry = results->data[i];
                            mvwprintw(stdscr, search_y + search_row + 1 + i, search_x + 1 , "%s", entry->title);
                        }

                        wrefresh(stdscr);
                    }
                    
                    array_destroy_struct(results, (void *) query_destroy);
                }

            }
            break;



        // Window resize event
        case KEY_RESIZE:
            wclear(stdscr);

            box(stdscr, 0, 0);
            mvprintw(0, 2, "SeeDown");

            search_y = stdscr->_maxy / 3;
            search_x = stdscr->_maxx / 2 - 25;

            break;



        default:
            if (key == KEY_BACKSPACE) { // Backspace
                if (strlen(search_string) > 0) search_string[strlen(search_string) - 1] = '\0';
            }
            else if(isalpha(key) || isdigit(key) || key == ' ' || key == '-' || key == '_' || key == '.' || key == '/' || key == '\\' || key == ':' || key == ';' || key == ',' || key == '?' || key == '!' || key == '@' || key == '#' || key == '$' || key == '%' || key == '^' || key == '&' || key == '*' || key == '(' || key == ')' || key == '+' || key == '=' || key == '{' || key == '}' || key == '[' || key == ']' || key == '|' || key == '<' || key == '>' || key == '`' || key == '~' || key == '\'' || key == '\"') {
                if (strlen(search_string) < 50) search_string[strlen(search_string)] = key;
            }
            break;
        }
    }
}