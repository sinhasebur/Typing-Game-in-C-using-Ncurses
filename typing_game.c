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
            attron(COLOR_PAIR(3));
            mvprintw(10, 0, "Typed: ");
            attroff(COLOR_PAIR(3));
            mvprintw(10, 6, "%s ",typed_word);
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
        attron(COLOR_PAIR(2));
        mvprintw(15, 0, "\n\n--------------Press ESC to exit or press r to play again.--------------");
        attroff(COLOR_PAIR(2));
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
            refresh();
        }
    }
    endwin();
    return 0;
}




void take_type() {
    clear();
    attron(COLOR_PAIR(3));
    printw("_______________________________________Welcome______________________________________\n\n");
    attroff(COLOR_PAIR(3));
    printw("You have 60 secs to play. In Easy mode, you will get word of characters ranging from 1 to 4,\nIn Medium mode, you will get words with characters ranging from 5 to 7,\nIn Hard Mode, you will get words where each are more than 7 characters long. A buffer will show the next 5 words.\nBest of luck! Enjoy!");
    printw("\nSelect\n");
    attron(COLOR_PAIR(1));
    printw("1 for Easy Mode\n");
    attroff(COLOR_PAIR(1));
    attron(COLOR_PAIR(5));
    printw("2 for Medium Mode\n");
    attroff(COLOR_PAIR(5));
    attron(COLOR_PAIR(6));
    printw("3 for Hard Mode\n");
    attroff(COLOR_PAIR(6));
    attron(COLOR_PAIR(3));
    printw("\n\nPlease Select: ");
    attroff(COLOR_PAIR(3));
    refresh();
    scanw("%d", &Gametype);
    if(Gametype==27){
        exit(1);
    }
    printw("%d\n", Gametype);
    refresh();
    while (!(Gametype == 1 || Gametype == 2 || Gametype == 3)) {
        clear();
        attron(COLOR_PAIR(2));
        printw("Invalid input. Please pick - \n");
        attroff(COLOR_PAIR(2));

        attron(COLOR_PAIR(1));
        printw("1 for Easy Mode \n");
        attroff(COLOR_PAIR(1));

        attron(COLOR_PAIR(5));
        printw("2 for Medium Mode \n");
        attroff(COLOR_PAIR(5));

        attron(COLOR_PAIR(6));
        printw("3 for Hard Mode \n");
        attroff(COLOR_PAIR(6));
        attron(COLOR_PAIR(4));
        printw("\n\nPlease Select: ");
        attroff(COLOR_PAIR(4));

        refresh();

        scanw("%d", &Gametype);
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

    if (Gametype == 1) {
        attron(COLOR_PAIR(1));
        mvprintw(0,0, "Easy Mode");
        attroff(COLOR_PAIR(1));
    } else if (Gametype == 2) {
        attron(COLOR_PAIR(5));
        mvprintw(0,0, "Medium Mode");
        attroff(COLOR_PAIR(5));
    } else {
        attron(COLOR_PAIR(6));
        mvprintw(0,0, "Hard Mode");
        attroff(COLOR_PAIR(6));
    }
    int j = 0;
    int row = 2;
    int max_width = COLS - 1;

    for (int n = 0; n < 5 && current_word + n < word_number; n++) {
        char *word = Word_Array[current_word + n];
        int word_len = strlen(word);

        if (j + word_len > max_width) {
            row += 2;
            j = 0;
        }

        for (int i = 0; i < word_len; i++) {
            if (n == 0 && i < strlen(typed_word)) {
                if (typed_word[i] == word[i]) {
                    attron(COLOR_PAIR(1));
                } else {
                    attron(COLOR_PAIR(2));
                }
            }
            mvprintw(row, j++, "%c", word[i]);
            attroff(COLOR_PAIR(1));
            attroff(COLOR_PAIR(2));
        }
        mvprintw(row, j++, " ");
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

    clear();

    attron(COLOR_PAIR(4));
    mvprintw(1, 5,  "------------------------------------");
    mvprintw(2, 5,  "|           RESULTS                 |");
    mvprintw(3, 5,  "------------------------------------");
    attroff(COLOR_PAIR(4));

    if (accuracy < 70) {
        attron(COLOR_PAIR(2));
        mvprintw(5, 7,  "Accuracy: %.3f%%", accuracy);
        attroff(COLOR_PAIR(2));
    } else if (accuracy >= 70 && accuracy < 80) {
        attron(COLOR_PAIR(5));
        mvprintw(5, 7,  "Accuracy: %.3f%%", accuracy);
        attroff(COLOR_PAIR(5));
    } else {
        attron(COLOR_PAIR(1));
        mvprintw(5, 7,  "Accuracy: %.3f%%", accuracy);
        attroff(COLOR_PAIR(1));
    }

    if (wpm < 30) {
        attron(COLOR_PAIR(2));
        mvprintw(6, 7,  "Words Per Minute: %.3f", wpm);
        attroff(COLOR_PAIR(2));
    } else if (wpm >= 30 && wpm < 50) {
        attron(COLOR_PAIR(5));
        mvprintw(6, 7,  "Words Per Minute: %.3f", wpm);
        attroff(COLOR_PAIR(5));
    } else {
        attron(COLOR_PAIR(1));
        mvprintw(6, 7,  "Words Per Minute: %.3f", wpm);
        attroff(COLOR_PAIR(1));
    }

    attron(COLOR_PAIR(6));
    mvprintw(7, 7,  "CPS: %.3f", cps);
    attroff(COLOR_PAIR(6));

    attron(COLOR_PAIR(5));
    mvprintw(9, 5,  "------------------------------------");
    mvprintw(10, 5, "|       OTHER STATISTICS           |");
    mvprintw(11, 5, "------------------------------------");
    attroff(COLOR_PAIR(5));

    mvprintw(13, 7, "Total Keystrokes: %d | Incorrect: %d | Correct: %d", total_characters, total_characters - correct_characters, correct_characters);
    mvprintw(14, 7, "Total Words: %d | Incorrect: %d | Correct: %d", total_words, total_words - correct_words, correct_words);

    refresh();
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
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    init_pair(4, COLOR_CYAN, COLOR_BLACK);
    init_pair(5, COLOR_YELLOW, COLOR_BLACK);
    init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
}
