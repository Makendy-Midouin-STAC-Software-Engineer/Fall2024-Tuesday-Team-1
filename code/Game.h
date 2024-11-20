#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>
#include <set>

class Game {
public:
    Game();
    void reset();
    void setTurnLimit(int limit);
    bool isValidNumber(const std::string& number) const;
    void setPlayerNumber(const std::string& number);
    std::pair<int, int> makeGuess(const std::string& guess);
    bool isGameOver() const;
    bool isSettingUp() const;
    bool isSettingTurnLimit() const;
    bool isPlayer1Turn() const;
    int getPlayer1Turns() const;
    int getPlayer2Turns() const;
    int getTurnLimit() const;
    const std::string& getFeedbackMessage() const;
    const std::vector<std::string>& getFeedbackHistory() const;

private:
    int countCorrectDigits(const std::string& guess, const std::string& target) const;
    int countCorrectPositions(const std::string& guess, const std::string& target) const;
    void switchTurns();
    void checkGameOver(const std::string& guess, const std::string& target);

    std::string player1Number;
    std::string player2Number;
    std::string feedbackMessage;
    std::vector<std::string> feedbackHistory;
    int turnLimit;
    int player1Turns;
    int player2Turns;
    bool gameOver;
    bool player1Turn;
    bool settingUp;
    bool settingTurnLimit;
};

#endif // GAME_H