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
        if (!isdigit(ch) || digits.find(ch) != digits.end()) {
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
    int turn = 1;
    int correctDigits = 0, correctPositions = 0;
    bool gameOver = false;
    bool player1Turn = true;
    bool settingUp = true;

    // Timer-related variables
    float turnTimeLimit = 0;   // Stores the number of seconds per turn, set by players
    float turnStartTime = 0;   // Records the start time of each turn
    bool timerSettingPhase = true;  // Indicates if we're in the phase to set the turn timer

    while (!WindowShouldClose()) {

        // Timer-setting phase
        if (timerSettingPhase) {
            int key = GetCharPressed();
            if (key >= '0' && key <= '9') {
                turnTimeLimit = turnTimeLimit * 10 + (key - '0'); // accumulate input as integer
            }
            if (IsKeyPressed(KEY_ENTER) && turnTimeLimit > 0) {
                timerSettingPhase = false;  // Move to setup phase after setting timer
                feedbackMessage = "Player 1, set your 4-digit number.";
            }
            if (IsKeyPressed(KEY_BACKSPACE)) {
                turnTimeLimit = turnTimeLimit / 10; // Remove last digit
            }
        }

        // Capture input for 4-digit numbers or guesses
        if ((settingUp || !gameOver) && !timerSettingPhase) {
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
                        turnStartTime = GetTime();  // Start the turn timer
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
                        player1Turn = !player1Turn;
                        turn++;
                        turnStartTime = GetTime();  // Reset timer at the start of each new turn
                    }

                    guess.clear();
                }
            }
        }

        // Timer countdown logic to automatically start the next turn on timeout
        if (!settingUp && !gameOver && !timerSettingPhase) {
            float elapsedTime = GetTime() - turnStartTime;
            if (elapsedTime > turnTimeLimit) {
                // Player ran out of time; switch turns automatically
                feedbackMessage = (player1Turn ? "Player 1" : "Player 2") + string(" ran out of time!");
                player1Turn = !player1Turn;
                turnStartTime = GetTime();  // Reset timer for the new turn
                turn++;
            }
        }

        // Drawing the UI
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Title and instructions
        DrawText("NumBrainer", 250, 50, 40, DARKBLUE);
        DrawText("Enter a 4-digit number with no repeating digits:", 100, 100, 20, DARKGRAY);

        if (timerSettingPhase) { // UI for timer setting phase
            DrawText("Set seconds per turn:", 100, 140, 20, DARKGRAY);
            DrawText(to_string((int)turnTimeLimit).c_str(), 400, 140, 25, DARKBLUE);
        }
        else if (settingUp) {
            DrawText((player1Turn ? "Player 1, set your number:" : "Player 2, set your number:"), 100, 140, 20, DARKGRAY);
        }
        else if (!gameOver) {
            DrawText((player1Turn ? "Player 1's turn to guess:" : "Player 2's turn to guess:"), 100, 140, 20, DARKGRAY);

            // Display remaining time for the current turn
            int remainingTime = (int)(turnTimeLimit - (GetTime() - turnStartTime));
            DrawText(("Time left: " + to_string(remainingTime) + "s").c_str(), 100, 180, 20, RED);
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

        // Instructions for resetting the game
        if (gameOver) {
            DrawText("Press 'R' to reset the game.", 100, 400, 20, GRAY);
        }

        // Reset game state
        if (IsKeyPressed(KEY_R) && gameOver) {
            player1Number.clear();
            player2Number.clear();
            guess.clear();
            feedbackMessage = "Player 1, set your 4-digit number.";
            player1Turn = true;
            settingUp = true;
            gameOver = false;
            turn = 1;
            timerSettingPhase = true; // Reset timer setting phase
            turnTimeLimit = 0;
            turnStartTime = 0;
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
