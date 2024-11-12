#include "raylib.h"
#include <string>
#include <set>
#include <cstdlib>
#include <ctime>

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

// Function to validate the number input (4 digits, no repeating digits)
bool isValidNumber(const string& number) {
    if (number.length() != 4) return false;
    set<char> digits;
    for (char ch : number) {
        if (!isdigit(ch) || digits.find(ch) != end(digits)) {
            return false;
        }
        digits.insert(ch);
    }
    return true;
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
    string timeLimitInput = "";  // Input for time limit per turn
    int turnLimit = 0;           // Number of turns for the round
    int timeLimitPerTurn = 0;    // Time limit in seconds per turn
    int player1Turns = 0, player2Turns = 0;
    int correctDigits = 0, correctPositions = 0;
    bool gameOver = false;
    bool player1Turn = true;
    bool settingUp = true;
    bool settingTurnLimit = true;  // Variable to control turn limit setting
    bool settingTimeLimit = false; // Variable to control time limit setting

    // Time limit feature variables
    int remainingTime = 0;  // Time left for the current player's turn
    double startTime = 0;   // To track elapsed time per turn

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
                settingTimeLimit = true;  // Move to time limit setting
                feedbackMessage = "Enter time limit per turn (seconds):";
            }
        }
        else if (settingTimeLimit && !gameOver) {
            int key = GetCharPressed();
            if (key >= '0' && key <= '9' && timeLimitInput.length() < 2) {
                timeLimitInput += (char)key;
            }
            if (IsKeyPressed(KEY_BACKSPACE) && !timeLimitInput.empty()) {
                timeLimitInput.pop_back();
            }
            if (IsKeyPressed(KEY_ENTER) && !timeLimitInput.empty()) {
                timeLimitPerTurn = stoi(timeLimitInput);
                settingTimeLimit = false;
                feedbackMessage = "Player 1, set your 4-digit number.";
            }
        }
        else if ((settingUp || !gameOver) && !settingTurnLimit && !settingTimeLimit) {
            int key = GetCharPressed();
            if (key >= '0' && key <= '9' && guess.length() < 4) {
                guess += (char)key;
            }

            if (IsKeyPressed(KEY_BACKSPACE) && !guess.empty()) {
                guess.pop_back();
            }

            // After 4-digit entry, validate and store as either Player's target or guess
            if (IsKeyPressed(KEY_ENTER) && guess.length() == 4 && isValidNumber(guess)) {
                if (settingUp) {
                    // Set up phase for target numbers
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
                        remainingTime = timeLimitPerTurn;  // Set initial turn time
                        startTime = GetTime();             // Start timer only after setup
                    }
                    guess.clear();
                }
                else {
                    // Game loop for guessing phase
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
                            startTime = GetTime();  // Reset timer on turn switch
                        }

                        // Check if both players have reached the turn limit
                        if (player1Turns >= turnLimit && player2Turns >= turnLimit) {
                            feedbackMessage = "Turn limit reached! It's a draw.";
                            gameOver = true;
                        }
                    }

                    guess.clear();
                }
            }

            // Countdown timer for each turn
            if (!gameOver && !settingTurnLimit && !settingTimeLimit && !settingUp) {
                remainingTime = timeLimitPerTurn - (GetTime() - startTime);  // Update remaining time
                if (remainingTime <= 0) {
                    // Switch turns automatically if time runs out
                    feedbackMessage = (player1Turn ? "Player 1" : "Player 2") + string(" ran out of time!");
                    player1Turn = !player1Turn;
                    startTime = GetTime();  // Reset timer for next player
                    remainingTime = timeLimitPerTurn;
                    if (player1Turn) player1Turns++; else player2Turns++;
                }
            }
        }

        // Drawing the UI
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Title and instructions
        DrawText("NumBrainer", 250, 50, 40, DARKBLUE);

        // Setting the amount of turns per game
        if (settingTurnLimit) {
            DrawText("Enter the number of turns for this round:", 100, 100, 20, DARKGRAY);
            DrawText(turnLimitInput.c_str(), 400, 140, 25, DARKBLUE);
        }
        else if (settingTimeLimit) {
            DrawText("Enter time limit per turn (seconds):", 100, 100, 20, DARKGRAY);
            DrawText(timeLimitInput.c_str(), 400, 140, 25, DARKBLUE);
        }
        else {
            DrawText("Enter a 4-digit number with no repeating digits:", 100, 100, 20, DARKGRAY);

            if (settingUp) {
                DrawText((player1Turn ? "Player 1, set your number:" : "Player 2, set your number:"), 100, 140, 20, DARKGRAY);
            }
            else if (!gameOver) {
                DrawText((player1Turn ? "Player 1's turn to guess:" : "Player 2's turn to guess:"), 100, 140, 20, DARKGRAY);
                DrawText(("Time left: " + to_string(remainingTime)).c_str(), 100, 200, 20, RED);  // Display remaining time
            }
            else {
                DrawText("Game Over!", 100, 140, 20, DARKPURPLE);
            }

            // Display player input and feedback message
            if (settingUp) {
                string maskedGuess(guess.length(), '*');
                DrawText(maskedGuess.c_str(), 400, 140, 25, DARKBLUE);
            }
            else {
                DrawText(guess.c_str(), 400, 140, 25, DARKBLUE);
            }

            DrawText(feedbackMessage.c_str(), 100, 300, 20, MAROON);
        }

        // Instructions for resetting the game
        if (gameOver) {
            DrawText("Press 'R' to reset the game.", 100, 400, 20, GRAY);
        }

        // Reset game state
        if (IsKeyPressed(KEY_R) && gameOver) {
            player1Number.clear();
            player2Number.clear();
            guess.clear();
            feedbackMessage = "Enter the number of turns for this game.";
            player1Turn = true;
            settingUp = true;
            settingTurnLimit = true;
            settingTimeLimit = false;
            gameOver = false;
            player1Turns = 0;
            player2Turns = 0;
            turnLimitInput.clear();
            timeLimitInput.clear();
            remainingTime = 0;
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
