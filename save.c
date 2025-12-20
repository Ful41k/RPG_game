#include <stdio.h>
#include "save.h"
#include "player.h"

// Сохранение данных в файл
void saveGame(Player* hero) {
    FILE* file = fopen("savegame.dat", "wb"); // wb = write binary
    if (file) {
        fwrite(hero, sizeof(Player), 1, file);
        fclose(file);
    } else {
        printf("Error: Could not create save file.\n");
    }
}

// Загрузка данных из файла
// Возвращает 1, если успешно, и 0, если файла нет
int loadGame(Player* hero) {
    FILE* file = fopen("savegame.dat", "rb"); // rb = read binary
    if (file) {
        fread(hero, sizeof(Player), 1, file);
        fclose(file);
        return 1; // Успех
    }
    return 0; // Файл не найден
}