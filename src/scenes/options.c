//Wiktor
#include "../../lib/raylib.h"
#include "../../lib/raygui.h"
#include "scenes.h"
#include <stdio.h>
#include "../scripts/global.h"
#include "../scripts/player/player.h"
#include "../scripts/styles.h"
#include <string.h>  // For strcpy and strcat
#include "../scripts/translation.h"
#include "../scripts/textures.h"


// Define options text and font size
//const char *optionsTxt = "OPTIONS";
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
    
    Vector2 resolutionLabelPos = {
        GetScreenWidth() / 2 - 200,
        GetScreenHeight() / 2
    };
    DrawTextEx(baseFont, "Resolution:", resolutionLabelPos, 20, 2, WHITE);


    // Prepare to draw the current resolution
    char resolutionText[32];
    updateResolutionString(resolutionText); // Update resolution text

    // Center the resolution text dynamically
    Vector2 resolutionPos = { 
        GetScreenWidth() / 2 + 100,
        GetScreenHeight() / 2
    };
    DrawTextEx(baseFont, resolutionText, resolutionPos, 20, 2, WHITE);

    // Check if the mouse is over the resolution text
    int textWidth = MeasureText(resolutionText, 20);  // Measure the width of the resolution text
    Rectangle resolutionBounds = {resolutionPos.x, resolutionPos.y, textWidth, 20};  // Bounds of the resolution text

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
    //DrawText("Fullscreen:", GetScreenWidth() / 2 - 200, GetScreenHeight() / 2 + 80, 20, WHITE);
    
    Vector2 fullscreenPos = { 
        GetScreenWidth() / 2 - 200, 
        GetScreenHeight() / 2 + 80 
    };
    DrawTextEx(baseFont, "Fullscreen:", fullscreenPos, 20, 2, WHITE);


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

    Vector2 position = { 0, 0 };  // Position where the texture will be drawn
    float scaleMenu = (float)GetScreenHeight() / 800.0f;
    DrawTextureEx(mainMenuBackground, position, 0.0f, scaleMenu, WHITE);


    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // Calculate 1/16 smaller than window size
    int rectWidth = screenWidth * 15 / 16;
    int rectHeight = screenHeight * 15 / 16;

    // Center position (rectangle's center)
    Vector2 rectPosition = { screenWidth / 2, screenHeight / 2 };
    // Origin at the center
    Vector2 origin = { rectWidth / 2, rectHeight / 2 };

    // Draw the rectangle centered on screen
    DrawRectanglePro((Rectangle){ rectPosition.x, rectPosition.y, rectWidth, rectHeight }, 
                 origin, 0, greySeeThroughColor);





    // Draw title text, centered at the top
    //DrawText(optionsTxt, GetScreenWidth() / 2 - MeasureText(optionsTxt, optionsTxtFontSize) / 2, GetScreenHeight() / 16, optionsTxtFontSize, WHITE);
    Vector2 optionsTxtPos = { 
        GetScreenWidth() / 2 - MeasureTextEx(baseFont, getTranslation("options_title"), optionsTxtFontSize, 2).x / 2, 
        GetScreenHeight() / 16 
    };
    DrawTextEx(baseFont, getTranslation("options_title"), optionsTxtPos, optionsTxtFontSize, 2, WHITE);


    // Handle resolution change
    handleResolutionChange();

    // Handle fullscreen toggle
    handleFullscreenToggle();

    // Back Button
    Rectangle exitBtn = {GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 + 200, 200, 50};
    if (GuiButton(exitBtn, getTranslation("general_save"))) {
        //loadTranslations("data/lang/polish.json");
        //saveSettings();
    }

    // Save Button
    Rectangle saveBtn = {GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 + 260, 200, 50};
    if (GuiButton(saveBtn, getTranslation("general_back"))) {
        *currentScene = MENU;  // Assuming MENU is the constant for your main menu
    }
}
