//Wiktor
#include "../../lib/raygui.h"
#include "../../lib/raylib.h"
#include "global.h"

// Implementation of the InitGuiStyles function
void initGuiStyle() {
    // Set general button styles: no border and larger text
    GuiSetStyle(BUTTON, BORDER_WIDTH, 0);  // No border
    GuiSetStyle(DEFAULT, TEXT_SIZE, 30);   // Bigger text size
    GuiSetFont(baseFont);   // Set font

    // Normal button colors
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(BLANK));  
    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, ColorToInt((Color){ 0, 0, 0, 50 }));
    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, ColorToInt((Color){ 0, 0, 0, 50 }));

    // Text colors
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt((Color){ 255, 255, 255, 255 }));
    GuiSetStyle(DEFAULT, TEXT_COLOR_FOCUSED, ColorToInt((Color){ 255, 255, 255, 255 }));
    GuiSetStyle(DEFAULT, TEXT_COLOR_PRESSED, ColorToInt((Color){ 50, 50, 50, 255 }));

    //Disabled Button Customization (Fixes Whiteish Background)
    GuiSetStyle(BUTTON, BASE_COLOR_DISABLED, ColorToInt((Color){ 0, 0, 0, 0 }));   // Darker transparent gray
    GuiSetStyle(BUTTON, TEXT_COLOR_DISABLED, ColorToInt((Color){ 180, 180, 180, 255 })); // Light gray text
    

    GuiSetStyle(DEFAULT, BACKGROUND_COLOR, 0x303030FF); // Dark gray background
    //GuiSetStyle(DEFAULT, TEXT_COLOR, 0xFFFFFFFF);        // White text
    //GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, 0x505050FF); // Buttons default
    GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL, 0x606060FF); // Window border
    GuiSetStyle(DEFAULT, LINE_COLOR, 0x808080FF);        // Separators etc.

}
