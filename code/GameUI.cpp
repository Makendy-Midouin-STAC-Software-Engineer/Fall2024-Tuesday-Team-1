#include "GameUI.h"

// Initialize the game window and set frame rate
GameUI::GameUI() {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "NumBrainer");
    SetTargetFPS(60);
}

// Main game loop
void GameUI::run() {
    while (!WindowShouldClose()) {
        processInput();
        render();
    }
    CloseWindow();
}

// Handle all user input
void GameUI::processInput() {
    // Handle game reset
    if (IsKeyPressed(KEY_R) && game.isGameOver()) {
        game.reset();
        currentInput.clear();
        return;
    }

    // Handle numeric input (only accept up to 4 digits)
    int key = GetCharPressed();
    if (key >= '0' && key <= '9' && currentInput.length() < 4) {
        currentInput += (char)key;
    }

    // Handle backspace for corrections
    if (IsKeyPressed(KEY_BACKSPACE) && !currentInput.empty()) {
        currentInput.pop_back();
    }

    // Return if no enter press or empty input
    if (!IsKeyPressed(KEY_ENTER) || currentInput.empty()) {
        return;
    }

    // Process input based on current game state
    if (game.isSettingTurnLimit()) {
        if (game.setTurnLimit(currentInput)) {
            currentInput.clear();
        }
    }
    else if (game.isSettingUp()) {
        if (game.setPlayerNumber(currentInput)) {
            currentInput.clear();
        }
    }
    else if (!game.isGameOver()) {
        if (game.makeGuess(currentInput)) {
            currentInput.clear();
        }
    }
}

// Main render function
void GameUI::render() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    if (!game.isGameOver()) {
        drawGameSetup();
    }
    else {
        drawGameStatistics();
    }

    EndDrawing();
}

// Draw the game setup and play screen
void GameUI::drawGameSetup() {
    // Draw title
    DrawText("NumBrainer", 250, 50, TITLE_FONT_SIZE, DARKBLUE);

    if (game.isSettingTurnLimit()) {
        // Draw turn limit input screen
        DrawText("Enter the number of turns for this round:", 100, 100, NORMAL_FONT_SIZE, DARKGRAY);
        DrawText(currentInput.c_str(), 500, 140, INPUT_FONT_SIZE, DARKBLUE);
    }
    else {
        // Draw main game screen
        DrawText("Enter a 4-digit number with no repeating digits:", 100, 100, NORMAL_FONT_SIZE, DARKGRAY);

        if (game.isSettingUp()) {
            // Draw number setup screen (mask the input)
            DrawText((game.isPlayer1Turn() ? "Player 1, set your number:" : "Player 2, set your number:"),
                    100, 140, NORMAL_FONT_SIZE, DARKGRAY);
            std::string maskedInput(currentInput.length(), '*');
            DrawText(maskedInput.c_str(), 400, 140, INPUT_FONT_SIZE, DARKBLUE);
        }
        else {
            // Draw gameplay screen
            DrawText((game.isPlayer1Turn() ? "Player 1's turn to guess:" : "Player 2's turn to guess:"),
                    100, 140, NORMAL_FONT_SIZE, DARKGRAY);
            DrawText(currentInput.c_str(), 400, 140, INPUT_FONT_SIZE, DARKBLUE);
            DrawText(("Time left: " + std::to_string(game.getRemainingTime())).c_str(),
                    100, 200, NORMAL_FONT_SIZE, RED);
        }

        // Draw feedback history
        int yOffset = 350;
        for (const std::string& feedback : game.getFeedbackHistory()) {
            Color feedbackColor = feedback.find("Player 1") != std::string::npos ? DARKBLUE : MAROON;
            DrawRectangle(90, yOffset - 5, 620, 30, LIGHTGRAY);
            DrawText(feedback.c_str(), 100, yOffset, NORMAL_FONT_SIZE, feedbackColor);
            yOffset += 35;
        }
    }

    // Draw current feedback message
    DrawText(game.getFeedbackMessage().c_str(), 100, 300, NORMAL_FONT_SIZE, MAROON);
}

// Draw the game over statistics screen
void GameUI::drawGameStatistics() {
    int centerX = WINDOW_WIDTH / 2;
    int startY = 150;
    int lineSpacing = 50;

    // Draw title and header
    DrawText("NumBrainer", centerX - MeasureText("NumBrainer", 40) / 2, 50, 40, DARKBLUE);
    DrawText("Game Statistics", centerX - MeasureText("Game Statistics", 40) / 2, startY, 40, DARKBLUE);

    // Display player 1's turn count
    std::string p1Text = "Player 1 Turns: " + std::to_string(game.getPlayer1Turns()) + "/" +
                        std::to_string(game.getTurnLimit());
    DrawText(p1Text.c_str(), centerX - MeasureText(p1Text.c_str(), 30) / 2,
            startY + lineSpacing, 30, DARKBLUE);

    // Display player 2's turn count
    std::string p2Text = "Player 2 Turns: " + std::to_string(game.getPlayer2Turns()) + "/" +
                        std::to_string(game.getTurnLimit());
    DrawText(p2Text.c_str(), centerX - MeasureText(p2Text.c_str(), 30) / 2,
            startY + lineSpacing * 2, 30, MAROON);

    // Determine and display the game result
    std::string resultText;
    Color resultColor;
    const std::string& feedback = game.getFeedbackMessage();

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

    // Draw the result
    DrawText(resultText.c_str(), centerX - MeasureText(resultText.c_str(), 30) / 2,
            startY + lineSpacing * 3, 30, resultColor);

    // Draw reset instruction
    std::string resetText = "Press 'R' to reset the game.";
    DrawText(resetText.c_str(), centerX - MeasureText(resetText.c_str(), 20) / 2,
            startY + lineSpacing * 4, 20, GRAY);
}