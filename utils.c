#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

// --- WINDOWS ---
#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>

    void delay_ms(int ms) {
        Sleep(ms);
    }

    void clear_screen() {
        system("cls");
    }

    // Локальные обертки для Windows
    static int kbhit_check() { return _kbhit(); }
    static int getch_read() { return _getch(); }

// --- LINUX / MACOS ---
#else
    #include <unistd.h>
    #include <termios.h>
    #include <fcntl.h>

    void delay_ms(int ms) {
        usleep(ms * 1000); // usleep принимает микросекунды
    }

    void clear_screen() {
        system("clear");
    }

    // Реализация kbhit() для Linux
    static int kbhit_check() {
        struct termios oldt, newt;
        int ch;
        int oldf;

        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

        ch = getchar();

        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        fcntl(STDIN_FILENO, F_SETFL, oldf);

        if (ch != EOF) {
            ungetc(ch, stdin);
            return 1;
        }
        return 0;
    }

    // Реализация getch() для Linux
    static int getch_read() {
        struct termios oldt, newt;
        int ch;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return ch;
    }
#endif

// --- ОБЩАЯ ЛОГИКА ---

void print_typewriter(const char* text, int speed) {
    int skip = 0;
    while (*text) {
        if (!skip && kbhit_check()) {
            getch_read();
            skip = 1;
        }

        if (*text == '\x1b') { // Пропуск цветовых кодов
            while (*text && *text != 'm') {
                putchar(*text++);
            }
            if (*text) putchar(*text++);
        } else {
            putchar(*text++);
            if (!skip) {
                fflush(stdout);
                delay_ms(speed);
            }
        }
    }
    putchar('\n');
    fflush(stdout);
}

void print_prompt(const char* text, int speed) {
    int skip = 0;
    while (*text) {
        if (!skip && kbhit_check()) {
            getch_read();
            skip = 1;
        }

        if (*text == '\x1b') {
            while (*text && *text != 'm') {
                putchar(*text++);
            }
            if (*text) putchar(*text++);
        } else {
            putchar(*text++);
            if (!skip) {
                fflush(stdout);
                delay_ms(speed);
            }
        }
    }
    fflush(stdout);
}