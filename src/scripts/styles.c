//Wiktor
#include "../../lib/raygui.h"
#include "../../lib/raylib.h"

// Implementation of the InitGuiStyles function
void initGuiStyle() {
        // Set general button styles: no border and larger text
    GuiSetStyle(BUTTON, BORDER_WIDTH, 0);  // No border
    GuiSetStyle(DEFAULT, TEXT_SIZE, 30);    // Bigger text size

    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(BLANK));  // Transparent button
    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, ColorToInt((Color){ 0, 0, 0, 50 }));
    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, ColorToInt((Color){ 0, 0, 0, 50 }));

    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt((Color){ 255, 255, 255, 255 }));
    GuiSetStyle(DEFAULT, TEXT_COLOR_FOCUSED, ColorToInt((Color){ 255, 255, 255, 255 }));
    GuiSetStyle(DEFAULT, TEXT_COLOR_PRESSED, ColorToInt((Color){ 50, 50, 50, 255 }));


    
}
