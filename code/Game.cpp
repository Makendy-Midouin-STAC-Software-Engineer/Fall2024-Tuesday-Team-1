#include "Game.h"
#include <cctype>

// Constructor initializes the game to default state
Game::Game()
    : turnLimit(0), player1Turns(0), player2Turns(0),
      player1Turn(true), gameState(GameState::SettingTurnLimit) {
}

// Reset the game to its initial state
void Game::reset() {
    player1Number.clear();
    player2Number.clear();
    feedbackMessage.clear();
    feedbackHistory.clear();
    turnLimit = 0;
    player1Turns = 0;
    player2Turns = 0;
    player1Turn = true;
    gameState = GameState::SettingTurnLimit;
}

// Set the turn limit for the game
void Game::setTurnLimit(int limit) {
    turnLimit = limit;
    gameState = GameState::SettingPlayerNumbers;
    feedbackMessage = "Player 1, set your 4-digit number.";
}

// Validate if a number is valid (4 digits, unique, numeric)
bool Game::isValidNumber(const std::string& number) const {
    if (number.length() != 4) return false;
    std::set<char> digits;
    for (char ch : number) {
        if (!isdigit(ch) || digits.count(ch)) return false;
        digits.insert(ch);
    }
    return true;
}

// Set the number for the current player
void Game::setPlayerNumber(const std::string& number) {
    if (!isValidNumber(number)) {
        feedbackMessage = "Invalid number. Ensure it has 4 unique digits.";
        return;
    }

    if (player1Turn) {
        player1Number = number;
        player1Turn = false;
        feedbackMessage = "Player 2, set your 4-digit number.";
    } else {
        player2Number = number;
        gameState = GameState::Playing;
        player1Turn = true;
        feedbackMessage = "Game starts! Player 1's turn to guess.";
    }
}

// Make a guess and calculate feedback
std::pair<int, int> Game::makeGuess(const std::string& guess) {
    if (!isValidNumber(guess)) {
        feedbackMessage = "Invalid guess. Ensure it has 4 unique digits.";
        return {0, 0};
    }

    const std::string& target = player1Turn ? player2Number : player1Number;
    int correctDigits = countCorrectDigits(guess, target);
    int correctPositions = countCorrectPositions(guess, target);

    // Record feedback
    feedbackHistory.push_back({guess, correctDigits, correctPositions,
        (player1Turn ? "Player 1" : "Player 2") + std::string(": ") +
        std::to_string(correctDigits) + " correct digits, " +
        std::to_string(correctPositions) + " in correct position."});

    checkGameOver(guess, target);
    if (!isGameOver()) {
        if (player1Turn) player1Turns++;
        else player2Turns++;

        if (player1Turns >= turnLimit && player2Turns >= turnLimit) {
            feedbackMessage = "Turn limit reached! It's a draw.";
            gameState = GameState::GameOver;
        } else {
            switchTurns();
        }
    }

    return {correctDigits, correctPositions};
}

// Count how many digits are correct
int Game::countCorrectDigits(const std::string& guess, const std::string& target) const {
    int correctCount = 0;
    for (char ch : guess) {
        if (target.find(ch) != std::string::npos) {
            correctCount++;
        }
    }
    return correctCount;
}

// Count how many digits are in the correct position
int Game::countCorrectPositions(const std::string& guess, const std::string& target) const {
    int correctPosCount = 0;
    for (size_t i = 0; i < guess.length(); i++) {
        if (guess[i] == target[i]) {
            correctPosCount++;
        }
    }
    return correctPosCount;
}

// Switch turns between players
void Game::switchTurns() {
    player1Turn = !player1Turn;
    feedbackMessage = (player1Turn ? "Player 1's turn to guess." : "Player 2's turn to guess.");
}

// Check if the game is over based on the guess
void Game::checkGameOver(const std::string& guess, const std::string& target) {
    if (guess == target) {
        feedbackMessage = (player1Turn ? "Player 1 wins!" : "Player 2 wins!");
        gameState = GameState::GameOver;
    }
}

// Getters for accessing game state and feedback
bool Game::isGameOver() const { return gameState == GameState::GameOver; }
GameState Game::getGameState() const { return gameState; }
bool Game::isPlayer1Turn() const { return player1Turn; }
int Game::getPlayer1Turns() const { return player1Turns; }
int Game::getPlayer2Turns() const { return player2Turns; }
int Game::getTurnLimit() const { return turnLimit; }
const std::string& Game::getFeedbackMessage() const { return feedbackMessage; }
const std::vector<Feedback>& Game::getFeedbackHistory() const { return feedbackHistory; }
