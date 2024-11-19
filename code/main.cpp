#include "raylib.h"
#include <string>
#include <set>
#include <vector>

using namespace std;

// Function to check how many digits are correct
int countCorrectDigits(const string& guess, const string& target) {
    int correctCount = 0;
    for (char ch : guess) {
        if (target.find(ch) != string::npos) {
            correctCount++;
        }
    }
    return correctCount;
}

// Function to check how many digits are in the correct position
int countCorrectPositions(const string& guess, const string& target) {
    int correctPosCount = 0;
    for (int i = 0; i < guess.length(); i++) {
        if (guess[i] == target[i]) {
            correctPosCount++;
        }
    }
    return correctPosCount;
}

// Function to validate the number input (4 digits, no repeating digits) with detailed feedback
string isValidNumber(const string& number) {
    if (number.length() != 4) {
        return "Error: Number must be exactly 4 digits long.";
    }

    set<char> digits;
    for (char ch : number) {
        if (!isdigit(ch)) {
            return "Error: Only numeric digits (0-9) are allowed.";
        }
        if (digits.find(ch) != digits.end()) {
            return "Error: Digits must not repeat.";
        }
        digits.insert(ch);
    }

    return "Valid";  // If all checks pass, return "Valid"
}

// Display end-game statistics centered on screen with game result
void DrawGameStatistics(int player1Turns, int player2Turns, bool gameOver, const string& feedbackMessage, int turnLimit) {
    if (!gameOver) return;

    int screenWidth = GetScreenWidth();
    int centerX = screenWidth / 2;
    int startY = 150;  // Start statistics lower to accommodate fixed title
    int lineSpacing = 50;  // Space between lines

    // Title stays in original position
    DrawText("NumBrainer", 250, 50, 40, DARKBLUE);

    // Game Statistics header
    DrawText("Game Statistics", centerX - MeasureText("Game Statistics", 40) / 2, startY, 40, DARKBLUE);

    // Player 1 Turns
    string p1Text = "Player 1 Turns: " + to_string(player1Turns) + "/" + to_string(turnLimit);
    DrawText(p1Text.c_str(), centerX - MeasureText(p1Text.c_str(), 30) / 2, startY + lineSpacing, 30, DARKBLUE);

    // Player 2 Turns
    string p2Text = "Player 2 Turns: " + to_string(player2Turns) + "/" + to_string(turnLimit);
    DrawText(p2Text.c_str(), centerX - MeasureText(p2Text.c_str(), 30) / 2, startY + lineSpacing * 2, 30, MAROON);

    // Result text
    string resultText;
    Color resultColor;
    if (feedbackMessage.find("Player 1 wins") != string::npos) {
        resultText = "Player 1 wins!";
        resultColor = DARKBLUE;
    }
    else if (feedbackMessage.find("Player 2 wins") != string::npos) {
        resultText = "Player 2 wins!";
        resultColor = MAROON;
    }
    else {
        resultText = "Draw Game";
        resultColor = PURPLE;
    }
    DrawText(resultText.c_str(), centerX - MeasureText(resultText.c_str(), 30) / 2, startY + lineSpacing * 3, 30, resultColor);

    // Reset instruction
    string resetText = "Press 'R' to reset the game.";
    DrawText(resetText.c_str(), centerX - MeasureText(resetText.c_str(), 20) / 2, startY + lineSpacing * 4, 20, GRAY);
}

int main() {
    // Initialize Raylib window
    InitWindow(800, 600, "NumBrainer");
    SetTargetFPS(60);

    // Game variables
    string player1Number = "", player2Number = "";
    string guess = "";
    string feedbackMessage = "";
    string turnLimitInput = "";  // Input for number of turns
    int turnLimit = 0;           // Number of turns for the round
    int player1Turns = 0, player2Turns = 0;
    int correctDigits = 0, correctPositions = 0;
    bool gameOver = false;
    bool player1Turn = true;
    bool settingUp = true;
    bool settingTurnLimit = true;

    // History vector to store feedback messages
    vector<string> feedbackHistory;

    while (!WindowShouldClose()) {
        // Capture input for turn limit or game setup
        if (settingTurnLimit && !gameOver) {
            int key = GetCharPressed();
            if (key >= '0' && key <= '9' && turnLimitInput.length() < 2) {
                turnLimitInput += (char)key;
            }
            if (IsKeyPressed(KEY_BACKSPACE) && !turnLimitInput.empty()) {
                turnLimitInput.pop_back();
            }
            if (IsKeyPressed(KEY_ENTER) && !turnLimitInput.empty()) {
                turnLimit = stoi(turnLimitInput);
                settingTurnLimit = false;
                feedbackMessage = "Player 1, set your 4-digit number.";
            }
        }
        else if ((settingUp || !gameOver) && !settingTurnLimit) {
            int key = GetCharPressed();
            if (key >= '0' && key <= '9' && guess.length() < 4) {
                guess += (char)key;
            }

            if (IsKeyPressed(KEY_BACKSPACE) && !guess.empty()) {
                guess.pop_back();
            }

            // After 4-digit entry, validate and store as either Player's target or guess
            if (IsKeyPressed(KEY_ENTER)) {
                string validationMessage = isValidNumber(guess);
                if (validationMessage != "Valid") {
                    feedbackMessage = validationMessage;  // Show specific error message
                    guess.clear();  // Clear the invalid guess
                }
                else {
                    // Proceed with valid guess processing
                    if (settingUp) {
                        if (player1Turn) {
                            player1Number = guess;
                            player1Turn = false;
                            feedbackMessage = "Player 2, set your 4-digit number.";
                        }
                        else {
                            player2Number = guess;
                            settingUp = false;
                            player1Turn = true;
                            feedbackMessage = "Game starts! Player 1's turn to guess.";
                        }
                        guess.clear();
                    }
                    else {
                        string target = player1Turn ? player2Number : player1Number;
                        correctDigits = countCorrectDigits(guess, target);
                        correctPositions = countCorrectPositions(guess, target);

                        if (guess == target) {
                            feedbackMessage = (player1Turn ? "Player 1" : "Player 2") + string(" wins!");
                            gameOver = true;
                        }
                        else {
                            feedbackMessage = (player1Turn ? "Player 1" : "Player 2");
                            feedbackMessage += ": " + to_string(correctDigits) + " correct digits, " +
                                to_string(correctPositions) + " in position.";

                            if (player1Turn) {
                                player1Turns++;
                            }
                            else {
                                player2Turns++;
                            }

                            // Switch turns only if both players haven't exceeded their turn limits
                            if (player1Turns < turnLimit || player2Turns < turnLimit) {
                                player1Turn = !player1Turn;
                            }

                            // Check if both players have reached the turn limit
                            if (player1Turns >= turnLimit && player2Turns >= turnLimit) {
                                feedbackMessage = "Turn limit reached! It's a draw.";
                                gameOver = true;
                            }
                        }

                        // Add guess and feedback to history
                        feedbackHistory.push_back("Guess: " + guess + " - " + feedbackMessage);
                        guess.clear();
                    }
                }
            }
        }

        // Drawing the UI
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (!gameOver) {
            // Title and instructions
            DrawText("NumBrainer", 250, 50, 40, DARKBLUE);

            // Setting the amount of turns per game
            if (settingTurnLimit) {
                DrawText("Enter the number of turns for this round:", 100, 100, 20, DARKGRAY);
                DrawText(turnLimitInput.c_str(), 400, 140, 25, DARKBLUE);
            }
            else {
                DrawText("Enter a 4-digit number with no repeating digits:", 100, 100, 20, DARKGRAY);

                if (settingUp) {
                    DrawText((player1Turn ? "Player 1, set your number:" : "Player 2, set your number:"), 100, 140, 20, DARKGRAY);
                }
                else {
                    DrawText((player1Turn ? "Player 1's turn to guess:" : "Player 2's turn to guess:"), 100, 140, 20, DARKGRAY);
                }

                // Display player input and feedback message
                if (settingUp) {
                    string maskedGuess(guess.length(), '*');
                    DrawText(maskedGuess.c_str(), 400, 180, 25, DARKBLUE);
                }
                else {
                    DrawText(guess.c_str(), 400, 180, 25, DARKBLUE);
                }
            }

            // Display feedback message
            DrawText(feedbackMessage.c_str(), 100, 250, 20, MAROON);

            // Show feedback history with alternating colors
            for (int i = 0; i < feedbackHistory.size(); i++) {
                Color entryColor = (i % 2 == 0) ? DARKBLUE : MAROON; // Alternate colors
                DrawText(feedbackHistory[i].c_str(), 50, 300 + i * 20, 15, entryColor);
            }

        }
        else {
            // Display end game statistics
            DrawGameStatistics(player1Turns, player2Turns, gameOver, feedbackMessage, turnLimit);
        }

        EndDrawing();

        // Reset game on 'R'
        if (IsKeyPressed(KEY_R)) {
            gameOver = false;
            settingTurnLimit = true;
            settingUp = true;
            player1Turn = true;
            player1Number = "";
            player2Number = "";
            feedbackMessage = "";
            turnLimitInput = "";
            turnLimit = 0;
            player1Turns = 0;
            player2Turns = 0;
            feedbackHistory.clear();
        }
    }

    CloseWindow();
    return 0;
}
