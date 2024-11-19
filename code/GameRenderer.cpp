#include "GameRenderer.h"

void GameRenderer::render(const std::string& currentInput) const {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    drawTitle();

    if (gameState.isGameOver()) {
        drawGameOver();
    } else if (gameState.isSettingTurnLimit() || gameState.isSettingTimeLimit()) {
        drawSetupPhase(currentInput);
    } else {
        drawGamePlay(currentInput);
    }

    drawFeedbackHistory();

    EndDrawing();
}

void GameRenderer::drawTitle() const {
    DrawText("NumBrainer", 250, 50, 40, DARKBLUE);
}

void GameRenderer::drawSetupPhase(const std::string& currentInput) const {
    if (gameState.isSettingTurnLimit()) {
        DrawText("Enter the number of turns for this round:", 100, 100, 20, DARKGRAY);
    } else if (gameState.isSettingTimeLimit()) {
        DrawText("Enter time limit per turn (seconds):", 100, 100, 20, DARKGRAY);
    }
    DrawText(currentInput.c_str(), 400, 140, 25, DARKBLUE);
}

void GameRenderer::drawGamePlay(const std::string& currentInput) const {
    DrawText("Enter a 4-digit number with no repeating digits:", 100, 100, 20, DARKGRAY);

    if (gameState.isSettingUp()) {
        DrawText((gameState.isPlayer1Turn() ? 
                 "Player 1, set your number:" : 
                 "Player 2, set your number:"), 100, 140, 20, DARKGRAY);
        std::string maskedInput(currentInput.length(), '*');
        DrawText(maskedInput.c_str(), 400, 140, 25, DARKBLUE);
    } else {
        DrawText((gameState.isPlayer1Turn() ? 
                 "Player 1's turn to guess:" : 
                 "Player 2's turn to guess:"), 100, 140, 20, DARKGRAY);
        DrawText(currentInput.c_str(), 400, 140, 25, DARKBLUE);

        std::string timeText = "Time left: " + std::to_string(gameState.getRemainingTime());
        DrawText(timeText.c_str(), 100, 200, 20, RED);
    }
}

void GameRenderer::drawGameOver() const {
    const int centerX = GetScreenWidth() / 2;
    const int startY = 200;
    const int lineSpacing = 40;

    std::string winnerText;
    if (gameState.getPlayer1Turns() == gameState.getPlayer2Turns()) {
        winnerText = "It's a draw!";
    } else {
        winnerText = (gameState.getPlayer1Turns() < gameState.getPlayer2Turns() ? 
                     "Player 1 wins!" : "Player 2 wins!");
    }

    DrawText(winnerText.c_str(),
             centerX - MeasureText(winnerText.c_str(), 40)/2,
             startY, 40, DARKBLUE);

    std::string statsText = "Turns taken - P1: " + 
                           std::to_string(gameState.getPlayer1Turns()) +
                           " P2: " + std::to_string(gameState.getPlayer2Turns());
    DrawText(statsText.c_str(),
             centerX - MeasureText(statsText.c_str(), 20)/2,
             startY + lineSpacing, 20, DARKGRAY);

    DrawText("Press 'R' to play again",
             centerX - MeasureText("Press 'R' to play again", 20)/2,
             startY + lineSpacing*2, 20, DARKGRAY);
}

void GameRenderer::drawFeedbackHistory() const {
    const auto& history = gameState.getFeedbackHistory();
    int yPos = 350;
    for (const auto& feedback : history) {
        Color color = (feedback.find("Player 1") != std::string::npos) ? DARKBLUE : MAROON;
        DrawText(feedback.c_str(), 100, yPos, 20, color);
        yPos += 25;
    }
}