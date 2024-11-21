#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>
#include <set>

// Main game logic class for the NumBrainer game
// Handles all game state and logic including:
// - Turn management
// - Player number validation
// - Score tracking
// - Timer management
// - Game state transitions
class Game {
public:
    Game();
    void reset();

    // Game setup and play methods
    bool setTurnLimit(const std::string& input);
    bool setPlayerNumber(const std::string& number);
    bool makeGuess(const std::string& guess);

    // Game state checks
    bool isSettingTurnLimit() const { return settingTurnLimit; }
    bool isSettingUp() const { return settingUp; }
    bool isGameOver() const { return gameOver; }
    bool isPlayer1Turn() const { return player1Turn; }

    // Game state getters
    int getPlayer1Turns() const { return player1Turns; }
    int getPlayer2Turns() const { return player2Turns; }
    int getTurnLimit() const { return turnLimit; }
    int getRemainingTime() const { return remainingTime; }
    const std::string& getFeedbackMessage() const { return feedbackMessage; }
    const std::vector<std::string>& getFeedbackHistory() const { return feedbackHistory; }

    // Game logic
    void updateTimer(double currentTime);
    static std::string validateNumber(const std::string& number);

private:
    // Helper methods
    int countCorrectDigits(const std::string& guess, const std::string& target) const;
    int countCorrectPositions(const std::string& guess, const std::string& target) const;
    void switchTurns();
    void addFeedback(const std::string& guess, const std::string& message);

    // Constants
    static constexpr int DEFAULT_TIME_LIMIT = 30;  // Time limit per turn in seconds

    // Game state - Player numbers and feedback
    std::string player1Number;      // Player 1's secret number
    std::string player2Number;      // Player 2's secret number
    std::string feedbackMessage;    // Current feedback message
    std::vector<std::string> feedbackHistory;  // History of all guesses and feedback

    // Game state - Counters and limits
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