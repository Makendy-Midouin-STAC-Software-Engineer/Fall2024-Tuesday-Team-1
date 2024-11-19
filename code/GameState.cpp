#include "GameState.h"
#include <set>

GameState::GameState()
    : player1Number("")
    , player2Number("")
    , player1Turns(0)
    , player2Turns(0)
    , turnLimit(0)
    , timeLimitPerTurn(0)
    , remainingTime(0)
    , player1Turn(true)
    , gameOver(false)
    , settingUp(true)
    , settingTurnLimit(true)
    , settingTimeLimit(false)
{}

std::string GameState::validateNumber(const std::string& number) const {
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

bool GameState::setPlayerNumber(const std::string& number) {
    if (validateNumber(number) != "Valid") {
        return false;
    }

    if (settingUp) {
        if (player1Turn) {
            player1Number = number;
            player1Turn = false;
            addFeedback("Player 2, set your 4-digit number.");
        } else {
            player2Number = number;
            settingUp = false;
            player1Turn = true;
            remainingTime = timeLimitPerTurn;
            addFeedback("Game starts! Player 1's turn to guess.");
        }
        return true;
    }
    return false;
}

void GameState::setTurnLimit(const std::string& limit) {
    try {
        int value = std::stoi(limit);
        if (value > 0) {
            turnLimit = value;
            settingTurnLimit = false;
            settingTimeLimit = true;
            addFeedback("Enter time limit per turn (seconds):");
        }
    } catch (...) {
        // Invalid input - do nothing
    }
}

void GameState::setTimeLimit(const std::string& limit) {
    try {
        int value = std::stoi(limit);
        if (value > 0) {
            timeLimitPerTurn = value;
            settingTimeLimit = false;
            addFeedback("Player 1, set your 4-digit number.");
        }
    } catch (...) {
        // Invalid input - do nothing
    }
}

std::pair<int, int> GameState::evaluateGuess(const std::string& guess, const std::string& target) const {
    int correctDigits = 0;
    int correctPositions = 0;

    for (size_t i = 0; i < guess.length(); i++) {
        if (guess[i] == target[i]) {
            correctPositions++;
        }
        if (target.find(guess[i]) != std::string::npos) {
            correctDigits++;
        }
    }

    return {correctDigits, correctPositions};
}

void GameState::processGuess(const std::string& guess) {
    if (validateNumber(guess) != "Valid") {
        return;
    }

    const std::string& targetNumber = player1Turn ? player2Number : player1Number;
    auto [correctDigits, correctPositions] = evaluateGuess(guess, targetNumber);

    if (correctPositions == 4) {
        std::string winner = (player1Turn ? "Player 1" : "Player 2");
        addFeedback(winner + " wins!");
        gameOver = true;
        return;
    }

    std::string feedback = (player1Turn ? "Player 1" : "Player 2");
    feedback += ": " + std::to_string(correctDigits) + " correct digits, " +
                std::to_string(correctPositions) + " in position.";
    addFeedback("Guess: " + guess + " - " + feedback);

    switchTurns();

    if (player1Turns >= turnLimit && player2Turns >= turnLimit) {
        addFeedback("Turn limit reached! It's a draw.");
        gameOver = true;
    }
}

void GameState::updateRemainingTime(double currentTime) {
    if (isGameplayPhase()) {
        remainingTime = timeLimitPerTurn - static_cast<int>(currentTime);
        if (remainingTime <= 0) {
            addFeedback((player1Turn ? "Player 1" : "Player 2") + std::string(" ran out of time!"));
            switchTurns();
            remainingTime = timeLimitPerTurn;
        }
    }
}

void GameState::switchTurns() {
    if (player1Turn) {
        player1Turns++;
    } else {
        player2Turns++;
    }
    player1Turn = !player1Turn;
}

void GameState::addFeedback(const std::string& message) {
    feedbackHistory.push_back(message);
}

void GameState::reset() {
    player1Number.clear();
    player2Number.clear();
    player1Turns = 0;
    player2Turns = 0;
    turnLimit = 0;
    timeLimitPerTurn = 0;
    remainingTime = 0;
    player1Turn = true;
    gameOver = false;
    settingUp = true;
    settingTurnLimit = true;
    settingTimeLimit = false;
    feedbackHistory.clear();
    addFeedback("Enter the number of turns for this game.");
}