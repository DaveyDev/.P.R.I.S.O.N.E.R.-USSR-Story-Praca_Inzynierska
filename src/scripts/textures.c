
Texture2D grassTexture;
Texture2D sandTexture;
Texture2D fieldTexture;
Texture2D wateredFieldTexture;
Texture2D seedFieldTexture;
Texture2D wateredSeedFieldTexture;
Texture2D emptyHand;
Texture2D drySprout;
Texture2D wateredSprout;
Texture2D smallWeedWateredSprout;
Texture2D bigWeedWateredSprout;
Texture2D plantedField;
Texture2D weedField;




void loadTextures(){

grassTexture = LoadTexture("src/textures/1.png");
sandTexture = LoadTexture("src/textures/2.png");
fieldTexture = LoadTexture("src/textures/3.png");
wateredFieldTexture = LoadTexture("src/textures/31.png");
seedFieldTexture = LoadTexture("src/textures/4.png");
wateredSeedFieldTexture = LoadTexture("src/textures/41.png");
emptyHand = LoadTexture("src/textures/emptyHand.png");
drySprout = LoadTexture("src/textures/5.png");
wateredSprout = LoadTexture("src/textures/51.png");
smallWeedWateredSprout = LoadTexture("src/textures/52.png");
bigWeedWateredSprout = LoadTexture("src/textures/53.png");
plantedField = LoadTexture("src/textures/311.png");
weedField = LoadTexture("src/textures/32.png");


}

void unloadTextures(){
    
    UnloadTexture(grassTexture);
    UnloadTexture(sandTexture);
    UnloadTexture(fieldTexture);
    UnloadTexture(wateredFieldTexture);
    UnloadTexture(seedFieldTexture);
    UnloadTexture(wateredSeedFieldTexture);
    UnloadTexture(emptyHand);
    UnloadTexture(drySprout);
    UnloadTexture(wateredSprout);
    UnloadTexture(smallWeedWateredSprout);
    UnloadTexture(bigWeedWateredSprout);
    UnloadTexture(plantedField);
    UnloadTexture(weedField);
 
}
