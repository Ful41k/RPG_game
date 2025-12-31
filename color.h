/**
 * @file color.h
 * @brief ANSI escape codes for terminal text coloring.
 */

#ifndef COLOR_H
#define COLOR_H

#define RED     "\x1b[31m"   /**< Red text */
#define GREEN   "\x1b[32m"   /**< Green text */
#define YELLOW  "\x1b[33m"   /**< Yellow text */
#define BLUE    "\x1b[34m"   /**< Blue text */
#define MAGENTA "\x1b[35m"   /**< Magenta text */
#define CYAN    "\x1b[36m"   /**< Cyan text */
#define RESET   "\x1b[0m"    /**< Resets color to terminal default */
#define WHITE   "\033[1;37m" /**< Bold White text */

#endif