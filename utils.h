#ifndef UTILS_H
#define UTILS_H

void print_typewriter(const char* text, int speed);
void print_prompt(const char* text, int speed);

// Новые кроссплатформенные функции
void delay_ms(int ms);
void clear_screen();

#endif