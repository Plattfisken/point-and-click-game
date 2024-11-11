#pragma once

#include "root.h"
#include "game.h"

int initializeLevels(Level *levels) {
    // Level 1
    levels[0].musicIndex = 0;
    // PlayMusicStream(levels[0].music);

    levels[0].backgroundTextureIndex = 2;

    levels[0].maxEntities = ARRAY_LENGTH(levels[0].entities);
    levels[0].maxInteractables = ARRAY_LENGTH(levels[0].interactables);

    // initializing interactables
    pushInteractable(levels[0].interactables, levels[0].maxInteractables, 1, (vec2){ 100, 200 });

    // Level 2
    levels[1].musicIndex = 1;

    levels[1].backgroundTextureIndex = 2;

    levels[1].maxEntities = ARRAY_LENGTH(levels[1].entities);
    levels[1].maxInteractables = ARRAY_LENGTH(levels[1].interactables);

    // initializing interactables
    pushInteractable(levels[1].interactables, levels[1].maxInteractables, 0, (vec2){ 200, 200 });

    return 1;
}
