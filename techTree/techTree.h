#ifndef TECH_TREES_H
#define TECH_TREES_H
#include <stdbool.h>


#define MAX_UPGRADES 13

typedef struct AppState AppState;

typedef struct {
    char name[50];
    int cost;
    int unlocked;
    int required1;
    int required2;
} Upgrade;

typedef struct {
    Upgrade upgrades[MAX_UPGRADES];
    int upgradePlatformLevel;
} TechTree;

void initTechTree(TechTree *tree);
bool unlockUpgrade(TechTree *tree, int index, int *gold);
void techTreeOverlay(AppState *app);
bool isUpgradeUnlocked(TechTree *tree, int index);
void upgradePlatform(AppState *app, TechTree *tree);
bool isSelectedPieceOnUpgradeTile(AppState *app);
bool isUpgradePlatformUnlocked(TechTree *tree);
#endif
