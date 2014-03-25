/*
 =======================================================================================================================
 File       : ConfigList.cpp
 -----------------------------------------------------------------------------------------------------------------------
 Author     : Shinichiro Nakamura
 Modified by: Kleber Kruger
 Date       : 2014-03-01
 Version    : 1.0
 Copyright  : Copyright (C) 2010 Shinichiro Nakamura (CuBeatSystems)
 Reference  : <http://shinta.main.jp/>
 -----------------------------------------------------------------------------------------------------------------------
 Description: Configuration file interface class (Original Version 0.0.1)
 =======================================================================================================================
 */

#include "ConfigList.h"

/**
 * Create a configuration file object.
 *
 * @param file 		- a pointer to a file name
 * @param header 	- a pointer to a header
 * @param format 	- a file format
 */
ConfigList::ConfigList() {

    /* Allocation for a config_t list */
    configlist = (config_t **) calloc(MAXCONFIG, sizeof (config_t *));
}

/**
 * Destroy a configuration file object.
 */
ConfigList::~ConfigList() {

    /* Remove all storage and the contents */
    for (int i = 0; i < MAXCONFIG; i++) {
        config_t *cfg = configlist[i];
        if (cfg != NULL) {
            free(cfg->key);
            free(cfg->value);
            free(cfg);
        }
        configlist[i] = NULL;
    }

    /* Remove cnofig_t list */
    free(configlist);
    configlist = NULL;
}

/*
 * Get a value for a key.
 *
 * @param key 	- a target key name
 *
 * @return 		- a value or NULL
 */
char* ConfigList::getValue(const char *key) {

    /* Null check */
    if (key == NULL) {
        return NULL;
    }

    /* Search a config_t object from the key */
    config_t *p = search(key);
    if (p == NULL) {
        return NULL;
    }

    return p->value;
}

/**
 * Get a value for a key.
 *
 * @param key 	- a target key name
 * @param value - a pointer to a value storage
 * @param size 	- a size of a value storage
 *
 * @return 		- a value or NULL
 */
bool ConfigList::getValue(const char *key, char *value, size_t size) {

    /* Null check */
    if (key == NULL) {
        return false;
    }

    /* Search a config_t object from the key */
    config_t *p = search(key);
    if (p == NULL) {
        return false;
    }

    /* Check the storage size */
    if (size <= strlen(p->value)) {
        return false;
    }

    /* Copy the value to the storage */
    strcpy(value, p->value);
    return true;
}

/**
 * Set a set of a key and value.
 *
 * @param key 	- a key
 * @param value - a value
 *
 * @return 		- true if it succeed
 */
bool ConfigList::setValue(const char *key, const char *value) {

    /* Null check */
    if ((key == NULL) || (value == NULL)) {
        return false;
    }

    /* Size check */
    if ((MAXLEN_KEY < strlen(key)) || (MAXLEN_VALUE < strlen(value))) {
        return false;
    }

    /* Search a config_t object from the key */
    config_t *p = search(key);
    if (p == NULL) {

        /* Allocation a memory for a new key */
        char *k = (char *) malloc(sizeof (char) * (strlen(key) + 1));
        if (k == NULL) {
            return false;
        }
        strcpy(k, key);

        /* Allocation a memory for a new value */
        char *v = (char *) malloc(sizeof (char) * (strlen(value) + 1));
        if (v == NULL) {
            free(k);
            return false;
        }
        strcpy(v, value);

        /* Allocation a memory for a new configuration */
        config_t *cfg = (config_t *) malloc(sizeof (config_t) * 1);
        if (cfg == NULL) {
            free(k);
            free(v);
            return false;
        }
        cfg->key = k;
        cfg->value = v;

        /* Add the new configuration */
        if (!add(cfg)) {
            free(k);
            free(v);
            free(cfg);
            return false;
        }

        return true;

    } else {

        /* The value is same */
        if (strcmp(value, p->value) == 0) {
            return true;
        }

        /* Free a memory for the value */
        free(p->value);
        p->value = NULL;

        /* Allocation memory for the new value */
        char *v = (char *) malloc(sizeof (char) * (strlen(value) + 1));
        if (v == NULL) {
            return false;
        }

        /* Store it */
        strcpy(v, value);
        p->value = v;

        return true;
    }
}

/**
 * Remove a configuration.
 *
 * @param key 	- a key
 *
 * @return 		- true if it succeed
 */
bool ConfigList::remove(const char *key) {
    if (key == NULL) {
        return false;
    }
    for (int i = 0; i < MAXCONFIG; i++) {
        config_t *cfg = configlist[i];
        if (cfg != NULL) {
            if (strcmp(cfg->key, key) == 0) {
                free(cfg->key);
                free(cfg->value);
                free(cfg);
                configlist[i] = NULL;
                return true;
            }
        }
    }
    return false;
}

/**
 * Remove all configuration
 *
 * @return - true if it succeed
 */
bool ConfigList::removeAll(void) {
    for (int i = 0; i < MAXCONFIG; i++) {
        config_t *p = configlist[i];
        if (p != NULL) {
            free(p->key);
            free(p->value);
        }
        free(p);
        configlist[i] = NULL;
    }
    return true;
}

/**
 * Get a number of configuration sets.
 *
 * @return - number of configuration sets
 */
int ConfigList::getCount() {
    int cnt = 0;
    for (int i = 0; i < MAXCONFIG; i++) {
        config_t *p = configlist[i];
        if (p != NULL) {
            cnt++;
        }
    }
    return cnt;
}

/**
 * Get a key and a value.
 *
 * @param index 		- index number of this list
 * @param key 			- a pointer to a buffer for key
 * @param keybufsiz 	- a size of the key buffer
 * @param value 		- a pointer to a buffer for value
 * @param valuebufsiz 	- a size of the value buffer
 *
 * @return 				- true if it succeed
 */
bool ConfigList::getKeyAndValue(int index, char *key, size_t keybufsiz, char *value, size_t valuebufsiz) {
    int cnt = 0;
    for (int i = 0; i < MAXCONFIG; i++) {
        config_t *p = configlist[i];
        if (p != NULL) {
            if (cnt == index) {
                if ((strlen(p->key) < keybufsiz) && (strlen(p->value) < valuebufsiz)) {
                    strcpy(key, p->key);
                    strcpy(value, p->value);
                    return true;
                }
                return false;
            }
            cnt++;
        }
    }
    return false;
}

ConfigList::config_t *ConfigList::search(const char *key) {

    if (key == NULL) {
        return NULL;
    }

    for (int i = 0; i < MAXCONFIG; i++) {
        if (configlist[i] != NULL) {
            if (strcmp(configlist[i]->key, key) == 0) {
                return configlist[i];
            }
        }
    }

    return NULL;
}

bool ConfigList::add(config_t *cfg) {
    for (int i = 0; i < MAXCONFIG; i++) {
        if (configlist[i] == NULL) {
            configlist[i] = cfg;
            return true;
        }
    }
    return false;
}
