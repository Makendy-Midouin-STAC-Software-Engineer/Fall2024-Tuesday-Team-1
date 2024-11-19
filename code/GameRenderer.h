#ifndef GAMERENDERER_H
#define GAMERENDERER_H

#include "raylib.h"
#include "GameState.h"
#include <string>

class GameRenderer {
public:
    GameRenderer(const GameState& state) : gameState(state) {}
    void render(const std::string& currentInput) const;

private:
    const GameState& gameState;

    void drawTitle() const;
    void drawSetupPhase(const std::string& currentInput) const;
    void drawGamePlay(const std::string& currentInput) const;
    void drawGameOver() const;
    void drawFeedbackHistory() const;
};

#endif //GAMERENDERER_H