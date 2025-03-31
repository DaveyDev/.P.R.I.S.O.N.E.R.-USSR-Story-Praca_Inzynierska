#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void copyFile(const char *src, const char *dest) {
    FILE *srcFile = fopen(src, "rb");
    if (!srcFile) {
        printf("Failed to open source file: %s\n", src);
        return;
    }

    FILE *destFile = fopen(dest, "wb");
    if (!destFile) {
        printf("Failed to open destination file: %s\n", dest);
        fclose(srcFile);
        return;
    }

    char buffer[4096];
    size_t bytesRead;
    
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), srcFile)) > 0) {
        fwrite(buffer, 1, bytesRead, destFile);
    }

    fclose(srcFile);
    fclose(destFile);
}

void makeGameSave(const char *mapName) {
    char sourceMapPath[256], destMapPath[256];
    char sourceItemPath[256], destItemPath[256];

    snprintf(sourceMapPath, sizeof(sourceMapPath), "data/levels/%s/%s.map", mapName, mapName);
    snprintf(destMapPath, sizeof(destMapPath), "data/saves/save1/%s.map", mapName); //temporary

    snprintf(sourceItemPath, sizeof(sourceItemPath), "data/levels/%s/items.dat", mapName);
    snprintf(destItemPath, sizeof(destItemPath), "data/saves/save1/items.dat"); //temporary 

    copyFile(sourceMapPath, destMapPath);
    copyFile(sourceItemPath, destItemPath);
}