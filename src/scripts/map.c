#include <time.h>

#define MAX_ROWS 12
#define MAX_COLS 16
#define WATERED_TILE 31
#define CHANGED_TILE 3





int map[MAX_ROWS][MAX_COLS];
time_t lastChangeTimes[MAX_ROWS][MAX_COLS];


void loadMap(const char *filename) {
    


    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Could not open file.\n");
        exit(EXIT_FAILURE);
    }

    for (int row = 0; row < MAX_ROWS; row++) {
        for (int col = 0; col < MAX_COLS; col++) {
            fscanf(file, "%d", &map[row][col]);
        }
    }

    fclose(file);

}


/*void drawMap(){
    for (int row = 0; row < MAX_ROWS; row++) {
            for (int col = 0; col < MAX_COLS; col++) {
                // Adjust the size and position based on your needs
                Rectangle block = {
                    col * 64,  // X position
                    row * 64,  // Y position
                    64,        // Width
                    64         // Height
                };


                // Draw different textures based on the map values
                switch(map[row][col]){
                    case 1: //grass
                    DrawTexture(grassTexture, block.x, block.y, WHITE);
                    break;
                    case 2: //sand
                    DrawTexture(sandTexture, block.x, block.y, WHITE);

                    
                    
                }
                
                // Add more conditions based on your game's needs

                // You can also add additional logic for player, enemies, etc.
            }
        }
} */

void drawMap(Camera2D camera){
    BeginMode2D(camera);
    for (int row = 0; row < MAX_ROWS; row++) {
            for (int col = 0; col < MAX_COLS; col++) {
                // Adjust the size and position based on your needs
                Rectangle block = {
                    col * 64,  // X position
                    row * 64,  // Y position
                    64,        // Width
                    64         // Height
                };


                // Draw different textures based on the map values
                switch(map[row][col]){
                    case 1: //grass
                    DrawTexture(grassTexture, block.x, block.y, WHITE);
                    break;
                    case 2: //sand
                    DrawTexture(sandTexture, block.x, block.y, WHITE);



                }

                // Add more conditions based on your game's needs

                // You can also add additional logic for player, enemies, etc.
            }
        }
        EndMode2D();
}
void clickOnTile(Player *player){
    
            Vector2 mousePos = GetMousePosition();
            // Check which block was clicked
            int col = (int)(mousePos.x / 64);
            int row = (int)(mousePos.y / 64);

            
            // Change the map (toggle between 1 and 2)
            if (map[row][col] == 1 ) {
                map[row][col] = 2;
            } else if(map[row][col] == 2){
                map[row][col] = 1;
            }
            
}



void updateMap(){
    /*
    time_t currentTime = time(NULL);

    // Iterate over the map
    for (int row = 0; row < MAX_ROWS; row++) {
        for (int col = 0; col < MAX_COLS; col++) {
            // Check if 60 seconds have passed since the last change for this tile
            if(map[row][col] == 3){
                lastChangeTimes[row][col] = currentTime;
            }
            if (map[row][col] == 31 && currentTime - lastChangeTimes[row][col] >= 20) {
                // Change the tile value from WATERED_TILE to CHANGED_TILE
                map[row][col] = 32;
                lastChangeTimes[row][col] = currentTime; // Update the last change time
            }
            if(map[row][col] == 32 && currentTime - lastChangeTimes[row][col] >= 20) {
                // Change the tile value from WATERED_TILE to CHANGED_TILE
                map[row][col] = 3;
                lastChangeTimes[row][col] = currentTime; // Update the last change time
            }
            if(map[row][col] == 51 && currentTime - lastChangeTimes[row][col] >= 100){
                
            }
            if(map[row][col] == 311 && currentTime - lastChangeTimes[row][col] >= 40){
                spawnCoffeeSeed(&coffeeSeedManager->coffeeSeed[coffeeSeedManager->numSeeds], (Vector2){col * 64 + 10, row * 64 + 10});
                coffeeSeedManager-> numSeeds++;
                lastChangeTimes[row][col] = currentTime;
                map[row][col] = 53;
                
            }
            
            if(map[row][col] == 53 && currentTime - lastChangeTimes[row][col] >= 5){
                lastChangeTimes[row][col] = currentTime;
                lastChangeTimes[row+1][col] = currentTime;
                lastChangeTimes[row-1][col] = currentTime;
                lastChangeTimes[row][col+1] = currentTime;
                lastChangeTimes[row][col-1] = currentTime;
                if(map[row+1][col] == 311){map[row+1][col] = 53;}
                if(map[row-1][col] == 311){map[row+1][col] = 53;}
                if(map[row][col+1] == 311){map[row+1][col] = 53;}
                if(map[row][col-1] == 311){map[row+1][col] = 53;}
                
            }
        }
    }*/
}
    
    