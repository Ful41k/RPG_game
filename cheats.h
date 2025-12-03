#ifndef CHEATS_H
#define CHEATS_H

#define KONAMI_LENGTH 10

#include "save.h"

void showCheatsMenu(Save *saves, int saveCount);

/* Состояние системы читов / Konami code */
typedef struct {
    int cheatsUnlocked;  // 0 = читы скрыты, 1 = читы открыты
    int konamiIndex;     // сколько символов из кода уже совпало
} CheatState;

/* Инициализация состояния читов */
void initCheatState(CheatState *state);

/* Обработка введённого символа для Konami code */
void processKonamiInput(CheatState *state, int ch);

/* Меню читов: выбор сохранения, изменение HP/coins, разблокировка финала */
void showCheatsMenu(Save *saves, int saveCount);

#endif /* CHEATS_H вот так крч */