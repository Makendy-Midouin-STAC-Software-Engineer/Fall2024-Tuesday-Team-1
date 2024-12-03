/**
 * @file GameUI.cpp
 * @brief Implementation of the game's user interface
 */

#include "GameUI.h"
#include <algorithm>
#include <cmath>

// Initialize window and UI settings
GameUI::GameUI() : timerAnimation(0.0f) {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "NumBrainer");
    SetTargetFPS(60);
}

// Main game loop
void GameUI::run() {
    while (!WindowShouldClose()) {
        // Update timer and animation
        if (!game.isGameOver() && !game.isSettingUp() && !game.isSettingTurnLimit()) {
            game.updateTimer(GetTime());

            // Update timer animation when time is low
            if (game.getRemainingTime() <= WARNING_TIME) {
                timerAnimation += GetFrameTime() * ANIMATION_SPEED;
            } else {
                timerAnimation = 0.0f;
            }
        }

        processInput();
        render();
    }
    CloseWindow();
}

// Process all user input
void GameUI::processInput() {
    // Handle game reset
    if (IsKeyPressed(KEY_R) && game.isGameOver()) {
        game.reset();
        currentInput.clear();
        return;
    }

    // Handle backspace
    if (IsKeyPressed(KEY_BACKSPACE) && !currentInput.empty()) {
        currentInput.pop_back();
        return;
    }

    // Handle numeric input
    int key = GetCharPressed();
    if (key >= '0' && key <= '9') {
        // For turn limit, allow up to 2 digits
        if (game.isSettingTurnLimit() && currentInput.length() < 2) {
            currentInput += (char)key;
        }
        // For game numbers/guesses, allow up to 4 non-repeating digits
        else if (!game.isSettingTurnLimit() && currentInput.length() < 4 &&
                 std::find(currentInput.begin(), currentInput.end(), (char)key) == currentInput.end()) {
            currentInput += (char)key;
        }
    }

    // Process enter key
    if (IsKeyPressed(KEY_ENTER) && !currentInput.empty()) {
        if (game.isSettingTurnLimit()) {
            if (game.setTurnLimit(currentInput)) currentInput.clear();
        }
        else if (game.isSettingUp()) {
            if (game.setPlayerNumber(currentInput)) currentInput.clear();
        }
        else if (!game.isGameOver()) {
            if (game.makeGuess(currentInput)) currentInput.clear();
        }
    }
}

// Main render function
void GameUI::render() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Draw title
    const char* title = "NumBrainer";
    int titleWidth = MeasureText(title, TITLE_FONT_SIZE);
    DrawText(title, (WINDOW_WIDTH - titleWidth) / 2, 50, TITLE_FONT_SIZE, DARKBLUE);

    // Draw appropriate game state
    if (game.isGameOver()) {
        drawGameStatistics();
    } else {
        drawGameState();
    }

    EndDrawing();
}

// Draw current game state
void GameUI::drawGameState() {
    if (game.isSettingTurnLimit()) {
        drawTurnLimitSetup();
    } else if (game.isSettingUp()) {
        drawNumberSetup();
    } else {
        drawGameplay();
    }

    // Draw feedback message in a box
    const std::string& feedback = game.getFeedbackMessage();
    if (!feedback.empty()) {
        int padding = 10;
        int boxWidth = MeasureText(feedback.c_str(), NORMAL_FONT_SIZE) + (padding * 2);
        int boxHeight = NORMAL_FONT_SIZE + (padding * 2);

        DrawRectangle((WINDOW_WIDTH - boxWidth) / 2, 300, boxWidth, boxHeight, LIGHTGRAY);
        DrawText(feedback.c_str(),
                (WINDOW_WIDTH - MeasureText(feedback.c_str(), NORMAL_FONT_SIZE)) / 2,
                300 + padding, NORMAL_FONT_SIZE, MAROON);
    }
}

// Draw turn limit setup screen
void GameUI::drawTurnLimitSetup() {
    const char* prompt = "Enter the number of turns for this round (1-99):";
    int promptWidth = MeasureText(prompt, NORMAL_FONT_SIZE);
    DrawText(prompt, (WINDOW_WIDTH - promptWidth) / 2, 150, NORMAL_FONT_SIZE, DARKGRAY);

    int inputWidth = MeasureText(currentInput.c_str(), INPUT_FONT_SIZE);
    DrawText(currentInput.c_str(), (WINDOW_WIDTH - inputWidth) / 2, 200, INPUT_FONT_SIZE, DARKBLUE);
}

// Draw number setup screen
void GameUI::drawNumberSetup() {
    const char* prompt = "Enter a 4-digit number with no repeating digits:";
    int promptWidth = MeasureText(prompt, NORMAL_FONT_SIZE);
    DrawText(prompt, (WINDOW_WIDTH - promptWidth) / 2, 150, NORMAL_FONT_SIZE, DARKGRAY);

    std::string playerPrompt = (game.isPlayer1Turn() ? "Player 1" : "Player 2") +
                              std::string(", set your number:");
    int playerPromptWidth = MeasureText(playerPrompt.c_str(), NORMAL_FONT_SIZE);
    DrawText(playerPrompt.c_str(), (WINDOW_WIDTH - playerPromptWidth) / 2, 200,
            NORMAL_FONT_SIZE, DARKGRAY);

    // Mask input with asterisks
    std::string maskedInput(currentInput.length(), '*');
    int inputWidth = MeasureText(maskedInput.c_str(), INPUT_FONT_SIZE);
    DrawText(maskedInput.c_str(), (WINDOW_WIDTH - inputWidth) / 2, 250, INPUT_FONT_SIZE, DARKBLUE);
}

// Draw animated timer
void GameUI::drawTimer() {
    std::string timeText = "Time left: " + std::to_string(game.getRemainingTime()) + "s";
    int timeWidth = MeasureText(timeText.c_str(), NORMAL_FONT_SIZE);

    // Calculate base position
    int baseX = (WINDOW_WIDTH - timeWidth) / 2;
    int baseY = 250;

    // Apply animation when time is low
    if (game.getRemainingTime() <= WARNING_TIME) {
        // Pulsing red color
        float alpha = (std::sin(timerAnimation) + 1.0f) * 0.5f;
        Color timerColor = ColorAlpha(RED, 0.5f + alpha * 0.5f);

        // Scale factor for size pulsing
        float scale = 1.0f + std::sin(timerAnimation * 0.5f) * 0.2f;

        // Adjust position for scaled text
        int scaledWidth = timeWidth * scale;
        int scaledX = (WINDOW_WIDTH - scaledWidth) / 2;

        // Draw warning background
        DrawRectangle(scaledX - 10, baseY - 5,
                     scaledWidth + 20, NORMAL_FONT_SIZE + 10,
                     ColorAlpha(RED, 0.2f));

        // Draw animated time text
        DrawTextEx(GetFontDefault(), timeText.c_str(),
                  (Vector2){(float)scaledX, (float)baseY},
                  NORMAL_FONT_SIZE * scale, 2, timerColor);
    } else {
        // Normal timer display
        DrawText(timeText.c_str(), baseX, baseY, NORMAL_FONT_SIZE, RED);
    }
}

// Draw main gameplay screen
void GameUI::drawGameplay() {
    // Draw current player's turn
    std::string turnText = (game.isPlayer1Turn() ? "Player 1" : "Player 2") +
                          std::string("'s turn to guess:");
    int turnWidth = MeasureText(turnText.c_str(), NORMAL_FONT_SIZE);
    DrawText(turnText.c_str(), (WINDOW_WIDTH - turnWidth) / 2, 150, NORMAL_FONT_SIZE, DARKGRAY);

    // Draw current input
    int inputWidth = MeasureText(currentInput.c_str(), INPUT_FONT_SIZE);
    DrawText(currentInput.c_str(), (WINDOW_WIDTH - inputWidth) / 2, 200, INPUT_FONT_SIZE, DARKBLUE);

    drawTimer();
    drawFeedbackHistory();
}

// Draw feedback history
void GameUI::drawFeedbackHistory() {
    const auto& history = game.getFeedbackHistory();
    int startIdx = (history.size() > MAX_HISTORY_ITEMS) ?
                   history.size() - MAX_HISTORY_ITEMS : 0;

    for (size_t i = startIdx; i < history.size(); i++) {
        const std::string& feedback = history[i];
        Color textColor = feedback.find("Player 1") != std::string::npos ? DARKBLUE : MAROON;

        int boxWidth = MeasureText(feedback.c_str(), NORMAL_FONT_SIZE) + 20;
        int boxX = (WINDOW_WIDTH - boxWidth) / 2;
        int boxY = FEEDBACK_HISTORY_START_Y + ((i - startIdx) * 35);

        DrawRectangle(boxX, boxY - 5, boxWidth, 30, Fade(LIGHTGRAY, 0.5f));
        DrawText(feedback.c_str(), boxX + 10, boxY, NORMAL_FONT_SIZE, textColor);
    }
}

// Draw game over statistics screen
void GameUI::drawGameStatistics() {
    int centerX = WINDOW_WIDTH / 2;
    int startY = 150;
    int lineSpacing = 50;

    // Draw statistics header
    const char* statsHeader = "Game Statistics";
    DrawText(statsHeader, centerX - MeasureText(statsHeader, 40) / 2, startY, 40, DARKBLUE);

    // Draw turn counts
    std::string p1Text = "Player 1 Turns: " + std::to_string(game.getPlayer1Turns()) + "/" +
                        std::to_string(game.getTurnLimit());
    DrawText(p1Text.c_str(), centerX - MeasureText(p1Text.c_str(), 30) / 2,
            startY + lineSpacing, 30, DARKBLUE);

    std::string p2Text = "Player 2 Turns: " + std::to_string(game.getPlayer2Turns()) + "/" +
                        std::to_string(game.getTurnLimit());
    DrawText(p2Text.c_str(), centerX - MeasureText(p2Text.c_str(), 30) / 2,
            startY + lineSpacing * 2, 30, MAROON);

    // Determine and draw game result
    const std::string& feedback = game.getFeedbackMessage();
    std::string resultText;
    Color resultColor;

    if (feedback.find("Player 1 wins") != std::string::npos) {
        resultText = "Player 1 wins!";
        resultColor = DARKBLUE;
    }
    else if (feedback.find("Player 2 wins") != std::string::npos) {
        resultText = "Player 2 wins!";
        resultColor = MAROON;
    }
    else {
        resultText = "Draw Game";
        resultColor = PURPLE;
    }

    DrawText(resultText.c_str(), centerX - MeasureText(resultText.c_str(), 30) / 2,
            startY + lineSpacing * 3, 30, resultColor);

    // Draw reset instruction
    const char* resetText = "Press 'R' to reset the game";
    DrawText(resetText, centerX - MeasureText(resetText, 20) / 2,
            startY + lineSpacing * 4, 20, GRAY);
}