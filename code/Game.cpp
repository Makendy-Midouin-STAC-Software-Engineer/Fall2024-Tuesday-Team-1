/**
 * @file Game.cpp
 * @brief Implementation of the Game class for NumBrainer
 */

#include "Game.h"
#include <raylib.h>
#include <algorithm>

// Constructor initializes all game state variables
Game::Game()
    : turnLimit(0)
    , player1Turns(0)
    , player2Turns(0)
    , remainingTime(TIME_LIMIT)
    , startTime(0)
    , gameOver(false)
    , player1Turn(true)
    , settingUp(true)
    , settingTurnLimit(true)
{}

// Reset all game state variables to their initial values
void Game::reset() {
    player1Number.clear();
    player2Number.clear();
    feedbackMessage.clear();
    feedbackHistory.clear();
    turnLimit = 0;
    player1Turns = 0;
    player2Turns = 0;
    remainingTime = TIME_LIMIT;
    startTime = 0;
    gameOver = false;
    player1Turn = true;
    settingUp = true;
    settingTurnLimit = true;
}

/**
 * Sets the turn limit for the game
 * @param input String containing the desired turn limit
 * @return true if successfully set, false if invalid input
 */
bool Game::setTurnLimit(const std::string& input) {
    try {
        int tempTurnLimit = std::stoi(input);
        if (tempTurnLimit < 1) {
            feedbackMessage = "Turn limit must be at least 1. Try again:";
            return false;
        }
        turnLimit = tempTurnLimit;
        settingTurnLimit = false;
        feedbackMessage = "Player 1, set your 4-digit number.";
        return true;
    }
    catch (...) {
        feedbackMessage = "Invalid input. Please enter a number.";
        return false;
    }
}

/**
 * Validates a 4-digit number input
 * @param number String to validate
 * @return "Valid" if valid, error message if invalid
 */
std::string Game::validateNumber(const std::string& number) {
    if (number.length() != 4) {
        return "Error: Number must be exactly 4 digits long.";
    }

    std::set<char> digits;
    for (char ch : number) {
        if (!isdigit(ch)) {
            return "Error: Only numeric digits (0-9) are allowed.";
        }
        if (digits.find(ch) != digits.end()) {
            return "Error: Digits must not repeat.";
        }
        digits.insert(ch);
    }
    return "Valid";
}

/**
 * Sets a player's secret number during setup
 * @param number The number to set
 * @return true if successfully set, false if invalid
 */
bool Game::setPlayerNumber(const std::string& number) {
    std::string validationMessage = validateNumber(number);
    if (validationMessage != "Valid") {
        feedbackMessage = validationMessage;
        return false;
    }

    if (player1Turn) {
        player1Number = number;
        player1Turn = false;
        feedbackMessage = "Player 2, set your 4-digit number.";
    } else {
        player2Number = number;
        settingUp = false;
        player1Turn = true;
        feedbackMessage = "Game starts! Player 1's turn to guess.";
        remainingTime = TIME_LIMIT;
        startTime = GetTime();
    }
    return true;
}

/**
 * Processes a player's guess during gameplay
 * @param guess The player's guess
 * @return true if guess was processed, false if invalid
 */
bool Game::makeGuess(const std::string& guess) {
    std::string validationMessage = validateNumber(guess);
    if (validationMessage != "Valid") {
        feedbackMessage = validationMessage;
        return false;
    }

    std::string target = player1Turn ? player2Number : player1Number;
    int correctDigits = countCorrectDigits(guess, target);
    int correctPositions = countCorrectPositions(guess, target);

    if (guess == target) {
        // Winner!
        feedbackMessage = (player1Turn ? "Player 1" : "Player 2") + std::string(" wins!");
        gameOver = true;
    } else {
        // Process guess and provide feedback
        feedbackMessage = (player1Turn ? "Player 1" : "Player 2");
        feedbackMessage += ": " + std::to_string(correctDigits) + " correct digits, " +
                          std::to_string(correctPositions) + " in position.";

        if (player1Turn) player1Turns++;
        else player2Turns++;

        if (player1Turns < turnLimit || player2Turns < turnLimit) {
            switchTurns();
        }

        if (player1Turns >= turnLimit && player2Turns >= turnLimit) {
            feedbackMessage = "Turn limit reached! It's a draw.";
            gameOver = true;
        }
    }

    addFeedback(guess, feedbackMessage);
    return true;
}

/**
 * Updates the turn timer and handles timeouts
 * @param currentTime Current game time
 */
void Game::updateTimer(double currentTime) {
    if (!gameOver && !settingTurnLimit && !settingUp) {
        remainingTime = TIME_LIMIT - (currentTime - startTime);
        if (remainingTime <= 0) {
            feedbackMessage = (player1Turn ? "Player 1" : "Player 2") + std::string(" ran out of time!");
            if (player1Turn) player1Turns++;
            else player2Turns++;
            switchTurns();
        }
    }
}

/**
 * Counts how many digits in the guess appear in the target number
 * @return Number of matching digits regardless of position
 */
int Game::countCorrectDigits(const std::string& guess, const std::string& target) const {
    int correctCount = 0;
    for (char ch : guess) {
        if (target.find(ch) != std::string::npos) correctCount++;
    }
    return correctCount;
}

/**
 * Counts how many digits are in the correct position
 * @return Number of digits in correct position
 */
int Game::countCorrectPositions(const std::string& guess, const std::string& target) const {
    int correctPosCount = 0;
    for (size_t i = 0; i < guess.length(); i++) {
        if (guess[i] == target[i]) correctPosCount++;
    }
    return correctPosCount;
}

// Switch active player and reset timer
void Game::switchTurns() {
    player1Turn = !player1Turn;
    startTime = GetTime();
    remainingTime = TIME_LIMIT;
}

// Add guess and feedback to history
void Game::addFeedback(const std::string& guess, const std::string& message) {
    feedbackHistory.push_back("Guess: " + guess + " - " + message);
}