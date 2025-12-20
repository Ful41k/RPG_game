#ifndef PLAYER_H
#define PLAYER_H

#define INVENTORY_SIZE 10

typedef struct {
    char name[32];
    int hp;
    int maxhp;
    int atk;
    int def;
    int coins;
    int xp;
    int level;
    int xpToNextLevel;
    
    // Rage System
    int rage;
    int maxRage;

    // Time System
    int isDay; // 1 = День, 0 = Ночь

    // Inventory & Items
    char inventory[INVENTORY_SIZE][32];
    int inventoryCount;
    int potionsSmall; 
    int potionsBig;
    
    // Equipment Flags
    int hasBaseSword;  // 0 - нет, 1 - есть
    int hasBaseArmor;  // 0 - нет, 1 - есть
    int upgradeLevelSword; // Уровень улучшения меча
    int upgradeLevelArmor; // Уровень улучшения брони

    // NPC & World Flags
    int knowsSadMan;       // 0 - нет, 1 - знаем имя (Garrick)
    int knowsWoman;        // 0 - нет, 1 - знаем имя (Elara)
    int blacksmithUnlocked;// 0 - закрыт, 1 - открыт
    int elaraQuestStatus;  // 0 - нет, 1 - взят, 2 - предмет найден, 3 - сдан
    int introBarnabyDone;  // 0 - нет, 1 - диалог про Мастера Эдварда прошел
    
    // Status Effects
    int hangover;          // 1 - штраф к атаке
    int aleDrankCount;     // Счетчик выпитого эля

    int maxUnlockedMission;
    int hasHeroSword; // Для совместимости

} Player;

Player createNewHero();
void addXp(Player* hero, int amount);
void addItemToInventory(Player* hero, const char* itemName);

#endif