#include "GameUI.h"

// Define static constants
const char* GameUI::WINDOW_TITLE = "NumBrainer";

// Constructor initializes the UI
GameUI::GameUI() : shouldClose(false) {}

// Initialize the game window and settings
void GameUI::initialize() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);
}

// Clean up resources
void GameUI::shutdown() {
    CloseWindow();
}

// Main game loop
void GameUI::run() {
    while (!WindowShouldClose() && !shouldClose) {
        processInput();
        render();
    }
}

// Process user input from the keyboard
void GameUI::processInput() {
    if (IsKeyPressed(KEY_R)) { // Reset game
        game.reset();
        currentInput.clear();
        return;
    }

    int key = GetCharPressed(); // Handle number key presses
    handleKeyPress(key);

    if (IsKeyPressed(KEY_BACKSPACE) && !currentInput.empty()) { // Backspace handling
        currentInput.pop_back();
    }

    if (IsKeyPressed(KEY_ENTER) && !currentInput.empty()) { // Submit input
        if (game.getGameState() == GameState::SettingTurnLimit) {
            game.setTurnLimit(std::stoi(currentInput));
        } else if (game.getGameState() == GameState::SettingPlayerNumbers) {
            game.setPlayerNumber(currentInput);
        } else if (game.getGameState() == GameState::Playing) {
            game.makeGuess(currentInput);
        }
        currentInput.clear();
    }
}

// Handle numeric key presses
void GameUI::handleKeyPress(int key) {
    if (key >= '0' && key <= '9') {
        if (game.getGameState() == GameState::SettingTurnLimit && currentInput.length() < 2) {
            currentInput += (char)key;
        } else if (currentInput.length() < 4) {
            currentInput += (char)key;
        }
    }
}

// Render the game UI
void GameUI::render() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    if (!game.isGameOver()) {
        drawGameScreen();
    } else {
        drawGameStatistics();
    }

    EndDrawing();
}

// Draw the main game screen
void GameUI::drawGameScreen() {
    DrawText("NumBrainer", 250, 50, 40, DARKBLUE);

    if (game.getGameState() == GameState::SettingTurnLimit) {
        DrawText("Enter the number of turns for this round:", 100, 100, 20, DARKGRAY);
        DrawText(currentInput.c_str(), 400, 140, 25, DARKBLUE);
    } else {
        DrawText("Enter a 4-digit number with no repeating digits:", 100, 100, 20, DARKGRAY);

        if (game.getGameState() == GameState::SettingPlayerNumbers) {
            DrawText((game.isPlayer1Turn() ? "Player 1, set your number:" : "Player 2, set your number:"),
                     100, 140, 20, DARKGRAY);
            std::string maskedInput(currentInput.length(), '*');
            DrawText(maskedInput.c_str(), 400, 180, 25, DARKBLUE);
        } else {
            DrawText((game.isPlayer1Turn() ? "Player 1's turn to guess:" : "Player 2's turn to guess:"),
                     100, 140, 20, DARKGRAY);
            DrawText(currentInput.c_str(), 400, 180, 25, DARKBLUE);
        }
    }

    DrawText(game.getFeedbackMessage().c_str(), 100, 250, 20, MAROON);

    const auto& history = game.getFeedbackHistory();
    for (size_t i = 0; i < history.size(); i++) {
        Color entryColor = (i % 2 == 0) ? DARKBLUE : MAROON;
        std::string feedbackEntry = history[i].guess + " - " + history[i].message;
        DrawText(feedbackEntry.c_str(), 50, 300 + i * 20, 15, entryColor);
    }
}

// Draw the game statistics screen
void GameUI::drawGameStatistics() {
    int centerX = SCREEN_WIDTH / 2;
    int startY = 150;
    int lineSpacing = 50;

    DrawText("NumBrainer", 250, 50, 40, DARKBLUE);
    DrawText("Game Statistics", centerX - MeasureText("Game Statistics", 40) / 2, startY, 40, DARKBLUE);

    std::string p1Text = "Player 1 Turns: " + std::to_string(game.getPlayer1Turns()) + "/" +
                         std::to_string(game.getTurnLimit());
    DrawText(p1Text.c_str(), centerX - MeasureText(p1Text.c_str(), 30) / 2,
             startY + lineSpacing, 30, DARKBLUE);

    std::string p2Text = "Player 2 Turns: " + std::to_string(game.getPlayer2Turns()) + "/" +
                         std::to_string(game.getTurnLimit());
    DrawText(p2Text.c_str(), centerX - MeasureText(p2Text.c_str(), 30) / 2,
             startY + lineSpacing * 2, 30, MAROON);

    const std::string& feedbackMessage = game.getFeedbackMessage();
    std::string resultText;
    Color resultColor;

    if (feedbackMessage.find("Player 1 wins") != std::string::npos) {
        resultText = "Player 1 wins!";
        resultColor = DARKBLUE;
    } else if (feedbackMessage.find("Player 2 wins") != std::string::npos) {
        resultText = "Player 2 wins!";
        resultColor = MAROON;
    } else {
        resultText = "Draw Game";
        resultColor = PURPLE;
    }

    DrawText(resultText.c_str(), centerX - MeasureText(resultText.c_str(), 30) / 2,
             startY + lineSpacing * 3, 30, resultColor);

    std::string resetText = "Press 'R' to reset the game.";
    DrawText(resetText.c_str(), centerX - MeasureText(resetText.c_str(), 20) / 2,
             startY + lineSpacing * 4, 20, GRAY);
}
