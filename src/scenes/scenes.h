#ifndef SCENES_H
#define SCENES_H

#include "../raylib.h"

// Enum definition for the different game screens
typedef enum AppScene { MENU = 0, OPTIONS, GAME, ENDSCREEN} AppScene;



void menuScene();
void optionsScene();
void prisonScene();




#endif 
