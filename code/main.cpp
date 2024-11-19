#include "raylib.h"
#include "GameState.h"
#include "GameRenderer.h"
#include <string>

int main() {
    InitWindow(800, 600, "NumBrainer");
    SetTargetFPS(60);

    GameState gameState;
    GameRenderer renderer(gameState);
    std::string currentInput;
    double startTime = GetTime();

    while (!WindowShouldClose()) {
        // Handle input
        if (gameState.isGameOver() && IsKeyPressed(KEY_R)) {
            gameState.reset();
            currentInput.clear();
            startTime = GetTime();
            continue;
        }

        int key = GetCharPressed();
        if (key >= '0' && key <= '9' && currentInput.length() < 4) {
            currentInput += static_cast<char>(key);
        }

        if (IsKeyPressed(KEY_BACKSPACE) && !currentInput.empty()) {
            currentInput.pop_back();
        }

        if (IsKeyPressed(KEY_ENTER) && !currentInput.empty()) {
            if (gameState.isSettingTurnLimit()) {
                gameState.setTurnLimit(currentInput);
                currentInput.clear();
            }
            else if (gameState.isSettingTimeLimit()) {
                gameState.setTimeLimit(currentInput);
                currentInput.clear();
                startTime = GetTime();
            }
            else if (gameState.isSettingUp()) {
                if (gameState.setPlayerNumber(currentInput)) {
                    currentInput.clear();
                }
            }
            else {
                gameState.processGuess(currentInput);
                currentInput.clear();
                startTime = GetTime();
            }
        }

        // Update game state
        if (gameState.isGameplayPhase()) {
            gameState.updateRemainingTime(GetTime() - startTime);
        }

        // Render
        renderer.render(currentInput);
    }

    CloseWindow();
    return 0;
}