//Wiktor
#include "../../lib/raylib.h"
#include "../../lib/raygui.h"
#include "scenes.h"
#include <stdio.h>
#include "../scripts/global.h"
#include "../scripts/player/player.h"
#include "../scripts/styles.h"
#include <string.h>  // For strcpy and strcat

// Define options text and font size
const char *optionsTxt = "OPTIONS";
const int optionsTxtFontSize = 30;

// Define fullscreen state
//bool isFullscreen = false;
bool fullscreenCheckBox = false;  // For GuiCheckBox state

// Function to update resolution string
void updateResolutionString(char *resolutionText) {
    sprintf(resolutionText, "%dx%d", resolutions[currentResolutionIndex].width, resolutions[currentResolutionIndex].height);
}

// Function to handle resolution change
void handleResolutionChange() {
    // Draw the label "Resolution:" on the left side
    DrawText("Resolution:", GetScreenWidth() / 2 - 200, GetScreenHeight() / 2, 20, WHITE);

    // Prepare to draw the current resolution
    char resolutionText[32];
    updateResolutionString(resolutionText); // Update resolution text

    // Center the resolution text dynamically
    int resolutionTextX = GetScreenWidth() / 2 + 100; // X position for resolution text
    int resolutionTextY = GetScreenHeight() / 2;      // Y position for resolution text
    DrawText(resolutionText, resolutionTextX, resolutionTextY, 20, WHITE);

    // Check if the mouse is over the resolution text
    int textWidth = MeasureText(resolutionText, 20);  // Measure the width of the resolution text
    Rectangle resolutionBounds = {resolutionTextX, resolutionTextY, textWidth, 20};  // Bounds of the resolution text

    if (CheckCollisionPointRec(GetMousePosition(), resolutionBounds)) {
        // Allow switching resolution with mouse wheel or clicks if mouse is over the resolution
        DrawRectangleLines(resolutionBounds.x, resolutionBounds.y, resolutionBounds.width, resolutionBounds.height, GREEN);  // Optional: Draw a highlight to indicate hover

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || GetMouseWheelMove() > 0) {
            // Switch to the next resolution
            currentResolutionIndex = (currentResolutionIndex + 1) % numResolutions;
            SetWindowSize(resolutions[currentResolutionIndex].width, resolutions[currentResolutionIndex].height);
        } else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) || GetMouseWheelMove() < 0) {
            // Switch to the previous resolution
            currentResolutionIndex = (currentResolutionIndex - 1 + numResolutions) % numResolutions;
            SetWindowSize(resolutions[currentResolutionIndex].width, resolutions[currentResolutionIndex].height);
        } 
    }
}

// Function to handle fullscreen toggle
void handleFullscreenToggle() {
    // Draw the label "Fullscreen:" on the left side
    DrawText("Fullscreen:", GetScreenWidth() / 2 - 200, GetScreenHeight() / 2 + 80, 20, WHITE);

    // Draw the fullscreen checkbox on the right side of the same line
    if (GuiCheckBox((Rectangle){GetScreenWidth() / 2 + 100, GetScreenHeight() / 2 + 80, 20, 20}, "ON", &fullscreenCheckBox)) {
        if (fullscreenCheckBox && !IsWindowFullscreen()) {
            ToggleFullscreen();
        } else if (!fullscreenCheckBox && IsWindowFullscreen()) {
            ToggleFullscreen();
        }
    }
}

void optionsScene() {
    // Get mouse position
    mousePoint = GetMousePosition();
    
    // Clear the background
    ClearBackground(bgColor);

    // Draw title text, centered at the top
    DrawText(optionsTxt, GetScreenWidth() / 2 - MeasureText(optionsTxt, optionsTxtFontSize) / 2, GetScreenHeight() / 16, optionsTxtFontSize, WHITE);

    // Handle resolution change
    handleResolutionChange();

    // Handle fullscreen toggle
    handleFullscreenToggle();

    // Back Button
    Rectangle exitBtn = {GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 + 200, 200, 50};
    if (GuiButton(exitBtn, "SAVE")) {
        //saveSettings();
    }

    // Save Button
    Rectangle saveBtn = {GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 + 260, 200, 50};
    if (GuiButton(saveBtn, "BACK")) {
        *currentScene = MENU;  // Assuming MENU is the constant for your main menu
    }
}
