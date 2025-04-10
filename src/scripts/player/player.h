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
    float footstepTimer;

    Vector2 colliderCenter;  // Center of the oval
    float colliderRadiusX;   // Horizontal radius
    float colliderRadiusY;   // Vertical radius

    float food;
    int health;

    

} Player;

extern Player player;

void initPlayer(Player *player, int screenWidth, int screenHeight, float speed);
void updatePlayer(Player *player, float deltaTime, int **objects, int **details, int rows, int cols, Camera2D camera);
void drawPlayer(Player *player);
void unloadPlayer(Player *player);
//bool checkCollisionWithObjects(Rectangle playerCollider, int **objects, int **details, int rows, int cols);
bool checkCollisionWithObjects(Vector2 colliderCenter, float radiusX, float radiusY, int **objects, int **details, int rows, int cols);
bool CheckCollisionEllipseRec(Vector2 ellipseCenter, float radiusX, float radiusY, Rectangle rect);
void drawPlayerStats(Player *player);
void savePlayerStats(Player *player);
int loadPlayerStats(Player *player);
void useFood(float foodAmound);


#endif  // PLAYER_H