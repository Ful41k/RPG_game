/**
 * @file utils.c
 * @brief Cross-platform terminal utilities for text rendering and input.
 * * This module provides abstraction for Windows and Unix-based systems (Linux/macOS).
 * It handles the "Typewriter" effect, screen clearing, and non-blocking input
 * detection used to skip text animations.
 */

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

    /**
     * @brief Detects if a keyboard key has been pressed (Non-blocking).
     * * Uses termios to disable canonical mode and echo, then sets fcntl 
     * to non-blocking to check the stdin buffer.
     * @return 1 if a key is waiting in the buffer, 0 otherwise.
     */
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

    /**
     * @brief Reads a single character from the keyboard without echoing to screen.
     * @return The integer value of the character read.
     */
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

/**
 * @brief Prints text character-by-character with skip functionality.
 * * Logic:
 * 1. Iterates through the string.
 * 2. Checks @ref kbhit_check for user input; if found, switches to "skip mode."
 * 3. Detects ANSI escape sequences (starting with '\x1b') to print them 
 * instantly, preventing broken color codes during the animation.
 * * @param text The string to print (supports ANSI colors).
 * @param speed Delay in milliseconds between characters.
 */
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

/**
 * @brief Prints a prompt without a trailing newline for user input.
 * * Similar to @ref print_typewriter but does not add a '\n' at the end,
 * making it ideal for "Choice: " or "Enter name: " prompts.
 * * @param text The prompt text.
 * @param speed Animation speed.
 */
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