#ifndef CHEATS_H
#define CHEATS_H

#define KONAMI_LENGTH 11

typedef struct {
    int lifePoints;           // HP героя
    int coins;                // монеты
    int itemsCount;           // сколько предметов (если нужно)
    int completedMissions;    // завершённые миссии
    int finalMissionUnlocked; // 0 = нет, 1 = финальная миссия открыта
} Save;

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
