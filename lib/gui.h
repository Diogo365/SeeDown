#include <ncurses.h>

void rectangle(int y1, int x1, int y2, int x2) {
    mvhline(y1, x1, 0, x2-x1);
    mvhline(y2, x1, 0, x2-x1);
    mvvline(y1, x1, 0, y2-y1);
    mvvline(y1, x2, 0, y2-y1);
    mvaddch(y1, x1, ACS_ULCORNER);
    mvaddch(y2, x1, ACS_LLCORNER);
    mvaddch(y1, x2, ACS_URCORNER);
    mvaddch(y2, x2, ACS_LRCORNER);
}

void unimplemented();
void exit_gui();
void init_gui();

typedef struct {
    char *name;
    int pos_x;
    int pos_y;
    int length;

    void (*callback)();
} MENU_ITEM;

#include "./screens/search.h"

char *logo[] = {
    "  ___          ___                  ",
    " / __| ___ ___|   \\ _____ __ ___ _  ",
    " \\__ \\/ -_) -_) |) / _ \\ V  V / ' \\ ",
    " |___/\\___\\___|___/\\___/\\_/\\_/|_||_|",
    "                                    " 
};

MENU_ITEM menu_items[] = {
    {"Search", 0, 0, 0, search_init},
    {"Library", 0, 0, 0, unimplemented},
    {"History", 0, 0, 0, unimplemented},
    {"Settings", 0, 0, 0, unimplemented},
    {"Exit", 0, 0, 0, exit_gui}
};

int process_mouse_click(MEVENT event) {
    int mouse_x = event.x;
    int mouse_y = event.y;

    for (size_t i = 0; i < sizeof(menu_items) / sizeof(menu_items[0]); i++) {
        MENU_ITEM item = menu_items[i];

        if (mouse_x >= item.pos_x && mouse_x < item.pos_x + item.length) {
            if (mouse_y == item.pos_y) {
                return i;
            }
        }
    }

    return -1;
}

void unimplemented() {
    mvprintw(stdscr->_maxy - 1, 2, "This feature is not implemented yet");
}

void exit_gui() {

    if (endwin() == ERR)
        printf("Error exiting ncurses\n");
    else 
        printf("Exited ncurses\n");

    printf("\033[?1003l\n");

    exit(0);
}

void init_gui() {
    // setenv("TERM", "xterm-1006", 1);

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);


    // Set up mouse event throwing
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
    mouseinterval(0);
    printf("\033[?1003h\n");

    box(stdscr, 0, 0);
    mvprintw(0, 2, "SeeDown");

    int menu_size = sizeof(menu_items) / sizeof(menu_items[0]);
    int menu_selected = 0;

    int menu_x = stdscr->_maxx / 2 - 5;
    int menu_y = stdscr->_maxy / 2 - menu_size / 2;

    bool exit = false;

    while (!exit) {

        mvprintw(0, stdscr->_maxx - 4, " %2d ", menu_selected);

        // Draw logo
        for (int i = 0; i < 5; i++) {
            mvprintw(stdscr->_maxy / 4 - 2 + i, stdscr->_maxx / 2 - 18, "%s", logo[i]);
        }

        // Draw menu
        for (int i = 0; i < menu_size; i++) {
            MENU_ITEM *item = &menu_items[i];
            item->pos_x = menu_x;
            item->pos_y = menu_y + i;
            item->length = strlen(item->name);

            if (i == menu_selected) {
                attron(A_REVERSE);
                mvprintw(item->pos_y, item->pos_x, "%s", item->name);
                attroff(A_REVERSE);
            } else {
                mvprintw(item->pos_y, item->pos_x, "%s", item->name);
            }
        }

        int key = getch();
        MEVENT event;

        switch (key) {

        // Arrow keys
        case KEY_UP:
            if ( menu_selected > 0 ) menu_selected--;
            break;
        case KEY_DOWN:
            if ( menu_selected < menu_size - 1) menu_selected++;
            break;



        // Enter key
        case 10:
            if (menu_selected != -1) {
                menu_items[menu_selected].callback();

                wclear(stdscr);

                box(stdscr, 0, 0);
                mvprintw(0, 2, "SeeDown");

                wrefresh(stdscr);
            }

            break;



        // Mouse events
        case KEY_MOUSE:
            if(getmouse(&event) == OK) {
                mvprintw(stdscr->_maxy, 2, " Mouse event: %d, %d, %d ", event.x, event.y, event.bstate);

                if(event.bstate & BUTTON1_PRESSED) { // Left-click
                    if (menu_selected != -1) {
                        menu_items[menu_selected].callback();

                        wclear(stdscr);

                        box(stdscr, 0, 0);
                        mvprintw(0, 2, "SeeDown");

                        wrefresh(stdscr);
                    }
                }
                else if (event.bstate & BUTTON2_PRESSED) { // Middle-click
                    // Do something
                }
                else if(event.bstate & BUTTON3_CLICKED) { // Right-click
                    // Do something
                } else { // Only mouse movement
                    menu_selected = process_mouse_click(event);
                }
            }
            break;



        // Window resize event
        case KEY_RESIZE:
            wclear(stdscr);

            box(stdscr, 0, 0);
            mvprintw(0, 2, "SeeDown");

            menu_x = stdscr->_maxx / 2 - 5;
            menu_y = stdscr->_maxy / 2 - menu_size / 2;

            break;



        default:
            break;
        }
    }

    endwin();
}