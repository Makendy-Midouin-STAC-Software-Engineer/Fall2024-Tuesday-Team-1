#include "Game.h"

Game::Game()
    : turnLimit(0)
    , player1Turns(0)
    , player2Turns(0)
    , gameOver(false)
    , player1Turn(true)
    , settingUp(true)
    , settingTurnLimit(true) {
}

void Game::reset() {
    player1Number.clear();
    player2Number.clear();
    feedbackMessage.clear();
    feedbackHistory.clear();
    turnLimit = 0;
    player1Turns = 0;
    player2Turns = 0;
    gameOver = false;
    player1Turn = true;
    settingUp = true;
    settingTurnLimit = true;
}

void Game::setTurnLimit(int limit) {
    turnLimit = limit;
    settingTurnLimit = false;
    feedbackMessage = "Player 1, set your 4-digit number.";
}

std::string validateNumber(const std::string& number) {
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

bool Game::isValidNumber(const std::string& number) const {
    return validateNumber(number) == "Valid";
}

void Game::setPlayerNumber(const std::string& number) {
    if (!isValidNumber(number)) return;

    if (player1Turn) {
        player1Number = number;
        player1Turn = false;
        feedbackMessage = "Player 2, set your 4-digit number.";
    } else {
        player2Number = number;
        settingUp = false;
        player1Turn = true;
        feedbackMessage = "Game starts! Player 1's turn to guess.";
    }
}

std::pair<int, int> Game::makeGuess(const std::string& guess) {
    if (!isValidNumber(guess)) return {0, 0};

    const std::string& target = player1Turn ? player2Number : player1Number;
    int correctDigits = countCorrectDigits(guess, target);
    int correctPositions = countCorrectPositions(guess, target);

    checkGameOver(guess, target);

    if (!gameOver) {
        if (player1Turn) player1Turns++;
        else player2Turns++;

        feedbackMessage = (player1Turn ? "Player 1" : "Player 2");
        feedbackMessage += ": " + std::to_string(correctDigits) + " correct digits, " +
                          std::to_string(correctPositions) + " in position.";

        feedbackHistory.push_back("Guess: " + guess + " - " + feedbackMessage);

        if (player1Turns < turnLimit || player2Turns < turnLimit) {
            switchTurns();
        }

        if (player1Turns >= turnLimit && player2Turns >= turnLimit) {
            feedbackMessage = "Turn limit reached! It's a draw.";
            gameOver = true;
        }
    }

    return {correctDigits, correctPositions};
}

int Game::countCorrectDigits(const std::string& guess, const std::string& target) const {
    int correctCount = 0;
    for (char ch : guess) {
        if (target.find(ch) != std::string::npos) {
            correctCount++;
        }
    }
    return correctCount;
}

int Game::countCorrectPositions(const std::string& guess, const std::string& target) const {
    int correctPosCount = 0;
    for (size_t i = 0; i < guess.length(); i++) {
        if (guess[i] == target[i]) {
            correctPosCount++;
        }
    }
    return correctPosCount;
}

void Game::switchTurns() {
    player1Turn = !player1Turn;
}

void Game::checkGameOver(const std::string& guess, const std::string& target) {
    if (guess == target) {
        feedbackMessage = (player1Turn ? "Player 1" : "Player 2") + std::string(" wins!");
        gameOver = true;
    }
}

// Getters
bool Game::isGameOver() const { return gameOver; }
bool Game::isSettingUp() const { return settingUp; }
bool Game::isSettingTurnLimit() const { return settingTurnLimit; }
bool Game::isPlayer1Turn() const { return player1Turn; }
int Game::getPlayer1Turns() const { return player1Turns; }
int Game::getPlayer2Turns() const { return player2Turns; }
int Game::getTurnLimit() const { return turnLimit; }
const std::string& Game::getFeedbackMessage() const { return feedbackMessage; }
const std::vector<std::string>& Game::getFeedbackHistory() const { return feedbackHistory; }