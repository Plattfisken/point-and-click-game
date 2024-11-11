#include "root.h"

int initializeEverything() {
    u16 windowWidth = 600, windowHeight = 400;
    InitWindow(windowWidth, windowHeight, "Point & Click Game");
    if(!IsWindowReady()) {
        // TODO:
        printf("Window failed to initialize\n");
        return 0;
    }

    InitAudioDevice();
    if(!IsAudioDeviceReady()) {
        // TODO:
        printf("Audio device failed to initialize\n");
        return 0;
    }

    return 1;
}

int main(int argc, char **argv) {
    if(!initializeEverything()){
        printf("Failed to initialize\n");
        return 1;
    }

    SetTargetFPS(60);
    ToggleBorderlessWindowed();

    GameState *gameState = calloc(1, sizeof(GameState));
    if(!gameState) {
        printf("Failed to allocate GameState\n");
        return 1;
    }

    if(!gameSetup(gameState)) {
        printf("Failed to setup game\n");
        return 1;
    }

    gameLoop(gameState);

    return 0;
}
