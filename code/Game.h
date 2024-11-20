#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>
#include <set>

// Enum for representing the game state
enum class GameState {
    SettingTurnLimit,
    SettingPlayerNumbers,
    Playing,
    GameOver
};

// Struct for storing feedback information
struct Feedback {
    std::string guess;
    int correctDigits;
    int correctPositions;
    std::string message;
};

class Game {
public:
    Game();
    void reset();
    void setTurnLimit(int limit);
    void setPlayerNumber(const std::string& number);
    std::pair<int, int> makeGuess(const std::string& guess);

    // Getters
    bool isGameOver() const;
    GameState getGameState() const;
    bool isPlayer1Turn() const;
    int getPlayer1Turns() const;
    int getPlayer2Turns() const;
    int getTurnLimit() const;
    const std::string& getFeedbackMessage() const;
    const std::vector<Feedback>& getFeedbackHistory() const;

private:
    // Helper methods
    int countCorrectDigits(const std::string& guess, const std::string& target) const;
    int countCorrectPositions(const std::string& guess, const std::string& target) const;
    void switchTurns();
    void checkGameOver(const std::string& guess, const std::string& target);
    bool isValidNumber(const std::string& number) const;

    // Member variables
    std::string player1Number;
    std::string player2Number;
    std::string feedbackMessage;
    std::vector<Feedback> feedbackHistory;
    int turnLimit;
    int player1Turns;
    int player2Turns;
    bool player1Turn;
    GameState gameState;
};

#endif // GAME_H
