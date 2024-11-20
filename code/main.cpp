#include "GameUI.h"

int main() {
    GameUI gameUI;

    // Initialize the game
    gameUI.initialize();

    // Run the game loop
    gameUI.run();

    // Shut down the game
    gameUI.shutdown();

    return 0;
}
