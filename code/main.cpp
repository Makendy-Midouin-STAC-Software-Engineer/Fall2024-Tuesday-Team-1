/**
* @file main.cpp
 * @brief Main entry point for the NumBrainer game
 *
 * A two-player number guessing game where each player tries to guess
 * their opponent's 4-digit number
 */

#include "GameUI.h"

int main() {
    // Create and run the game
    GameUI game;
    game.run();
    return 0;
}