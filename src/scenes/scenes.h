//Wiktor
#ifndef SCENES_H
#define SCENES_H

#include "../../lib/raylib.h"

// Enum definition for the different game screens
typedef enum AppScene { MENU = 0, OPTIONS, GAME, ENDSCREEN, EDITOR, PAUSE, STARTGAME, CREATE_MAP, END_GAME, WIN_GAME} AppScene;



void mapsEditorScene();
void menuMapEditorScene();
void menuScene();
void optionsScene();
void prisonScene();
void pauseScene();
void endGameScene();
void winGameScene();
void startGameScene();
void createMapScene();



#endif 
