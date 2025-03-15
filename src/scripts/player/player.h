#ifndef PLAYER_H
#define PLAYER_H

#include "../../../lib/raylib.h"
#include "../animation.h"


typedef struct {
    Vector2 position;
    Texture2D playerTexture;
    float speed;
    Rectangle collider;
    SpriteAnimation playerAnimation[5];
    

} Player;


void initPlayer(Player *player, int screenWidth, int screenHeight, float speed);
void updatePlayer(Player *player, float deltaTime, int **objects, int **details, int rows, int cols, Camera2D camera);
void drawPlayer(Player *player);
void unloadPlayer(Player *player);
bool checkCollisionWithObjects(Rectangle playerCollider, int **objects, int **details, int rows, int cols);


#endif  // PLAYER_H