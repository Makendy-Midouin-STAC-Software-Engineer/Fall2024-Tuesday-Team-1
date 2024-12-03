/**
* @file GameUI.h
 * @brief Header file for the game's user interface
 *
 * Handles all user interface aspects including:
 * - Window management
 * - Input processing
 * - Rendering game state
 * - Visual feedback and animations
 */

#ifndef GAMEUI_H
#define GAMEUI_H

#include "Game.h"
#include "raylib.h"
#include <string>

class GameUI {
public:
    // Constructor initializes window and basic settings
    GameUI();

    // Main game loop
    void run();

private:
    // Core UI functions
    void processInput();   // Handle all user input
    void render();         // Main render function

    // Specialized rendering functions
    void drawGameState();       // Draws current game state
    void drawTurnLimitSetup();  // Draw turn limit input screen
    void drawNumberSetup();     // Draw number setup screen
    void drawGameplay();        // Draw main gameplay screen
    void drawGameStatistics();  // Draw end game statistics
    void drawFeedbackHistory(); // Draw guess history
    void drawTimer();          // Draw timer with animation

    // Game instance and UI state
    Game game;
    std::string currentInput;
    float timerAnimation;      // Animation value for timer warning

    // Window and display constants
    static constexpr int WINDOW_WIDTH = 1000;
    static constexpr int WINDOW_HEIGHT = 800;
    static constexpr int TITLE_FONT_SIZE = 50;
    static constexpr int NORMAL_FONT_SIZE = 20;
    static constexpr int INPUT_FONT_SIZE = 25;
    static constexpr int FEEDBACK_HISTORY_START_Y = 350;
    static constexpr int MAX_HISTORY_ITEMS = 10;    // Maximum history items to display
    static constexpr int WARNING_TIME = 5;          // Time threshold for warning animation
    static constexpr float ANIMATION_SPEED = 8.0f;  // Animation speed multiplier
};

#endif // GAMEUI_H