//Hubert
#include "../../lib/cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "translation.h"


Language currentLanguage = LANG_POLISH;
cJSON *translations = NULL;

void loadLanguage(){
    if(currentLanguage == LANG_ENGLISH) loadTranslations("data/lang/english.json");
    if(currentLanguage == LANG_POLISH) loadTranslations("data/lang/polish.json");
}

void loadTranslations(const char *languageFile) {
    FILE *file = fopen(languageFile, "r");
    if (!file) {
        printf("cannot open language file: %s\n", languageFile);
        return;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *data = (char *)malloc(length + 1);
    fread(data, 1, length, file);
    data[length] = '\0';

    fclose(file);

    translations = cJSON_Parse(data);
    free(data);

    if (!translations) {
        printf("error parsing JSON\n");
    }
}

const char *getTranslation(const char *key) {
    if (!translations) return key;

    cJSON *entry = cJSON_GetObjectItem(translations, key);
    if (!entry) return key;

    return entry->valuestring;
}

void freeTranslations() {
    if (translations) {
        cJSON_Delete(translations);
    }
}
