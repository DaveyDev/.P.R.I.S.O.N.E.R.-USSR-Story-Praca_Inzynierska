//Hubert
#ifndef TRANSLATION_H
#define TRANSLATION_H

#include "../../lib/cJSON.h"

typedef enum {
    LANG_POLISH,
    LANG_ENGLISH
} Language;

// Global variable for the current language
extern Language currentLanguage;

// Pointer to store the loaded translations
extern cJSON *translations;

/**
 * Loads the translations from the specified JSON file.
 * 
 * @param languageFile The file path for the language JSON file (e.g., "translations/eng.json").
 */

void loadLanguage();

void loadTranslations(const char *languageFile);

/**
 * Retrieves the translation for a given key.
 * 
 * @param key The translation key (e.g., "menu_start").
 * @return The translated string or the key if no translation is found.
 */
const char *getTranslation(const char *key);

/**
 * Frees the memory allocated for translations.
 */
void freeTranslations(void);

#endif // TRANSLATIONS_H
