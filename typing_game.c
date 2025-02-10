#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define Hard 3
#define Medium 2
#define Easy 1
#define word_number 150

char Main_Array[600][15];
char Word_Array[word_number][15];
int Gametime = 60;
int Gametype, size_upper, size_lower, current_word = 0;
int correct_words = 0, total_words = 0, correct_characters = 0, total_characters = 0;

void take_type();
void create_word_array();
void print_words(char *a);
void check(char *a);
void store_array();
void calculate();
void start_ncurses();

int main() {

    start_ncurses();

    store_array();

    int exit_game = 0;

    while (!exit_game) {

        current_word = 0;
        correct_words = 0;
        total_words = 0;
        correct_characters = 0;
        total_characters = 0;
        clear();

        freopen("/dev/tty", "r", stdin);
        take_type();
        noecho();
        nodelay(stdscr, TRUE);
        timeout(100);

        create_word_array();

        time_t start = time(NULL);
        int position = 0;
        char typed_word[15] = {0};
        int ch;

        while (time(NULL) - start < Gametime) {
            clear();
            print_words(typed_word);
            mvprintw(10, 0, "Typed: %s", typed_word);
            refresh();

            ch = getch();

            if(ch==KEY_UP||ch==KEY_DOWN||ch==KEY_LEFT||ch==KEY_RIGHT||(ch >= KEY_F(1)&&ch<= KEY_F(12))){
                continue;
            }
            else if (ch == 27) {
                exit_game = 1;
                break;
            } else if (ch == ' ' || ch == '\n') {
                typed_word[position] = '\0';
                check(typed_word);
                memset(typed_word, 0, sizeof(typed_word));
                position = 0;
                current_word++;
            } else if (ch == '\b' || ch == 127|| ch == KEY_BACKSPACE) {
                if (position > 0) {
                    position--;
                    typed_word[position] = '\0';
                    total_characters++;
                }
            } else if(((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) ){
                typed_word[position++] = ch;
                total_characters++;
            } else {
                continue;
            }
        }
        if (exit_game)
            break;
        clear();

        calculate();

        mvprintw(LINES - 3, 0, "\n\n--------------Press ESC to exit or press r to play again.--------------");
        refresh();
        nodelay(stdscr, FALSE);
        int choice;
        while (1) {
            choice = getch();
            if (choice == 27) {
                exit_game = 1;
                break;
            } else if (choice == 'r' || choice == 'R') {
                break;
            }
        }
    }
    endwin();
    return 0;
}

void take_type() {
    clear();
    printw("_______________________________________Welcome______________________________________\nYou have 60 secs to play. In Easy mode, you will get word of characters ranging from 1 to 4,\nIn Medium mode, you will get words with characters ranging from 5 to 7,\nIn Hard Mode, you will get words where each are more than 7 characters long. A buffer will show the next 5 words.\nBest of luck! Enjoy!");
    printw("\nSelect  1 for Easy, 2 for Medium, or 3 for Hard.\n\nPlease Select: ");
    refresh();
    scanw("%d", &Gametype);
    if(Gametype==27){
        exit(1);
    }
    printw("%d\n", Gametype);
    refresh();
    while (!(Gametype == 1 || Gametype == 2 || Gametype == 3)) {
        printw("Invalid input. Please select 1, 2, or 3 (Easy,Medium and Hard respectively).\nPlease Select:");
        scanw("%d", &Gametype);
        if(Gametype==27){
            exit(1);
        }
        printw("%d\n", Gametype);
        refresh();
    }
    printw("%d\n", Gametype);
    refresh();
    if (Gametype == Easy) {
        size_lower = 1;
        size_upper = 4;
    } else if (Gametype == Medium) {
        size_lower = 5;
        size_upper = 7;
    } else if (Gametype == Hard) {
        size_lower = 8;
        size_upper = 14;
    }
}

void create_word_array() {
    char temp[15];
    int i = 0, j = 0;
    while (i < word_number && j < 600) {
        strcpy(temp, Main_Array[j]);
        j++;
        if (strlen(temp) <= size_upper && strlen(temp) >= size_lower) {
            strcpy(Word_Array[i++], temp);
        }
    }
    srand(time(NULL));
    for (int j = i - 1; j > 0; j--) {
        int k = rand() % (j + 1);
        char swap_temp[15];
        strcpy(swap_temp, Word_Array[j]);
        strcpy(Word_Array[j], Word_Array[k]);
        strcpy(Word_Array[k], swap_temp);
    }
}

void check(char *a) {
    if (strcmp(a, Word_Array[current_word]) == 0) {
        correct_words++;
        correct_characters += strlen(a);
    }
    total_words++;
}

void print_words(char *typed_word) {
    int j = 0;
    for (int i = 0; i < strlen(Word_Array[current_word]); i++) {
        if (i >= strlen(typed_word)) {
            attroff(COLOR_PAIR(1));
            attroff(COLOR_PAIR(2));
        } else if (typed_word[i] == Word_Array[current_word][i]) {
            attron(COLOR_PAIR(1));
        } else {
            attron(COLOR_PAIR(2));
        }
        mvprintw(0, j, "%c", Word_Array[current_word][i]);
        attroff(COLOR_PAIR(1));
        attroff(COLOR_PAIR(2));
        j++;
    }
    for (int i = 1; i < 5 && current_word + i < word_number; i++) {
        mvprintw(i, 0, "%s", Word_Array[current_word + i]);
    }
    refresh();
}

void calculate() {
    float accuracy;
    if (total_words != 0) {
        accuracy = ((correct_words / (float)total_words) * 100);
    }
    float wpm = (correct_words / (Gametime / 60.0));
    float cps = (correct_characters / Gametime);
    printw("\n_______________RESULTS_______________\n\n           Accuracy: %.3f%%\n           WPM: %.3f\n           CPS: %.3f\n", accuracy, wpm, cps);
    printw("\n\n___________Other Statistics___________\n\n");
    printw("Total Keystrokes: %d | Incorrect %d | Correct %d\n", total_characters,total_characters-correct_characters, correct_characters);
    printw("Total Words:%d | Incorrect %d | Correct %d\n", total_words,total_words-correct_words, correct_words);
}

void store_array(){
    int i = 0;
    for(i = 0; i < 600; i++){
        scanf("%s", Main_Array[i]);
    }
}

void start_ncurses(){
    initscr();
    keypad(stdscr, TRUE);
    start_color();
    cbreak();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
}
