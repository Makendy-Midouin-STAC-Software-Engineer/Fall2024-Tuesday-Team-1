#ifndef GAMEUI_H
#define GAMEUI_H

#include "Game.h"
#include "raylib.h"
#include <string>

// Handles all user interface aspects of the game including:
// - Window management
// - Input processing
// - Rendering game state
// - Visual feedback
class GameUI {
public:
    GameUI();
    void run();

private:
    void processInput();
    void render();
    void drawGameSetup();
    void drawGameStatistics();

    // Game instance and UI state
    Game game;
    std::string currentInput;

    // Window and display constants
    static constexpr int WINDOW_WIDTH = 1000;
    static constexpr int WINDOW_HEIGHT = 800;
    static constexpr int TITLE_FONT_SIZE = 50;
    static constexpr int NORMAL_FONT_SIZE = 20;
    static constexpr int INPUT_FONT_SIZE = 25;
};

#endif // GAMEUI_H