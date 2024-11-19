#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <string>
#include <vector>

class GameState {
public:
    GameState();

    // Getters
    [[nodiscard]] bool isGameOver() const { return gameOver; }
    [[nodiscard]] bool isPlayer1Turn() const { return player1Turn; }
    [[nodiscard]] bool isSettingUp() const { return settingUp; }
    [[nodiscard]] bool isSettingTurnLimit() const { return settingTurnLimit; }
    [[nodiscard]] bool isSettingTimeLimit() const { return settingTimeLimit; }
    [[nodiscard]] int getRemainingTime() const { return remainingTime; }
    [[nodiscard]] int getPlayer1Turns() const { return player1Turns; }
    [[nodiscard]] int getPlayer2Turns() const { return player2Turns; }
    [[nodiscard]] int getTurnLimit() const { return turnLimit; }
    [[nodiscard]] int getTimeLimit() const { return timeLimitPerTurn; }
    [[nodiscard]] const std::vector<std::string>& getFeedbackHistory() const { return feedbackHistory; }

    // State checks
    [[nodiscard]] bool isInputPhase() const { return !gameOver && (settingTurnLimit || settingTimeLimit || settingUp); }
    [[nodiscard]] bool isGameplayPhase() const { return !gameOver && !settingTurnLimit && !settingTimeLimit && !settingUp; }

    // Setters and game state updates
    [[nodiscard]] std::string validateNumber(const std::string& number) const;
    bool setPlayerNumber(const std::string& number);
    void setTurnLimit(const std::string& limit);
    void setTimeLimit(const std::string& limit);
    void processGuess(const std::string& guess);
    void updateRemainingTime(double currentTime);
    void reset();

private:
    std::string player1Number;
    std::string player2Number;
    int player1Turns;
    int player2Turns;
    int turnLimit;
    int timeLimitPerTurn;
    int remainingTime;
    bool player1Turn;
    bool gameOver;
    bool settingUp;
    bool settingTurnLimit;
    bool settingTimeLimit;
    std::vector<std::string> feedbackHistory;

    void switchTurns();
    [[nodiscard]] std::pair<int, int> evaluateGuess(const std::string& guess, const std::string& target) const;
    void addFeedback(const std::string& message);
};

#endif //GAMESTATE_H