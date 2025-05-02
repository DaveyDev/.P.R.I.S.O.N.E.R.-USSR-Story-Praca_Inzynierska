#include "npc.h"

bool dialogOpen = false;
NPC *currentDialogNPC = NULL;
const char *currentDialogText = NULL;


void openTalkMenuForNPC(NPC *npc) {
    dialogOpen = true;
    currentDialogNPC = npc;
    currentDialogText = "Hey, got a job for you. Interested?";
}


void closeDialog() {
    if (currentDialogNPC) {
        currentDialogNPC->behavior = currentDialogNPC->lastBehavior;
        currentDialogNPC->isTalking = false;
    }

    dialogOpen = false;
    currentDialogNPC = NULL;
    currentDialogText = NULL;
}

void drawDialogBox() {
    if (!dialogOpen || !currentDialogNPC) return;

    Rectangle box = { 50, GetScreenHeight() - 150, GetScreenWidth() - 100, 100 };
    DrawRectangleRec(box, DARKGRAY);
    DrawRectangleLinesEx(box, 2, BLACK);

    DrawText(currentDialogText, box.x + 10, box.y + 10, 20, WHITE);

    // Example buttons (fake for now)
    DrawText("1) Accept job", box.x + 10, box.y + 40, 18, LIGHTGRAY);
    DrawText("2) Decline", box.x + 10, box.y + 60, 18, LIGHTGRAY);
}

