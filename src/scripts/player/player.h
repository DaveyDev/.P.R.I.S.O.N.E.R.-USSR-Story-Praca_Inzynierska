#ifndef PLAYER_H
#define PLAYER_H

#include "../../../lib/raylib.h"
#include "../animation.h"

typedef struct {
    Vector2 position;
    Texture2D playerTexture;
    float speed;
    Rectangle collider;
    SpriteAnimation playerAnimation;

} Player;


void initPlayer(Player *player, int screenWidth, int screenHeight, float speed);
void updatePlayer(Player *player, float deltaTime);
void drawPlayer(Player *player);
void unloadPlayer(Player *player);


#endif  // PLAYER_H