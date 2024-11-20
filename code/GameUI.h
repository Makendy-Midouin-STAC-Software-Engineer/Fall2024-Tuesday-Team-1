#ifndef GAMEUI_H
#define GAMEUI_H

#include "raylib.h"
#include "Game.h"
#include <string>

class GameUI {
public:
    GameUI();
    void initialize();
    void run();
    void shutdown();

private:
    void processInput();
    void render();
    void drawGameScreen();
    void drawGameStatistics();
    void handleKeyPress(int key);

    Game game;               // Game logic instance
    std::string currentInput; // Current user input
    bool shouldClose;        // Flag to close the game
    static const int SCREEN_WIDTH = 800;
    static const int SCREEN_HEIGHT = 600;
    static const char* WINDOW_TITLE; // Window title
};

#endif // GAMEUI_H
