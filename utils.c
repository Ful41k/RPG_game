#include <time.h>

#include "utils.h"

// function for text that appear some amount of time
void delay_ms(int milliseconds) {
    long pause = milliseconds * (CLOCKS_PER_SEC / 1000);
    clock_t start = clock();
    while (clock() - start < pause) {}
}
