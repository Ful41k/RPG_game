/**
 * @file save.c
 * @brief Implementation of binary save/load operations and file management.
 * * This module manages the persistence of the @ref Player struct. It includes 
 * a dynamic slot system and a file-reorganization algorithm for deletions.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "save.h"
#include "player.h"

/**
 * @brief Generates a formatted date/time string for the save metadata.
 * @param buffer Character buffer to store the timestamp.
 */
void getCurrentTimeString(char* buffer) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buffer, 30, "%d-%m-%Y %H:%M:%S", t);
}

/**
 * @brief Serializes the Player struct to a binary file.
 * * Automatically iterates through "save_X.dat" names to find the 
 * first empty slot to prevent overwriting existing saves.
 * @param hero Pointer to the player data to be saved.
 */
void saveGame(Player* hero) {
    // 1. Get current time
    getCurrentTimeString(hero->saveDate);

    // 2. We need a unique filename for "unlimited" saves. 
    // We can use a simple counter by checking how many saves exist or use the timestamp.
    // For this example, let's append to a list.
    char filename[64];
    static int saveCount = 0; // In a real game, you'd load this count from a file
    
    // Let's find the next available slot number
    int slot = 1;
    while (1) {
        sprintf(filename, "save_%d.dat", slot);
        FILE* check = fopen(filename, "rb");
        if (!check) break; // Slot is free
        fclose(check);
        slot++;
    }

    FILE* file = fopen(filename, "wb");
    if (file) {
        fwrite(hero, sizeof(Player), 1, file);
        fclose(file);
        printf("\nGame saved in slot %d!\n", slot);
    }
}

/**
 * @brief UI and logic for managing save files.
 * * Features:
 * 1. **Scan:** Loops and reads all "save_X.dat" files to display stats.
 * 2. **Load:** Reads the binary data directly into the hero struct.
 * 3. **Delete:** Removes a file and **reorganizes** the remaining files 
 * (e.g., if you delete slot 2, slot 3 is renamed to 2) to maintain a continuous list.
 * * @param hero Pointer to the player structure to be populated.
 * @return 1 if a game was loaded, 0 if deleted or cancelled.
 */
int loadGameMenu(Player* hero) {
    int slot = 1;
    char filename[64];
    Player temp;

    printf("\nSaved Games:\n");
    
    // Loop through all possible save files to display them
    while (1) {
        sprintf(filename, "save_%d.dat", slot);
        FILE* file = fopen(filename, "rb");
        if (!file) break;

        fread(&temp, sizeof(Player), 1, file);
        printf("%d. %s, %d L. POINTS, %d COINS, %d ITEMS, %d COMPLETED MISSIONS\n",
               slot, temp.saveDate, temp.hp, temp.coins, temp.inventoryCount, temp.maxUnlockedMission);
        
        fclose(file);
        slot++;
    }

    if (slot == 1) {
        printf("No saves found.\n");
        return 0;
    }

    int choice;
    printf("Select a game [1-%d]: ", slot - 1);
    scanf("%d", &choice);

    printf("\nSelect one of the following actions:\n1. Load\n2. Delete\nInsert the action: ");
    int action;
    scanf("%d", &action);

    sprintf(filename, "save_%d.dat", choice);

    if (action == 1) { // LOAD
        FILE* file = fopen(filename, "rb");
        if (file) {
            fread(hero, sizeof(Player), 1, file);
            fclose(file);
            return 1;
        }
    } else if (action == 2) { // DELETE
        printf("Are you sure you want to permanently delete the save? [y/n]: ");
        char confirm;
        scanf(" %c", &confirm);
        if (confirm == 'y' || confirm == 'Y') {
            remove(filename); // Delete the chosen file (e.g., save_2.dat)

            // FIX: Shift all following files up by one
            int nextSlot = choice + 1;
            char oldName[64], newName[64];
            
            while (1) {
                sprintf(oldName, "save_%d.dat", nextSlot);
                sprintf(newName, "save_%d.dat", nextSlot - 1);
                
                // Try to rename the next file to the current (empty) slot
                if (rename(oldName, newName) != 0) {
                    // If rename fails, it means there are no more files to shift
                    break;
                }
                nextSlot++;
            }
            printf("Save deleted and files reorganized.\n");
        }
    }

    return 0;
}