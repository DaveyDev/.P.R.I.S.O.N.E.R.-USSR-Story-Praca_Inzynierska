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
    float extraFood;
    float health;
    float extraHealth;
    int maxFood;
    int maxHealth;
    float lastDamageTime; // Global or part of the player struct

    bool wasKnockedOutToday;
    //JobPlayer currentJob;
    
    bool isStateDone;



    

} Player;

extern Player player;

void initPlayer(Player *player, int screenWidth, int screenHeight, float speed);
void updatePlayer(Player *player, float deltaTime, int **objects, int **details, int rows, int cols, Camera2D camera);
void drawPlayer(Player *player);
void unloadPlayer(Player *player);
//bool checkCollisionWithObjects(Rectangle playerCollider, int **objects, int **details, int rows, int cols);
bool checkCollisionWithObjects(Vector2 colliderCenter, float radiusX, float radiusY, int **objects, int **details, int rows, int cols);
bool CheckCollisionEllipseRec(Vector2 ellipseCenter, float radiusX, float radiusY, Rectangle rect);
void drawPlayerStats(Player *player, int fontSize, Color textColor, Color bgColor);
void savePlayerStats(Player *player, Camera2D camera);
int loadPlayerStats(Player *player, Camera2D camera);
void useFood(float foodAmound);
void useHealth(float healthAmount);
void drainExtraStats(float deltaTime);
//struct NPC; // forward declaration
//void updateDoors(Vector2 playerPos, float playerRadiusX, float playerRadiusY, struct NPC *npcs, int npcCount);



#endif  // PLAYER_H