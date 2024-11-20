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

    Game game;
    std::string currentInput;
    bool shouldClose;
    static const int SCREEN_WIDTH = 800;
    static const int SCREEN_HEIGHT = 600;
    static const char* WINDOW_TITLE;
};

#endif //GAMEUI_H
