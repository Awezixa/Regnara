#include "techTree.h"
#include <stdio.h>
#include <string.h>
#include "../game.h"

/*

Piece Upgrades


Building upgrades
- increased gold income

Generic/ random upgrades
- 25% to gain half captured piece value

*/


void initTechTree(TechTree *tree)
{
    // Index 0: Knight (Needs Pawn - Start)
    strcpy(tree->upgrades[0].name, "Knight");
    tree->upgrades[0].cost = 5;
    tree->upgrades[0].required1 = -1; 
    tree->upgrades[0].required2 = -1;
    strcpy(tree->upgrades[0].description, "An agile fighter capable of bypassing obstacles easily.");
    strcpy(tree->upgrades[0].unlocksText, "- Rook Upgrade Path\n- Bishop Upgrade Path");
    strcpy(tree->upgrades[0].givesText, "Unlocks Knight training option");

    // Index 1: Envoy (Needs Pawn - Start)
    strcpy(tree->upgrades[1].name, "Envoy");
    tree->upgrades[1].cost = 5;
    tree->upgrades[1].required1 = -1;
    tree->upgrades[1].required2 = -1;
    strcpy(tree->upgrades[1].description, "The King's right hand man in conquering the land");
    strcpy(tree->upgrades[1].unlocksText, "- Lancer Upgrade\n- Mage Upgrade\n- Catapult Upgrade\n- Queen unlock");
    strcpy(tree->upgrades[1].givesText, "Unlocks Envoy upgrad option and can claim towns in 3 turns");

    // Index 2: Extra Money (Needs Pawn - Start)
    strcpy(tree->upgrades[2].name, "Town Income");
    tree->upgrades[2].cost = 10;
    tree->upgrades[2].required1 = -1;
    tree->upgrades[2].required2 = -1;
    strcpy(tree->upgrades[2].description, "Increase gold amount gained based on towns controlled");
    strcpy(tree->upgrades[2].unlocksText, "- Unit increase Cap");
    strcpy(tree->upgrades[2].givesText, "+5 bonus Gold every single turn round");

    // Index 3: UPG Platform I (Needs Pawn - Start)
    strcpy(tree->upgrades[3].name, "UPG PLAT I");
    tree->upgrades[3].cost = 15;
    tree->upgrades[3].required1 = -1;
    tree->upgrades[3].required2 = -1;
    strcpy(tree->upgrades[3].description, "A platform of magical powers that can improve your pieces");
    strcpy(tree->upgrades[3].unlocksText, "- Upgrade platform II effects\n- UPG PLAT III effects");
    strcpy(tree->upgrades[3].givesText, "Gives a random chance of upgrading a piece to a better piece");

    // Index 4: Rook (Needs Knight)
    strcpy(tree->upgrades[4].name, "Rook");
    tree->upgrades[4].cost = 8;
    tree->upgrades[4].required1 = 0; // Knight
    tree->upgrades[4].required2 = -1;
    strcpy(tree->upgrades[4].description, "A strong defensive piece to help fortify the land");
    strcpy(tree->upgrades[4].unlocksText, "- Rook Upgrade Path\n- Queen unlock");
    strcpy(tree->upgrades[4].givesText, "Unlocks Rook training option");

    // Index 5: Bishop (Needs Knight)
    strcpy(tree->upgrades[5].name, "Bishop");
    tree->upgrades[5].cost = 8;
    tree->upgrades[5].required1 = 0; // Knight
    tree->upgrades[5].required2 = -1;
    strcpy(tree->upgrades[5].description, "A piece with plenty wisdom");
    strcpy(tree->upgrades[5].unlocksText, "- Bishop Upgrade Path\n- Rook Upgrade Path\n- Queen unlock");
    strcpy(tree->upgrades[5].givesText, "Unlocks Rook training option");

    // Index 6: Lancer (Needs Envoy + Knight)
    strcpy(tree->upgrades[6].name, "Lancer");
    tree->upgrades[6].cost = 12;
    tree->upgrades[6].required1 = 1; // Envoy
    tree->upgrades[6].required2 = 0; // Knight
    strcpy(tree->upgrades[6].description, "The knights better version, cabable of dealing with multiple units at a time");
    strcpy(tree->upgrades[6].unlocksText, "- Rook Upgrade Path\n- Bishop Upgrade Path");
    strcpy(tree->upgrades[6].givesText, "Unlocks Lancer Upgrade to Knight and Lancer training Option");

    // Index 7: Mage (Needs Lancer + Bishop)
    strcpy(tree->upgrades[7].name, "Mage");
    tree->upgrades[7].cost = 15;
    tree->upgrades[7].required1 = 6; // Lancer
    tree->upgrades[7].required2 = 5; // Bishop
    strcpy(tree->upgrades[7].description, "A piece empowered by an unknown force");
    strcpy(tree->upgrades[7].unlocksText, "- Catapult upgrade\n- Queen Path");
    strcpy(tree->upgrades[7].givesText, "Unlocks Mage Upgrade to Bishop and Mage training Option");

    // Index 8: Catapult (Needs Mage + Rook)
    strcpy(tree->upgrades[8].name, "Catapult");
    tree->upgrades[8].cost = 18;
    tree->upgrades[8].required1 = 7; // Mage
    tree->upgrades[8].required2 = 4; // Rook
    strcpy(tree->upgrades[8].description, "A powerful yet slow ranged unit");
    strcpy(tree->upgrades[8].unlocksText, "- Queen unlock");
    strcpy(tree->upgrades[8].givesText, "Unlocks Knight training option");

    // Index 9: Queen (Needs Rook + Catapult)
    strcpy(tree->upgrades[9].name, "Queen");
    tree->upgrades[9].cost = 20;
    tree->upgrades[9].required1 = 4; // Rook
    tree->upgrades[9].required2 = 8; // Catapult
    strcpy(tree->upgrades[9].description, "The Strongest on the board some may call her");
    strcpy(tree->upgrades[9].unlocksText, "A path to Victory!!");
    strcpy(tree->upgrades[9].givesText, "Unlocks Knight training option");

    // Index 10: Extra Pieces (Needs Extra Money)
    strcpy(tree->upgrades[10].name, "Unit Cap increase");
    tree->upgrades[10].cost = 15;
    tree->upgrades[10].required1 = 2; // Extra Money
    tree->upgrades[10].required2 = -1;
    strcpy(tree->upgrades[10].description, "Army growth for longevity");
    strcpy(tree->upgrades[10].givesText, "Increase Unit cap by 5");

    // Index 11: UPG Platform II (Needs UPG Platform I)
    strcpy(tree->upgrades[11].name, "UPG PLAT II");
    tree->upgrades[11].cost = 18;
    tree->upgrades[11].required1 = 3; // Plat I
    tree->upgrades[11].required2 = -1;
    strcpy(tree->upgrades[11].description, "A platform of magical powers that can improve your pieces");
    strcpy(tree->upgrades[11].unlocksText, "- Unlocks UPG PLAT III");
    strcpy(tree->upgrades[11].givesText, "Improves upgrade chances Rook, Bishop and Lancer");

    // Index 12: UPG Platform III (Needs UPG Platform II)
    strcpy(tree->upgrades[12].name, "UPG PLAT III");
    tree->upgrades[12].cost = 25;
    tree->upgrades[12].required1 = 11; // Plat II
    tree->upgrades[12].required2 = -1;
    strcpy(tree->upgrades[12].description, "A platform of magical powers that can improve your pieces");
    strcpy(tree->upgrades[12].unlocksText, "An outlet for your inner gambler");
    strcpy(tree->upgrades[12].givesText, "Improves upgrade chances for a Queen, Catapult and Mage");

    tree->upgradePlatformLevel = 0;

    for (int i = 0; i < MAX_UPGRADES; i++) tree->upgrades[i].unlocked = 0;
}

// In techTree.c
bool unlockUpgrade(TechTree *tree, int index, int *gold)
{
    Upgrade *u = &tree->upgrades[index];

    // Check first requirement
    if (u->required1 != -1 && !tree->upgrades[u->required1].unlocked) return false;
    
    // Check second requirement (if it exists)
    if (u->required2 != -1 && !tree->upgrades[u->required2].unlocked) return false;

    if (u->unlocked) return false;

    if (*gold >= u->cost)
    {
        *gold -= u->cost;
        u->unlocked = 1;

        if (index == 3)
        {
            tree->upgradePlatformLevel = 1;
        }
        else if (index == 11)
        {
            tree->upgradePlatformLevel = 2;
        }
        else if (index == 12)
        {
            tree->upgradePlatformLevel = 3;
        }

        printf("Unlocked: %s\n", u->name);
        return true;
    }
    return false;
}

void techTreeOverlay(AppState *app)
{
    SDL_FRect bgPos = {
        100.0f,
        (WINDOW_HEIGHT - 800) / 2.0f,
        1300,
        731
    };

    SDL_RenderTexture(app->renderer,
                      app->techTreeBackground,
                      NULL,
                      &bgPos);

    TechTree *tree =
        (app->currentPlayer == 1)
        ? &app->techTreeP1
        : &app->techTreeP2;

    int *gold =
        (app->currentPlayer == 1)
        ? &app->P1.p1Gold
        : &app->P2.p2Gold;

    SDL_FRect titleRect = {
        900,
        50,
        400,
        60
    };

    drawText(app,
             app->fontLarge,
             "TECH TREE",
             titleRect);

    /*
        THESE are now the REAL node/icon positions.
        Adjust these until they perfectly line up
        with the circles/icons on your background.
    */
    static const SDL_FRect nodePositions[MAX_UPGRADES] =
    {
        { 422 +100.0f, 351 + ((WINDOW_HEIGHT - 800) / 2.0f), 87, 109 },   // Knight
        { 275 +100.0f, 554 + ((WINDOW_HEIGHT - 800) / 2.0f), 87, 109 },   // Envoy
        { 275 +100.0f, 209 + ((WINDOW_HEIGHT - 800) / 2.0f), 87, 109 },   // Town Income
        { 275 +100.0f, 30 + ((WINDOW_HEIGHT - 800) / 2.0f), 87, 109 },   // Platform I
        { 827 +100.0f, 242 + ((WINDOW_HEIGHT - 800) / 2.0f), 87, 109 },   // Rook
        { 628 +100.0f, 351 + ((WINDOW_HEIGHT - 800) / 2.0f), 87, 109 },   // Bishop
        { 422 +100.0f, 554 + ((WINDOW_HEIGHT - 800) / 2.0f), 87, 109 },   // Lancer
        { 628 +100.0f, 554 + ((WINDOW_HEIGHT - 800) / 2.0f), 87, 109 },   // Mage
        { 827 +100.0f, 554 + ((WINDOW_HEIGHT - 800) / 2.0f), 87, 109 },   // Catapult
        { 1078 +100.0f, 399 + ((WINDOW_HEIGHT - 800) / 2.0f), 87, 109 },  // Queen
        { 422 +100.0f, 209 + ((WINDOW_HEIGHT - 800) / 2.0f), 87, 109 },   // Unit Cap
        { 523 +100.0f, 30 + ((WINDOW_HEIGHT - 800) / 2.0f), 87, 109 },   // Platform II
        { 777 +100.0f, 30 + ((WINDOW_HEIGHT - 800) / 2.0f), 87, 109 }    // Platform III
    };

    SDL_FPoint mouse = {
        app->input.mouseX,
        app->input.mouseY
    };

    /*
        Persistent selected node
    */
    static int selectedUpgrade = -1;

    for (int i = 0; i < MAX_UPGRADES; i++)
    {
        if (strlen(tree->upgrades[i].name) == 0)
            continue;

        SDL_FRect nodeRect = nodePositions[i];

        TechNodeState state =
            getTechState(tree, i, *gold);

        SDL_Texture *tex =
            getTechTexture(app, i, state);

        if (tex)
        {
            SDL_RenderTexture(app->renderer,
                              tex,
                              NULL,
                              &nodeRect);
        }

        /*
            Clicking a node selects it
        */
        if (app->input.mouseLeftPressed)
        {
            if (SDL_PointInRectFloat(&mouse,
                                     &nodeRect))
            {
                selectedUpgrade = i;
            }
        }
    }

    /*
        Draw selected node description
    */
    if (selectedUpgrade != -1)
    {
        descBox(app,
                &tree->upgrades[selectedUpgrade]);
    }
}


bool isUpgradeUnlocked(TechTree *tree, int index)
{
    if (index < 0 || index >= MAX_UPGRADES) return false;

    return tree->upgrades[index].unlocked;
}

TechNodeState getTechState(TechTree *tree, int index, int currentGold)
{
    Upgrade *u = &tree->upgrades[index];

    if (u->unlocked)
        return TECH_BOUGHT;

    // Requirement check
    if (u->required1 != -1 &&
        !tree->upgrades[u->required1].unlocked)
        return TECH_LOCKED;

    if (u->required2 != -1 &&
        !tree->upgrades[u->required2].unlocked)
        return TECH_LOCKED;

    // Gold check
    if (currentGold < u->cost)
        return TECH_LOCKED;

    return TECH_BUYABLE;
}

SDL_Texture *getTechTexture(AppState *app, int index, TechNodeState state)
{
    int p1 = (app->currentPlayer == 1);

    switch(index)
    {
        // 0 - Knight
        case 0:
            if (p1)
            {
                if (state == TECH_BOUGHT) return app->techTreeBlueKnightBought;
                if (state == TECH_BUYABLE) return app->techTreeBlueKnightBuyable;
                return app->techTreeBlueKnightLocked;
            }
            else
            {
                if (state == TECH_BOUGHT) return app->techTreeRedKnightBought;
                if (state == TECH_BUYABLE) return app->techTreeRedKnightBuyable;
                return app->techTreeRedKnightLocked;
            }

        // 1 - Envoy
        case 1:
            if (p1)
            {
                if (state == TECH_BOUGHT) return app->techTreeBlueEnvoyBought;
                if (state == TECH_BUYABLE) return app->techTreeBlueEnvoyBuyable;
                return app->techTreeBlueEnvoyLocked;
            }
            else
            {
                if (state == TECH_BOUGHT) return app->techTreeRedEnvoyBought;
                if (state == TECH_BUYABLE) return app->techTreeRedEnvoyBuyable;
                return app->techTreeRedEnvoyLocked;
            }

        // 2 - Town Income
        case 2:
            if (p1)
            {
                if (state == TECH_BOUGHT) return app->techTreeBlueGoldBought;
                if (state == TECH_BUYABLE) return app->techTreeBlueGoldBuyable;
                return app->techTreeBlueGoldLocked;
            }
            else
            {
                if (state == TECH_BOUGHT) return app->techTreeRedGoldBought;
                if (state == TECH_BUYABLE) return app->techTreeRedGoldBuyable;
                return app->techTreeRedGoldLocked;
            }

        // 3 - Upgrade Platform I
        case 3:
        case 11:
        case 12:
            if (p1)
            {
                if (state == TECH_BOUGHT) return app->techTreeBluePlatformBought;
                if (state == TECH_BUYABLE) return app->techTreeBluePlatformBuyable;
                return app->techTreeBluePlatformLocked;
            }
            else
            {
                if (state == TECH_BOUGHT) return app->techTreeRedPlatformBought;
                if (state == TECH_BUYABLE) return app->techTreeRedPlatformBuyable;
                return app->techTreeRedPlatformLocked;
            }

        // 4 - Rook
        case 4:
            if (p1)
            {
                if (state == TECH_BOUGHT) return app->techTreeBlueRookBought;
                if (state == TECH_BUYABLE) return app->techTreeBlueRookBuyable;
                return app->techTreeBlueRookLocked;
            }
            else
            {
                if (state == TECH_BOUGHT) return app->techTreeRedRookBought;
                if (state == TECH_BUYABLE) return app->techTreeRedRookBuyable;
                return app->techTreeRedRookLocked;
            }

        // 5 - Bishop
        case 5:
            if (p1)
            {
                if (state == TECH_BOUGHT) return app->techTreeBlueBishopBought;
                if (state == TECH_BUYABLE) return app->techTreeBlueBishopBuyable;
                return app->techTreeBlueBishopLocked;
            }
            else
            {
                if (state == TECH_BOUGHT) return app->techTreeRedBishopBought;
                if (state == TECH_BUYABLE) return app->techTreeRedBishopBuyable;
                return app->techTreeRedBishopLocked;
            }

        // 6 - Lancer
        case 6:
            if (p1)
            {
                if (state == TECH_BOUGHT) return app->techTreeBlueLancerBought;
                if (state == TECH_BUYABLE) return app->techTreeBlueLancerBuyable;
                return app->techTreeBlueLancerLocked;
            }
            else
            {
                if (state == TECH_BOUGHT) return app->techTreeRedLancerBought;
                if (state == TECH_BUYABLE) return app->techTreeRedLancerBuyable;
                return app->techTreeRedLancerLocked;
            }

        // 7 - Mage
        case 7:
            if (p1)
            {
                if (state == TECH_BOUGHT) return app->techTreeBlueMageBought;
                if (state == TECH_BUYABLE) return app->techTreeBlueMageBuyable;
                return app->techTreeBlueMageLocked;
            }
            else
            {
                if (state == TECH_BOUGHT) return app->techTreeRedMageBought;
                if (state == TECH_BUYABLE) return app->techTreeRedMageBuyable;
                return app->techTreeRedMageLocked;
            }

        // 8 - Catapult
        case 8:
            if (p1)
            {
                if (state == TECH_BOUGHT) return app->techTreeBlueCatapultBought;
                if (state == TECH_BUYABLE) return app->techTreeBlueCatapultBuyable;
                return app->techTreeBlueCatapultLocked;
            }
            else
            {
                if (state == TECH_BOUGHT) return app->techTreeRedCatapultBought;
                if (state == TECH_BUYABLE) return app->techTreeRedCatapultBuyable;
                return app->techTreeRedCatapultLocked;
            }

        // 9 - Queen
        case 9:
            if (p1)
            {
                if (state == TECH_BOUGHT) return app->techTreeBlueQueenBought;
                if (state == TECH_BUYABLE) return app->techTreeBlueQueenBuyable;
                return app->techTreeBlueQueenLocked;
            }
            else
            {
                if (state == TECH_BOUGHT) return app->techTreeRedQueenBought;
                if (state == TECH_BUYABLE) return app->techTreeRedQueenBuyable;
                return app->techTreeRedQueenLocked;
            }

        // 10 - Unit Cap Increase
        case 10:
            if (p1)
            {
                if (state == TECH_BOUGHT) return app->techTreeBluePiecesBought;
                if (state == TECH_BUYABLE) return app->techTreeBluePiecesBuyable;
                return app->techTreeBluePiecesLocked;
            }
            else
            {
                if (state == TECH_BOUGHT) return app->techTreeRedPiecesBought;
                if (state == TECH_BUYABLE) return app->techTreeRedPiecesBuyable;
                return app->techTreeRedPiecesLocked;
            }

        default:
            return NULL;
    }
}


void descBox(AppState *app, Upgrade *u) {
    float menuRightEdge = 100.0f + 1200.0f;
    SDL_FRect panel = { menuRightEdge + 20.0f, (float)(WINDOW_HEIGHT - 600) / 2.0f, 400.0f, 600.0f };

    // Draw background card panel
    SDL_SetRenderDrawBlendMode(app->renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(app->renderer, 45, 30, 50, 240);
    SDL_RenderFillRect(app->renderer, &panel);
    SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, 255);
    SDL_RenderRect(app->renderer, &panel);

    // Maximum text rendering space boundary width
    float maxTextWidth = panel.w - 40.0f; 

    // 1. Draw Title Text
    SDL_FRect nameRect = { panel.x + 20, panel.y + 20, maxTextWidth, 40 };
    drawText(app, app->fontLarge, u->name, nameRect);

    // 2. Draw Wrapped Paragraph Body Description
    SDL_FRect descRect = { panel.x + 20, panel.y + 80, maxTextWidth, 120 };
    drawTextWrapped(app, app->font, u->description, descRect, (int)maxTextWidth);

    // 3. Draw Requirements / Unlocks Category
    SDL_FRect labelUnlocks = { panel.x + 20, panel.y + 190, maxTextWidth, 30 };
    drawText(app, app->font, "Unlocks:", labelUnlocks);
    
    SDL_FRect textUnlocks = { panel.x + 20, panel.y + 220, maxTextWidth, 60 };
    drawTextWrapped(app, app->font, u->unlocksText, textUnlocks, (int)maxTextWidth);

    // 4. Draw Yields / Benefits Category
    SDL_FRect labelGives = { panel.x + 20, panel.y + 330, maxTextWidth, 30 };
    drawText(app, app->font, "Gives:", labelGives);
    
    SDL_FRect textGives = { panel.x + 20, panel.y + 370, maxTextWidth, 60 };
    drawTextWrapped(app, app->font, u->givesText, textGives, (int)maxTextWidth);

    TechTree *tree =
        (app->currentPlayer == 1)
        ? &app->techTreeP1
        : &app->techTreeP2;

    int *gold =
        (app->currentPlayer == 1)
        ? &app->P1.p1Gold
        : &app->P2.p2Gold;

    SDL_FRect unlockButton = {
        panel.x + 80,
        panel.y + 500,
        240,
        70
    };

    TechNodeState state =
        getTechState(tree,
                    (int)(u - tree->upgrades),
                    *gold);

    SDL_Texture *buttonTex;

    if (state == TECH_BOUGHT)
        buttonTex = app->buttonOff;
    else if (state == TECH_BUYABLE)
        buttonTex = app->buttonOn;
    else
        buttonTex = app->buttonOff;

    SDL_RenderTexture(app->renderer,
                    buttonTex,
                    NULL,
                    &unlockButton);

    char buttonText[64];

    if (state == TECH_BOUGHT)
    {
        snprintf(buttonText,
                sizeof(buttonText),
                "UNLOCKED");
    }
    else
    {
        snprintf(buttonText,
                sizeof(buttonText),
                "UNLOCK - %d GOLD",
                u->cost);
    }

    drawText(app,
            app->font,
            buttonText,
            unlockButton);

    if (app->input.mouseLeftPressed)
    {
        SDL_FPoint mouse = {
            app->input.mouseX,
            app->input.mouseY
        };

        if (SDL_PointInRectFloat(&mouse,
                                &unlockButton))
        {
            if (state == TECH_BUYABLE)
            {
                int index =
                    (int)(u - tree->upgrades);

                unlockUpgrade(tree,
                            index,
                            gold);
            }
        }
    }

}

bool isUpgradePlatformUnlocked(TechTree *tree)
{
    return tree->upgradePlatformLevel > 0;
}

bool isSelectedPieceOnUpgradeTile(AppState *app)
{
    if (!app->selectedPiece)
        return false;

    int row = app->selectedPiece->row;
    int col = app->selectedPiece->col;

    return map_data[row][col] == 'U';
}
