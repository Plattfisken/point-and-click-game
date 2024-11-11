#pragma once

#ifndef GAME_H
#define GAME_H

#include "root.h"

#define INVENTORY_DISPLAY_HEIGHT 50
#define INVENTORY_ITEM_PADDING_X 45
#define INVENTORY_ITEM_PADDING_Y 10
#define LEVEL_BORDER_WIDTH 50

typedef struct {
    // Texture2D texture;
    vec2 position;
    vec2 boxSize;
    i32 textureIndex;
    bool exists;
} Interactable;

typedef struct {
    Interactable items[32];
    u8 capacity;
    u8 size;
} Inventory;

typedef struct {
    // Texture2D texture;
    Inventory inventory;
    vec2 position;
    vec2 boxSize;
    vec2 targetPosition;
    i32 textureIndex;
    bool exists;
    u16 moveSpeed;
} Entity;

typedef struct {
    Entity entities[32];
    Interactable interactables[64];
    // Music music;
    // Texture2D background;
    i32 backgroundTextureIndex;
    i32 musicIndex;
    u32 maxEntities;
    u32 maxInteractables;
} Level;

typedef struct {
    Texture2D textures[3];
    Music musics[2];
    Level levels[10];
    Entity players[4];
    int currentLevelIndex;
    int targetLevelIndex;
    i32 playerCount;
    i32 currentPlayerIndex;
    i32 draggedItemIndex;
} GameState;

int gameSetup(GameState *gameState);
int gameUpdateAndRender(GameState *GameState);

Entity* pushEntity(Entity *entityStack, size_t capacity, i32 textureIndex, vec2 position, u16 moveSpeed);
void popEntity(Entity *entity);
void walkEntityTowardsTarget(Entity *entity, f32 deltaTime);

int nextPlayerIndex(Entity *players, i32 playerCount, i32 currentPlayerIndex);
int prevPlayerIndex(Entity *players, i32 playerCount, i32 currentPlayerIndex);

Interactable* pushInteractable(Interactable *interactableStack, size_t capacity, i32 textureIndex, vec2 position);
void popInteractable(Interactable *Interactable);

void addItemToInventory(Inventory *inventory, Interactable *item);

bool updateCameraPos(Camera2D *camera, vec2 playerPos);
#endif

