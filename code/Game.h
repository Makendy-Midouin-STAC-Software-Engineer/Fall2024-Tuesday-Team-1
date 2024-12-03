/**
 * @file Game.h
 * @brief Header file for the main game logic class of NumBrainer
 *
 * This class handles all game mechanics including:
 * - Turn management
 * - Player number validation
 * - Score tracking
 * - Timer management
 * - Game state transitions
 */

#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>
#include <set>

class Game {
public:
    // Constructor initializes game state
    Game();

    // Resets all game state variables to initial values
    void reset();

    // Game setup and play methods
    bool setTurnLimit(const std::string& input);     // Sets maximum turns per player
    bool setPlayerNumber(const std::string& number); // Sets player's secret number
    bool makeGuess(const std::string& guess);        // Processes a player's guess

    // Game state check methods
    bool isSettingTurnLimit() const { return settingTurnLimit; }
    bool isSettingUp() const { return settingUp; }
    bool isGameOver() const { return gameOver; }
    bool isPlayer1Turn() const { return player1Turn; }

    // Game state getter methods
    int getPlayer1Turns() const { return player1Turns; }
    int getPlayer2Turns() const { return player2Turns; }
    int getTurnLimit() const { return turnLimit; }
    int getRemainingTime() const { return remainingTime; }
    const std::string& getFeedbackMessage() const { return feedbackMessage; }
    const std::vector<std::string>& getFeedbackHistory() const { return feedbackHistory; }

    // Game logic methods
    void updateTimer(double currentTime);  // Updates turn timer, handles timeouts
    static std::string validateNumber(const std::string& number);  // Validates 4-digit input

private:
    // Helper methods
    int countCorrectDigits(const std::string& guess, const std::string& target) const;     // Counts matching digits
    int countCorrectPositions(const std::string& guess, const std::string& target) const;  // Counts correct positions
    void switchTurns();        // Switches active player and resets timer
    void addFeedback(const std::string& guess, const std::string& message);  // Adds to feedback history

    // Game constants
    static constexpr int TIME_LIMIT = 30;      // Fixed time limit of 30 seconds per turn
    static constexpr int WARNING_TIME = 5;     // Time threshold for warning animation

    // Game state - Player data
    std::string player1Number;      // Player 1's secret number
    std::string player2Number;      // Player 2's secret number
    std::string feedbackMessage;    // Current feedback message
    std::vector<std::string> feedbackHistory;  // History of all guesses and feedback

    // Game state - Counters
    int turnLimit;      // Maximum turns per player
    int player1Turns;   // Number of turns taken by player 1
    int player2Turns;   // Number of turns taken by player 2
    int remainingTime;  // Time remaining in current turn
    double startTime;   // Start time of current turn

    // Game state - Flags
    bool gameOver;          // True if game has ended
    bool player1Turn;       // True if it's player 1's turn
    bool settingUp;         // True if players are setting their numbers
    bool settingTurnLimit;  // True if turn limit is being set
};

#endif // GAME_H