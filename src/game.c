#pragma once

#include "root.h"
#include "game.h"

int gameSetup(GameState *gameState) {
    // load textures
    gameState->textures[0] = LoadTexture("../assets/player-stand.png");
    gameState->textures[1] = LoadTexture("../assets/player-run.png");
    gameState->textures[2] = LoadTexture("../assets/finalNight.PNG");
    for(int i = 0; i < ARRAY_LENGTH(gameState->textures); ++i) {
        if(!IsTextureReady(gameState->textures[i])) {
            printf("failed to load texture at index %d", i);
            return 0;
        }
    }

    // load songs
    gameState->musics[0] = LoadMusicStream("../assets/Song.wav");
    gameState->musics[1] = LoadMusicStream("../assets/Lysna_haR.mp3");
    for(int i = 0; i < ARRAY_LENGTH(gameState->musics); ++i) {
        if(!IsMusicReady(gameState->musics[i])) {
            printf("failed to load music at index %d", i);
            return 0;
        }
    }

    // initializeLevels
    if(!initializeLevels(gameState->levels)) {
        printf("Failed to initialize levels");
        return 0;
    }
    gameState->currentLevelIndex = 0;

    PlayMusicStream(gameState->musics[gameState->levels[gameState->currentLevelIndex].musicIndex]);

    // initializing players
    Entity *players[3];
    players[0] = pushEntity(gameState->players, ARRAY_LENGTH(gameState->players), 0, (vec2){ 200, 200 }, 300);
    players[1] = pushEntity(gameState->players, ARRAY_LENGTH(gameState->players), 1, (vec2){ 400, 200 }, 300);
    players[2] = pushEntity(gameState->players, ARRAY_LENGTH(gameState->players), 1, (vec2){ 600, 200 }, 300);

    gameState->playerCount = ARRAY_LENGTH(players);
    gameState->currentPlayerIndex = 0;
    gameState->draggedItemIndex = -1;

    return 1;
}

void gameLoop(GameState *gameState) {
    while(!WindowShouldClose()) {
        f32 deltaTime = GetFrameTime();
        Level *level = &gameState->levels[gameState->currentLevelIndex];
        Entity *activePlayer = &gameState->players[gameState->currentPlayerIndex];

        // process input
        if(IsMouseButtonPressed(0)) {
            if(GetMousePosition().y > INVENTORY_DISPLAY_HEIGHT) {
                vec2 mousePosition = GetMousePosition();

                activePlayer->targetPosition = mousePosition;
                if(mousePosition.x > GetScreenWidth() - LEVEL_BORDER_WIDTH) {
                    activePlayer->targetPosition.x = GetScreenWidth();
                    gameState->targetLevelIndex++;
                }

                for(int i = 0; i < level->maxInteractables; ++i) {
                    Interactable *interactable = &level->interactables[i];
                    if(interactable->exists) {
                        if((mousePosition.x > interactable->position.x) &&
                           (mousePosition.x < (interactable->position.x + interactable->boxSize.x)) &&
                           (mousePosition.y > interactable->position.y) &&
                           (mousePosition.y < (interactable->position.y + interactable->boxSize.y))) {
                            addItemToInventory(&activePlayer->inventory, interactable);
                            break;
                        }
                    }
                }
            }
        }

        if(IsMouseButtonDown(0)) {
            if(gameState->draggedItemIndex == -1) {
                for(int i = 0; i < activePlayer->inventory.size; ++i) {
                    Interactable *item = &activePlayer->inventory.items[i];
                    vec2 mousePos = GetMousePosition();
                    if((mousePos.x > item->position.x) &&
                       (mousePos.x < (item->position.x + item->boxSize.x)) &&
                       (mousePos.y > item->position.y) &&
                       (mousePos.y < (item->position.y + item->boxSize.y))) {
                        gameState->draggedItemIndex = i;
                    }
                }
            }
        }

        if(IsMouseButtonReleased(0)) { 
            gameState->draggedItemIndex = -1;
        }

        if(IsKeyPressed(KEY_N)) { 
            int nextPlayerIdx = nextPlayerIndex(gameState->players, gameState->playerCount, gameState->currentPlayerIndex);
            if(gameState->players[nextPlayerIdx].exists) {
                gameState->currentPlayerIndex = nextPlayerIdx;
                gameState->draggedItemIndex = -1;
            }
        }

        if(IsKeyPressed(KEY_P)) {
            int prevPlayeridx = prevPlayerIndex(gameState->players, gameState->playerCount, gameState->currentPlayerIndex);
            if(gameState->players[prevPlayeridx].exists) {
                gameState->currentPlayerIndex = prevPlayeridx;
                gameState->draggedItemIndex = -1;
            }
        }

        if(IsKeyPressed(KEY_SPACE)) {
            pushEntity(level->entities, level->maxEntities, 0, activePlayer->position, 100);
        }

        if(IsKeyPressed(KEY_D)) {
            for(int i = level->maxEntities; i >= 0; --i) {
                if(level->entities[i].exists) {
                    popEntity(&level->entities[i]);
                    break;
                }
            }
        }

        if(IsKeyPressed(KEY_V)) {
            pushInteractable(level->interactables, level->maxInteractables, 1, activePlayer->position);
        }

        if(IsKeyPressed(KEY_F)) {
            for(int i = level->maxInteractables; i >= 0; --i) {
                if(level->interactables[i].exists) {
                    popInteractable(&level->interactables[i]);
                    break;
                }
            }
        }

        // update
        UpdateMusicStream(gameState->musics[level->musicIndex]);

        if(floatAlmostEquals(activePlayer->position.x, GetScreenWidth(), 1e-6) && gameState->targetLevelIndex != gameState->currentLevelIndex) {
            Level *prevLevel = level;
            level = &gameState->levels[++gameState->currentLevelIndex];
            if(level->musicIndex != prevLevel->musicIndex) {
                PlayMusicStream(gameState->musics[level->musicIndex]);
            }
            activePlayer->position.x = -activePlayer->boxSize.x; 
            activePlayer->targetPosition.x = LEVEL_BORDER_WIDTH;
        }

        for(int i = 0; i < gameState->playerCount; ++i) {
            if(gameState->players[i].exists) {
                walkEntityTowardsTarget(&gameState->players[i], deltaTime);
            }
        }

        for(int i = 0; i < level->maxEntities; ++i) {
            if(level->entities[i].exists) {
                if(&gameState->levels[gameState->currentLevelIndex].entities[i] != activePlayer) {
                    gameState->levels[gameState->currentLevelIndex].entities[i].targetPosition = (vec2){ i * 100 + activePlayer->position.x, activePlayer->position.y };
                }
                walkEntityTowardsTarget(&gameState->levels[gameState->currentLevelIndex].entities[i], deltaTime);
            }
        }

        for(int i = 0; i < activePlayer->inventory.size; ++i) {
            activePlayer->inventory.items[i].position = 
                (vec2){ (INVENTORY_ITEM_PADDING_X - activePlayer->inventory.items[i].boxSize.x) + 
                INVENTORY_ITEM_PADDING_X * i, INVENTORY_ITEM_PADDING_Y };
        }

        // TODO: maybe make it so the item will stay the same relative to the cursor, instead of a constant offset like this
        // but maybe it's good like this????
        if(gameState->draggedItemIndex != -1) {
            activePlayer->inventory.items[gameState->draggedItemIndex].position = vec2SubtractValue(GetMousePosition(), 10);
        }

        // render
        ClearBackground(WHITE);

        BeginDrawing();

        // drawing background
        DrawTextureEx(gameState->textures[level->backgroundTextureIndex],
                      (vec2){ 0, INVENTORY_DISPLAY_HEIGHT }, 0, 1.0f, WHITE);

        // drawing players
        for(int i = 0; i < gameState->playerCount; ++i) {
            DrawRectangleV(gameState->players[i].position, gameState->players[i].boxSize, GREEN);
            DrawTextureV(gameState->textures[gameState->players[i].textureIndex], gameState->players[i].position, WHITE);
        }

        // drawing entities
        for(int i = 0; i < level->maxEntities; ++i) {
            if(level->entities[i].exists) {
                DrawRectangleV(level->entities[i].position,
                               level->entities[i].boxSize, BLUE);
                DrawTextureV(gameState->textures[level->entities[i].textureIndex],
                             level->entities[i].position, WHITE);
            }
        }

        // drawing interactables
        for(int i = 0; i < level->maxInteractables; ++i) {
            if(level->interactables[i].exists) {
                DrawRectangleV(level->interactables[i].position,
                               level->interactables[i].boxSize, RED);
                DrawTextureV(gameState->textures[level->interactables[i].textureIndex],
                             level->interactables[i].position, WHITE);
            }
        }

        // drawing inventory
        DrawRectangle(0, 0, GetScreenWidth(), INVENTORY_DISPLAY_HEIGHT, GRAY);
        for(int i = 0; i < activePlayer->inventory.size; ++i) {
            if(i != gameState->draggedItemIndex) {
                DrawRectangleV(activePlayer->inventory.items[i].position, activePlayer->inventory.items[i].boxSize, RED);
                DrawTextureV(gameState->textures[activePlayer->inventory.items[i].textureIndex],
                             activePlayer->inventory.items[i].position, WHITE);
            } 
        }

        // drawing dragged item
        if(gameState->draggedItemIndex != -1) {
            DrawRectangleV(activePlayer->inventory.items[gameState->draggedItemIndex].position,
                           activePlayer->inventory.items[gameState->draggedItemIndex].boxSize, RED);
            DrawTextureV(gameState->textures[activePlayer->inventory.items[gameState->draggedItemIndex].textureIndex],
                         activePlayer->inventory.items[gameState->draggedItemIndex].position, WHITE);
        }

        // print fps
        char fps[16];
        sprintf(fps, "FPS: %d", GetFPS());
        DrawText(fps, GetScreenWidth() - 60, 10, 16, BLACK);

        EndDrawing();
    }
}

void walkEntityTowardsTarget(Entity *entity, f32 deltaTime) {
    vec2 direction = vec2Normalize(vec2Sub(entity->targetPosition, entity->position));

    if(!vec2AlmostEquals(entity->position, entity->targetPosition)) {
        vec2 vectorToWalk = vec2Scale(direction, entity->moveSpeed * deltaTime);
        f32 distanceToWalk = vec2Length(vectorToWalk);

        if(distanceToWalk > vec2Distance(entity->position, entity->targetPosition)) { 
            entity->position = entity->targetPosition; 
        }
        else {
            entity->position = vec2Add(entity->position, vectorToWalk);
        }
    }
}

void addItemToInventory(Inventory *inventory, Interactable *item) {
    u8 nextEmptySlot= inventory->size;
    if(nextEmptySlot < inventory->capacity) {
        inventory->items[nextEmptySlot] = *item;
        inventory->items[nextEmptySlot].position = 
            (vec2){ (INVENTORY_ITEM_PADDING_X - item->boxSize.x) + INVENTORY_ITEM_PADDING_X * nextEmptySlot, INVENTORY_ITEM_PADDING_Y }; 
        inventory->size++;

        // TODO: How to do this? Should an interactable be said to stop existing once it enters an inventory? 
        // Currently the inventory will contain a copy of the interactable and not a pointer to it, potentially this should change as well?
        item->exists = false;
    }
}

Entity* pushEntity(Entity *entityStack, size_t capacity, i32 textureIndex, vec2 position, u16 moveSpeed) {
    for(int i = 0; i < capacity; ++i) {
        if(!entityStack[i].exists) {
            Entity *entity = &entityStack[i];

            entity->textureIndex = textureIndex;
            entity->position = position;
            entity->boxSize = (vec2){ 30, 30 };
            entity->targetPosition = position;
            entity->moveSpeed = moveSpeed;
            entity->inventory.capacity = ARRAY_LENGTH(entity->inventory.items);
            entity->inventory.size = 0;
            entity->exists = true;

            return entity;
        }
    }
    // TODO: too many entities
    printf("Can not add more entities\n");
    return 0;
}

void popEntity(Entity *entity) {
    entity->exists = false;
}

Interactable* pushInteractable(Interactable *interactableStack, size_t capacity, i32 textureIndex, vec2 position) {
    for(int i = 0; i < capacity; ++i) {
        if(!interactableStack[i].exists) {
            Interactable *interactable = &interactableStack[i];

            interactable->textureIndex = textureIndex;
            interactable->position = position;
            interactable->boxSize = (vec2){ 30, 30 };
            interactable->exists = true;

            return interactable;
        }
    }
    // TODO: too many interactables
    printf("Can not add more interactables\n");
    return 0;
}

void popInteractable(Interactable *interactable) {
    interactable->exists = false;
}

inline int nextPlayerIndex(Entity *players, i32 playerCount, i32 currentPlayerIndex) {
    if(currentPlayerIndex < 0) currentPlayerIndex = playerCount;
    return ++currentPlayerIndex % playerCount;
}

inline int prevPlayerIndex(Entity *players, i32 playerCount, i32 currentPlayerIndex) {
    if(currentPlayerIndex <= 0) currentPlayerIndex = playerCount;
    return --currentPlayerIndex % playerCount;
}

// bool updateCameraPos(Camera2D *camera, vec2 playerPos) {
//     int cameraBorder = 50; // bad name
//     vec2 screenStartInWorld = GetScreenToWorld2D((vec2){ 0, 0 }, *camera);
//     vec2 screenBorderInWorld = GetScreenToWorld2D((vec2){ GetScreenWidth(), GetScreenHeight() }, *camera);
//
//     if(playerPos.x < screenStartInWorld.x + cameraBorder) {
//         camera->target.x -= GetScreenWidth();
//         return true;
//     }
//     else if(playerPos.y < screenStartInWorld.y + cameraBorder) {
//         camera->target.y -= GetScreenHeight();
//         return true;
//     }
//     else if(playerPos.x > screenBorderInWorld.x - cameraBorder) {
//         camera->target.x += GetScreenWidth();
//         return true;
//     }
//     else if(playerPos.y > screenBorderInWorld.y - cameraBorder) {
//         camera->target.y += GetScreenHeight();
//         return true;
//     }
//     else return false;
// }
