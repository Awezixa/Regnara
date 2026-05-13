#ifndef TECH_TREES_H
#define TECH_TREES_H
#include <stdbool.h>


#define MAX_UPGRADES 20

typedef struct AppState AppState;

typedef struct {
    char name[50];
    int cost;
    int unlocked;
    int required;
} Upgrade;

typedef struct {
    Upgrade upgrades[MAX_UPGRADES];
} TechTree;

void initTechTree(TechTree *tree);
void unlockUpgrade(TechTree *tree, int index, int *gold);
void printTechTree(AppState *app);
bool isUpgradeUnlocked(TechTree *tree, int index);
#endif
