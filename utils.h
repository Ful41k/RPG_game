/**
 * @file utils.h
 * @brief Function prototypes for cross-platform utilities.
 */

#ifndef UTILS_H
#define UTILS_H

/** @brief Character-by-character text effect. */
void print_typewriter(const char* text, int speed);

/** @brief Animated prompt for input. */
void print_prompt(const char* text, int speed);

/** @brief Cross-platform millisecond sleep. */
void delay_ms(int ms);

/** @brief Cross-platform terminal clearing. */
void clear_screen();

#endif