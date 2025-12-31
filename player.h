#ifndef PLAYER_H
#define PLAYER_H

#define INVENTORY_SIZE 10

typedef struct {
    char name[32];
    char saveDate[30]; // Added to store: "11-10-2025 15:03:29"
    int hp;
    int maxhp;
    int atk;
    int def;
    int coins;
    int xpToNextLevel;
    int rage;
    int maxRage;
    int isDay; 
    char inventory[INVENTORY_SIZE][32];
    int inventoryCount;
    int potionsSmall; 
    int potionsBig;
    int hasBaseSword;  
    int hasBaseArmor;  
    int hasCastleKey;
    int upgradeLevelSword; 
    int upgradeLevelArmor; 
    int knowsSadMan;       
    int knowsWoman;        
    int blacksmithUnlocked;
    int elaraQuestStatus;  
    int introBarnabyDone;  
    int hangover;          
    int aleDrankCount;     
    int maxUnlockedMission;
    int hasHeroSword; 
} Player;

// Function prototypes
Player createNewHero();
void addXp(Player* hero, int amount);
void addItemToInventory(Player* hero, const char* itemName);

#endif