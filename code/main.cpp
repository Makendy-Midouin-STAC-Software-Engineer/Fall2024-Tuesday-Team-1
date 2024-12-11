#if defined(_WIN32)
#define NOGDI
#define NOUSER
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "raylib.h"
#include <string>
#include <set>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

// Global color definitions
const Color BACKGROUND_COLOR = { 230, 230, 250, 255 };  // Lavender background
const Color PRIMARY_COLOR = { 0, 123, 255, 255 };       // Vibrant blue
const Color SECONDARY_COLOR = { 220, 53, 69, 255 };     // Bright red
const Color NEUTRAL_COLOR = { 108, 117, 125, 255 };     // Dark gray
const Color BUTTON_COLOR = { 40, 167, 69, 255 };        // Green
const Color BUTTON_HOVER_COLOR = { 30, 132, 56, 255 };  // Darker green
const Color INPUT_BG = { 248, 249, 250, 255 };          // Very light gray
const Color SUCCESS_COLOR = { 25, 135, 84, 255 };       // Deep green
const Color WARNING_COLOR = { 255, 193, 7, 255 };       // Bright yellow
const Color TIMER_WARNING = { 220, 20, 60, 255 };       // Crimson red for urgency

// Global button rectangles
static Rectangle resetButton = { 0, 0, 200, 40 };
static Rectangle menuButton = { 0, 0, 200, 40 };
static Rectangle resetBtn = { 0, 0, 100, 40 };
static Rectangle menuBtn = { 0, 0, 200, 40 };

// Function to check if a point is inside a rectangle
bool IsMouseOverButton(int mouseX, int mouseY, int x, int y, int width, int height) {
    return mouseX >= x && mouseX <= (x + width) && mouseY >= y && mouseY <= (y + height);
}

// Add fade alpha variable
//float fadeAlpha = 0.0f;

// Function to clamp a value between a minimum and maximum
float Clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

// Function to reset the game state
void ResetGame(bool& startScreen, string& player1Number, string& player2Number, string& guess,
    string& feedbackMessage, bool& player1Turn, bool& settingUp, bool& settingTurnLimit,
    bool& gameOver, vector<string>& feedbackHistory,
    int& player1Turns, int& player2Turns, string& turnLimitInput, int& remainingTime,
    string& player1Name, string& player2Name, bool& settingPlayer1Name, bool& settingPlayer2Name) {
    startScreen = true;
    player1Number.clear();
    player2Number.clear();
    guess.clear();
    feedbackMessage.clear();
    player1Turn = true;
    settingUp = true;
    settingTurnLimit = true;
    gameOver = false;
    feedbackHistory.clear();
    player1Turns = 0;
    player2Turns = 0;
    turnLimitInput.clear();
    remainingTime = 0;
    player1Name.clear();
    player2Name.clear();
    settingPlayer1Name = false;
    settingPlayer2Name = false;
}

// Function to count how many digits are correct
int countCorrectDigits(const string& guess, const string& target) {
    int correctCount = 0;
    for (char ch : guess) {
        if (target.find(ch) != string::npos) {
            correctCount++;
        }
    }
    return correctCount;
}

// Function to count how many digits are in the correct position
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

    return "Valid"; // If all checks pass, return "Valid"
}



// Function to display game statistics
void DrawGameStatistics(int player1Turns, int player2Turns, bool gameOver, const string& feedbackMessage, int turnLimit) {
    if (!gameOver) return;

    int screenWidth = GetScreenWidth();
    int centerX = screenWidth / 2;
    int startY = 150;
    int lineSpacing = 50;

    // Draw a card-like container
    DrawRectangleRounded({ (float)(centerX - 300), (float)(startY - 50), 600, 400 }, 0.02f, 8, BLACK);
    DrawRectangleRoundedLines({ (float)(centerX - 300), (float)(startY - 50), 600, 400 }, 0.02f, 8, 2, Fade(NEUTRAL_COLOR, 0.3f));

    // Title with shadow
    DrawText("NumBrainer", centerX - MeasureText("NumBrainer", 40) / 2 + 3, 53, 40, Fade(BLACK, 0.2f));
    DrawText("NumBrainer", centerX - MeasureText("NumBrainer", 40) / 2, 50, 40, PRIMARY_COLOR);

    // Game Statistics header
    DrawText("Game Statistics", centerX - MeasureText("Game Statistics", 40) / 2, startY, 40, PRIMARY_COLOR);

    // Player stats with modern styling
    string p1Text = "Player 1 Turns: " + to_string(player1Turns) + "/" + to_string(turnLimit);
    string p2Text = "Player 2 Turns: " + to_string(player2Turns) + "/" + to_string(turnLimit);

    // Player 1 stats box
    DrawRectangleRounded({ (float)(centerX - 200), (float)(startY + lineSpacing), 400, 40 }, 0.2f, 8, Fade(PRIMARY_COLOR, 0.1f));
    DrawText(p1Text.c_str(), centerX - MeasureText(p1Text.c_str(), 30) / 2, startY + lineSpacing + 5, 30, PRIMARY_COLOR);

    // Player 2 stats box
    DrawRectangleRounded({ (float)(centerX - 200), (float)(startY + lineSpacing * 2), 400, 40 }, 0.2f, 8, Fade(SECONDARY_COLOR, 0.1f));
    DrawText(p2Text.c_str(), centerX - MeasureText(p2Text.c_str(), 30) / 2, startY + lineSpacing * 2 + 5, 30, SECONDARY_COLOR);

    // Result text with appropriate styling
    string resultText;
    Color resultColor;
    if (feedbackMessage.find("Player 1 wins") != string::npos) {
        resultText = "Player 1 wins!";
        resultColor = PRIMARY_COLOR;
    }
    else if (feedbackMessage.find("Player 2 wins") != string::npos) {
        resultText = "Player 2 wins!";
        resultColor = SECONDARY_COLOR;
    }
    else {
        resultText = "Draw Game";
        resultColor = NEUTRAL_COLOR;
    }

    // Result box
    DrawRectangleRounded({ (float)(centerX - 200), (float)(startY + lineSpacing * 3), 400, 40 }, 0.2f, 8, Fade(resultColor, 0.1f));
    DrawText(resultText.c_str(), centerX - MeasureText(resultText.c_str(), 30) / 2, startY + lineSpacing * 3 + 5, 30, resultColor);

    // Modern buttons at the bottom
    float buttonY = startY + lineSpacing * 4;

    // Define button rectangles for click detection
    Rectangle resetButton = { (float)(centerX - 210), buttonY, 200, 40 };
    Rectangle menuButton = { (float)(centerX + 10), buttonY, 200, 40 };

    // Get mouse position for hover effects
    Vector2 mousePoint = { (float)GetMouseX(), (float)GetMouseY() };

    // Reset button with hover effect
    Color resetColor = CheckCollisionPointRec(mousePoint, resetButton) ? BUTTON_HOVER_COLOR : BUTTON_COLOR;
    DrawRectangleRounded(resetButton, 0.3f, 8, resetColor);
    DrawText("Reset (R)", centerX - 180, buttonY + 10, 20, WHITE);

    // Menu button with hover effect
    Color menuColor = CheckCollisionPointRec(mousePoint, menuButton) ? BUTTON_HOVER_COLOR : BUTTON_COLOR;
    DrawRectangleRounded(menuButton, 0.3f, 8, menuColor);
    DrawText("Menu (M)", centerX + 40, buttonY + 10, 20, WHITE);

    // Handle button clicks in main game loop
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(mousePoint, resetButton)) {
            // Trigger reset (same as pressing R)
            return;
        }
        if (CheckCollisionPointRec(mousePoint, menuButton)) {
            // Trigger menu return (same as pressing M)
            return;
        }
    }
}


void DrawModernInput(const char* label, const char* value, float x, float y, bool isActive) {
    // Draw label
    DrawText(label, x, y, 20, NEUTRAL_COLOR);

    // Input box
    Rectangle inputRect = { x, y + 30, 200, 40 };
    DrawRectangleRounded(inputRect, 0.2f, 8, Fade(INPUT_BG, 0.5f));

    // Bottom line (active indicator)
    if (isActive) {
        DrawRectangleRounded(
            { inputRect.x, inputRect.y + inputRect.height - 2, inputRect.width, 2 },
            1.0f, 1, PRIMARY_COLOR);
    }

    // Input text
    DrawText(value, x + 10, y + 40, 20, PRIMARY_COLOR);

    // Blinking cursor when active
    if (isActive && ((int)(GetTime() * 2) % 2)) {
        DrawText("_", x + 10 + MeasureText(value, 20), y + 40, 20, PRIMARY_COLOR);
    }
}

void DrawFeedbackMessage(const char* message, float x, float y) {
    Color messageColor = NEUTRAL_COLOR;
    const char* icon = ">";

    if (strstr(message, "Error")) {
        messageColor = WARNING_COLOR;
        icon = "!";
    }
    else if (strstr(message, "wins")) {
        messageColor = SUCCESS_COLOR;
        icon = "*";
    }

    DrawText(icon, x, y, 20, messageColor);
    DrawText(message, x + 30, y, 20, messageColor);
}

#if defined(_WIN32)
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
#else
int main(void)
#endif
{
    // Initialize Raylib window
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "NumBrainer");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);  // Disable default ESC key handling

    // Game variables
    string player1Number = "", player2Number = "";
    string guess = "";
    string feedbackMessage = "";
    string turnLimitInput = "";  // Input for number of turns
    int turnLimit = 0;           // Number of turns for the round
    int timeLimitPerTurn = 30;   // Set default time limit to 30 seconds
    int player1Turns = 0, player2Turns = 0;
    int correctDigits = 0, correctPositions = 0;
    bool gameOver = false;
    bool player1Turn = true;
    bool settingUp = true;
    bool settingTurnLimit = true;  // Variable to control turn limit setting
    bool startScreen = true;
    bool exitRequested = false;  // Add exit confirmation flag
    // Time limit feature variables
    int remainingTime = 0;  // Time left for the current player's turn
    double startTime = 0;   // To track elapsed time per turn

    // History vector to store feedback messages
    vector<string> feedbackHistory;

    // Button properties
    const int buttonWidth = 100;
    const int buttonHeight = 40;
    const int buttonX = GetScreenWidth() - buttonWidth - 20; // Top-right corner with padding
    const int buttonY = 20;

    // Add these near the top with other game variables
    string player1Name = "";
    string player2Name = "";
    bool settingPlayer1Name = false;
    bool settingPlayer2Name = false;

    while (!WindowShouldClose() || exitRequested)  // Modified condition to prevent immediate exit
    {
        Vector2 mousePoint = { (float)GetMouseX(), (float)GetMouseY() };

        // Handle exit confirmation
        if (IsKeyPressed(KEY_ESCAPE)) {
            exitRequested = true;
        }

        if (exitRequested) {
            // Define buttons first
            Rectangle yesBtn = { screenWidth/2 - 160, screenHeight/2 + 20, 140, 40 };
            Rectangle noBtn = { screenWidth/2 + 20, screenHeight/2 + 20, 140, 40 };

            // Handle input
            if (IsKeyPressed(KEY_Y) || (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && 
                CheckCollisionPointRec(mousePoint, yesBtn))) {
                break;  // Exit the game
            }
            else if (IsKeyPressed(KEY_N) || (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && 
                CheckCollisionPointRec(mousePoint, noBtn))) {
                exitRequested = false;
            }

            BeginDrawing();
            ClearBackground(BACKGROUND_COLOR);
            
            // Draw semi-transparent overlay (reduced opacity)
            DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.3f));
            
            // Draw confirmation dialog with modern styling
            Rectangle dialogBox = { screenWidth/2 - 200, screenHeight/2 - 100, 400, 200 };
            
            // Main dialog box with gradient effect
            DrawRectangleRounded(dialogBox, 0.02f, 8, WHITE);
            DrawRectangleRoundedLines(dialogBox, 0.02f, 8, 2, Fade(NEUTRAL_COLOR, 0.3f));
            
            // Title with shadow effect
            DrawText("Exit Game", 
                screenWidth/2 - MeasureText("Exit Game", 30)/2 + 2,
                screenHeight/2 - 80 + 2, 30, Fade(BLACK, 0.2f));  // Shadow
            DrawText("Exit Game", 
                screenWidth/2 - MeasureText("Exit Game", 30)/2,
                screenHeight/2 - 80, 30, PRIMARY_COLOR);
            
            // Question text
            DrawText("Are you sure you want to exit?",
                screenWidth/2 - MeasureText("Are you sure you want to exit?", 20)/2,
                screenHeight/2 - 30, 20, NEUTRAL_COLOR);
            
            // Yes button (with hover effect)
            Color yesColor = (IsKeyDown(KEY_Y) || CheckCollisionPointRec(mousePoint, yesBtn)) ? 
                BUTTON_HOVER_COLOR : BUTTON_COLOR;
            DrawRectangleRounded(yesBtn, 0.3f, 8, yesColor);
            DrawText("Yes (Y)",
                screenWidth/2 - 160 + (140 - MeasureText("Yes (Y)", 20))/2,
                screenHeight/2 + 30, 20, WHITE);
            
            // No button (with hover effect)
            Color noColor = (IsKeyDown(KEY_N) || CheckCollisionPointRec(mousePoint, noBtn)) ? 
                BUTTON_HOVER_COLOR : BUTTON_COLOR;
            DrawRectangleRounded(noBtn, 0.3f, 8, noColor);
            DrawText("No (N)",
                screenWidth/2 + 20 + (140 - MeasureText("No (N)", 20))/2,
                screenHeight/2 + 30, 20, WHITE);
            
            EndDrawing();
            continue;
        }

        // Handle input for turn limit or game setup
        if (settingTurnLimit && !gameOver) {
            int key = GetCharPressed();
            if (key >= '0' && key <= '9' && turnLimitInput.length() < 2) {
                turnLimitInput += (char)key;
            }
            if (IsKeyPressed(KEY_BACKSPACE) && !turnLimitInput.empty()) {
                turnLimitInput.pop_back();
            }
            if (IsKeyPressed(KEY_ENTER) && !turnLimitInput.empty()) {
                int tempTurnLimit = stoi(turnLimitInput);
                if (tempTurnLimit < 1) {
                    feedbackMessage = "Turn limit must be at least 1. Try again:";
                    turnLimitInput.clear(); // Clear invalid input
                }
                else {
                    turnLimit = tempTurnLimit;
                    settingTurnLimit = false;
                    feedbackMessage = "Player 1, set your 4-digit number.";
                    remainingTime = timeLimitPerTurn;  // Set initial turn time
                }
            }
        }
        else if (!settingTurnLimit && !gameOver) {
            int key = GetCharPressed();
            if (key >= '0' && key <= '9' && guess.length() < 4) {
                guess += (char)key;
            }
            if (IsKeyPressed(KEY_BACKSPACE) && !guess.empty()) {
                guess.pop_back();
            }
            // Add back the ENTER key handling for number setup and guessing
            if (IsKeyPressed(KEY_ENTER) && !guess.empty()) {
                string validationMessage = isValidNumber(guess);
                if (validationMessage != "Valid") {
                    feedbackMessage = validationMessage;
                    guess.clear();
                }
                else {
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
                            remainingTime = timeLimitPerTurn;
                            startTime = GetTime();
                        }
                        guess.clear();
                    }
                    else {
                        string target = player1Turn ? player2Number : player1Number;
                        correctDigits = countCorrectDigits(guess, target);
                        correctPositions = countCorrectPositions(guess, target);

                        if (guess == target) {
                            feedbackMessage = string(player1Turn ? "Player 1" : "Player 2") + string(" wins!");
                            gameOver = true;
                        }
                        else {
                            feedbackMessage = (player1Turn ? player1Name : player2Name);
                            feedbackMessage += " guessed " + guess + ": " +  // Add the guess to the feedback
                                to_string(correctDigits) + " correct digits, " +
                                to_string(correctPositions) + " in position.";

                            if (player1Turn) player1Turns++;
                            else player2Turns++;

                            if (player1Turns < turnLimit || player2Turns < turnLimit) {
                                player1Turn = !player1Turn;
                                startTime = GetTime();
                                remainingTime = timeLimitPerTurn;
                            }

                            if (player1Turns >= turnLimit && player2Turns >= turnLimit) {
                                feedbackMessage = "Turn limit reached! It's a draw.";
                                gameOver = true;
                            }
                        }

                        // Add to history with the feedback message
                        feedbackHistory.push_back(feedbackMessage);
                        guess.clear();
                    }
                }
            }
        }

        // Update timer if game is in progress (add this before BeginDrawing())
        if (!gameOver && !settingTurnLimit && !settingUp && !startScreen) {
            remainingTime = timeLimitPerTurn - (int)(GetTime() - startTime);

            // Check if time ran out
            if (remainingTime <= 0) {
                if (player1Turn) player1Turns++;
                else player2Turns++;

                feedbackMessage = (player1Turn ? player1Name : player2Name) + string(" ran out of time!");
                feedbackHistory.push_back(feedbackMessage);

                // Switch turns
                player1Turn = !player1Turn;
                startTime = GetTime();
                remainingTime = timeLimitPerTurn;
                guess.clear();

                // Check if game should end
                if (player1Turns >= turnLimit && player2Turns >= turnLimit) {
                    feedbackMessage = "Turn limit reached! It's a draw.";
                    gameOver = true;
                }
            }
        }

        BeginDrawing();
        ClearBackground(BACKGROUND_COLOR);

        if (startScreen) {
            // Modern title with shadow effect
            DrawText("NumBrainer", 253, 203, 50, Fade(BLACK, 0.2f));  // Shadow
            DrawText("NumBrainer", 250, 200, 50, PRIMARY_COLOR);

            // Stylish start button with hover effect
            int startButtonX = screenWidth / 2 - 100;
            int startButtonY = screenHeight / 2 - 25;
            int startButtonWidth = 200;
            int startButtonHeight = 50;

            bool isOverStartButton = IsMouseOverButton((int)mousePoint.x, (int)mousePoint.y,
                startButtonX, startButtonY,
                startButtonWidth, startButtonHeight);

            // Draw button with hover effect
            Color currentButtonColor = isOverStartButton ? BUTTON_HOVER_COLOR : BUTTON_COLOR;
            DrawRectangleRounded({ (float)startButtonX, (float)startButtonY,
                                 (float)startButtonWidth, (float)startButtonHeight },
                0.3f, 8, currentButtonColor);

            // Center the text in the button
            const char* startText = "START GAME";
            int textWidth = MeasureText(startText, 24);
            DrawText(startText,
                startButtonX + (startButtonWidth - textWidth) / 2,
                startButtonY + 12,
                24, WHITE);

            // Add a subtle description
            const char* descText = "A two-player number guessing game";
            int descWidth = MeasureText(descText, 20);
            DrawText(descText,
                screenWidth / 2 - descWidth / 2,
                screenHeight / 2 + 60,
                20, NEUTRAL_COLOR);

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && isOverStartButton) {
                startScreen = false;
                settingPlayer1Name = true;  // Start with player 1's name
            }
        }
        else if (settingPlayer1Name || settingPlayer2Name) {
            string& currentName = settingPlayer1Name ? player1Name : player2Name;

            // Handle keyboard input for names - only letters and space
            int key = GetKeyPressed();

            // Handle letters (both uppercase and lowercase)
            if (currentName.length() < 12) {
                // Handle uppercase letters
                if (key >= KEY_A && key <= KEY_Z) {
                    // Add uppercase if shift is pressed, lowercase otherwise
                    if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
                        currentName += (char)('A' + (key - KEY_A));
                    }
                    else {
                        currentName += (char)('a' + (key - KEY_A));
                    }
                }
                // Handle space
                else if (key == KEY_SPACE && !currentName.empty()) {  // Prevent starting with space
                    currentName += ' ';
                }
            }

            // Handle backspace
            if (IsKeyPressed(KEY_BACKSPACE) && !currentName.empty()) {
                currentName.pop_back();
            }

            // Handle enter key - only if name isn't empty and isn't just spaces
            if (IsKeyPressed(KEY_ENTER) && !currentName.empty() &&
                currentName.find_first_not_of(' ') != string::npos) {
                if (settingPlayer1Name) {
                    settingPlayer1Name = false;
                    settingPlayer2Name = true;
                    feedbackMessage = "Enter Player 2's name (must be different from Player 1)";
                }
                else {
                    // Check if Player 2's name is the same as Player 1's
                    if (currentName == player1Name) {
                        feedbackMessage = "Names must be different! Please choose another name.";
                        currentName.clear();
                    }
                    else {
                        settingPlayer2Name = false;
                        settingTurnLimit = true;
                        feedbackMessage.clear();
                        while (GetKeyPressed() != 0) {} // Clear key buffer
                        turnLimitInput.clear();
                    }
                }
            }

            // Draw name input screen
            DrawText("NumBrainer", screenWidth / 2 - MeasureText("NumBrainer", 50) / 2, 100, 50, PRIMARY_COLOR);

            string prompt = settingPlayer1Name ? "Enter Player 1's Name" : "Enter Player 2's Name";
            Color promptColor = settingPlayer1Name ? PRIMARY_COLOR : SECONDARY_COLOR;
            DrawText(prompt.c_str(),
                screenWidth / 2 - MeasureText(prompt.c_str(), 30) / 2,
                200, 30, promptColor);

            // Draw input box
            Rectangle inputBox = { screenWidth / 2 - 150, 250, 300, 40 };
            DrawRectangleRounded(inputBox, 0.2f, 8, Fade(INPUT_BG, 0.5f));

            // Draw current name
            DrawText(currentName.c_str(),
                screenWidth / 2 - MeasureText(currentName.c_str(), 20) / 2,
                260, 20, promptColor);

            // Draw blinking cursor
            if ((int)(GetTime() * 2) % 2) {
                DrawText("_",
                    screenWidth / 2 - MeasureText(currentName.c_str(), 20) / 2 +
                    MeasureText(currentName.c_str(), 20),
                    260, 20, promptColor);
            }

            // Draw feedback message if it exists
            if (!feedbackMessage.empty()) {
                DrawText(feedbackMessage.c_str(),
                    screenWidth / 2 - MeasureText(feedbackMessage.c_str(), 20) / 2,
                    320, 20, WARNING_COLOR);
            }

            // Draw instruction
            DrawText("Press ENTER to confirm",
                screenWidth / 2 - MeasureText("Press ENTER to confirm", 20) / 2,
                360, 20, NEUTRAL_COLOR);
        }
        else if (gameOver) {
            // Card container
            DrawRectangleRounded({ screenWidth / 2 - 250, 100, 500, 400 }, 0.02f, 8, WHITE);
            DrawRectangleRoundedLines({ screenWidth / 2 - 250, 100, 500, 400 }, 0.02f, 8, 2,
                ColorAlpha(NEUTRAL_COLOR, 0.3f));

            // Title with shadow effect
            DrawText("Game Over!", screenWidth / 2 - MeasureText("Game Over!", 40) / 2 + 3, 53, 40,
                Fade(BLACK, 0.2f));
            DrawText("Game Over!", screenWidth / 2 - MeasureText("Game Over!", 40) / 2, 50, 40,
                PRIMARY_COLOR);

            // Statistics
            string p1Stats = player1Name + "'s Turns: " + to_string(player1Turns) + "/" + to_string(turnLimit);
            string p2Stats = player2Name + "'s Turns: " + to_string(player2Turns) + "/" + to_string(turnLimit);

            // Player 1 stats
            DrawRectangleRounded({ screenWidth / 2 - 200, 180, 400, 40 }, 0.2f, 8,
                Fade(PRIMARY_COLOR, 0.1f));
            DrawText(p1Stats.c_str(), screenWidth / 2 - MeasureText(p1Stats.c_str(), 30) / 2, 185, 30,
                PRIMARY_COLOR);

            // Player 2 stats
            DrawRectangleRounded({ screenWidth / 2 - 200, 240, 400, 40 }, 0.2f, 8,
                Fade(SECONDARY_COLOR, 0.1f));
            DrawText(p2Stats.c_str(), screenWidth / 2 - MeasureText(p2Stats.c_str(), 30) / 2, 245, 30,
                SECONDARY_COLOR);

            // Result text
            Color resultColor = feedbackMessage.find("Player 1 wins") != string::npos ? PRIMARY_COLOR :
                              feedbackMessage.find("Player 2 wins") != string::npos ? SECONDARY_COLOR :
                              NEUTRAL_COLOR;
            DrawText(feedbackMessage.c_str(),
                screenWidth / 2 - MeasureText(feedbackMessage.c_str(), 30) / 2,
                320, 30, resultColor);

            // Buttons
            // Reset button
            Rectangle resetBtn = { screenWidth / 2 - 210, 400, 200, 40 };
            Color resetColor = CheckCollisionPointRec(mousePoint, resetBtn) ?
                BUTTON_HOVER_COLOR : BUTTON_COLOR;
            DrawRectangleRounded(resetBtn, 0.3f, 8, resetColor);
            DrawText("Reset (R)", screenWidth / 2 - 180, 410, 20, WHITE);

            // Menu button
            Rectangle menuBtn = { screenWidth / 2 + 10, 400, 200, 40 };
            Color menuColor = CheckCollisionPointRec(mousePoint, menuBtn) ?
                BUTTON_HOVER_COLOR : BUTTON_COLOR;
            DrawRectangleRounded(menuBtn, 0.3f, 8, menuColor);
            DrawText("Menu (M)", screenWidth / 2 + 40, 410, 20, WHITE);

            // Handle button clicks
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (CheckCollisionPointRec(mousePoint, resetBtn)) {
                    ResetGame(startScreen, player1Number, player2Number, guess, feedbackMessage,
                        player1Turn, settingUp, settingTurnLimit, gameOver,
                        feedbackHistory, player1Turns, player2Turns, turnLimitInput, remainingTime,
                        player1Name, player2Name, settingPlayer1Name, settingPlayer2Name);
                }
                else if (CheckCollisionPointRec(mousePoint, menuBtn)) {
                    startScreen = true;
                    ResetGame(startScreen, player1Number, player2Number, guess, feedbackMessage,
                        player1Turn, settingUp, settingTurnLimit, gameOver,
                        feedbackHistory, player1Turns, player2Turns, turnLimitInput, remainingTime,
                        player1Name, player2Name, settingPlayer1Name, settingPlayer2Name);
                }
            }
        }
        else {
            // Draw  title
            DrawText("NumBrainer", 253, 53, 50, Fade(BLACK, 0.2f));
            DrawText("NumBrainer", 250, 50, 50, PRIMARY_COLOR);

            // Add reset button in top-right corner
            Rectangle resetBtn = { (float)(screenWidth - 120), 20, 100, 40 };
            Color resetColor = CheckCollisionPointRec(mousePoint, resetBtn) ?
                BUTTON_HOVER_COLOR : BUTTON_COLOR;
            DrawRectangleRounded(resetBtn, 0.3f, 8, resetColor);
            DrawText("RESET", screenWidth - 100, 30, 20, WHITE);

            // Handle reset button click
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
                CheckCollisionPointRec(mousePoint, resetBtn)) {
                ResetGame(startScreen, player1Number, player2Number, guess, feedbackMessage,
                    player1Turn, settingUp, settingTurnLimit, gameOver,
                    feedbackHistory, player1Turns, player2Turns, turnLimitInput, remainingTime,
                    player1Name, player2Name, settingPlayer1Name, settingPlayer2Name);
            }

            // Main game container
            DrawRectangleRounded({ 50, 90, screenWidth - 100, screenHeight - 140 }, 0.02f, 8, WHITE);
            DrawRectangleRoundedLines({ 50, 90, screenWidth - 100, screenHeight - 140 }, 0.02f, 8, 2,
                Fade(NEUTRAL_COLOR, 0.3f));

            if (settingTurnLimit) {
                DrawText("Game Setup", screenWidth / 2 - MeasureText("Game Setup", 40) / 2, 110, 40, PRIMARY_COLOR);
                DrawModernInput("Number of turns per player", turnLimitInput.c_str(), 100, 180, true);
                DrawText("Press ENTER to confirm", 100, 280, 20, NEUTRAL_COLOR);

                if (!feedbackMessage.empty()) {
                    DrawFeedbackMessage(feedbackMessage.c_str(), 100, 320);
                }
            }
            else if (settingUp) {
                string setupText = player1Turn ? player1Name : player2Name;
                setupText += ", set your number";

                DrawText(setupText.c_str(), screenWidth / 2 - MeasureText(setupText.c_str(), 30) / 2, 110, 30,
                    player1Turn ? PRIMARY_COLOR : SECONDARY_COLOR);

                string maskedGuess(guess.length(), '*');
                DrawModernInput("Enter 4-digit number", maskedGuess.c_str(), 100, 180, true);

                DrawText("Number must be 4 digits with no repeats", 100, 280, 20, NEUTRAL_COLOR);
                DrawText("Press ENTER to confirm", 100, 310, 20, NEUTRAL_COLOR);

                if (!feedbackMessage.empty()) {
                    DrawFeedbackMessage(feedbackMessage.c_str(), 100, 350);
                }
            }
            else {
                string playerText = player1Turn ? player1Name + "'s Turn" : player2Name + "'s Turn";
                DrawText(playerText.c_str(), screenWidth / 2 - MeasureText(playerText.c_str(), 30) / 2, 110, 30,
                    player1Turn ? PRIMARY_COLOR : SECONDARY_COLOR);

                DrawModernInput("Enter your guess", guess.c_str(), 100, 180, true);

                Color timerColor = remainingTime <= 5 ? TIMER_WARNING : NEUTRAL_COLOR;
                if (remainingTime <= 5) {
                    timerColor = ColorAlpha(TIMER_WARNING, 0.5f + sinf(GetTime() * 4) * 0.5f);
                }

                string timeText = "Time: " + to_string(remainingTime) + "s";
                DrawRectangleRounded({ 100, 250, 150, 40 }, 0.2f, 8, Fade(timerColor, 0.1f));
                DrawText(timeText.c_str(), 120, 260, 20, timerColor);

                if (!feedbackMessage.empty()) {
                    DrawFeedbackMessage(feedbackMessage.c_str(), 100, 310);
                }

                // Feedback history with scrolling
                DrawText("History", 100, 350, 25, NEUTRAL_COLOR);

                // Define the scrollable area
                Rectangle historyArea = { 90, 390, 620, 160 }; // Fixed height for history area

                // Calculate total content height
                float totalContentHeight = feedbackHistory.size() * 35; // 35 pixels per history item

                // Handle mouse wheel for scrolling
                static float scrollOffset = 0;
                float wheel = GetMouseWheelMove();
                scrollOffset -= wheel * 20; // 20 pixels per scroll step

                // Clamp scroll offset
                float maxScroll = totalContentHeight - historyArea.height;
                scrollOffset = Clamp(scrollOffset, 0, maxScroll > 0 ? maxScroll : 0);

                // Begin scissors mode to clip content outside the history area
                BeginScissorMode((int)historyArea.x, (int)historyArea.y,
                    (int)historyArea.width, (int)historyArea.height);

                // Draw history items with scroll offset
                int yOffset = 390 - (int)scrollOffset;
                for (const string& feedback : feedbackHistory) {
                    // Only draw if in visible area (with some padding)
                    if (yOffset + 35 >= historyArea.y - 35 && yOffset <= historyArea.y + historyArea.height + 35) {
                        // Use the color based on whose turn it was
                        Color feedbackColor = feedback.find(player1Name) != string::npos ?
                            PRIMARY_COLOR : SECONDARY_COLOR;

                        DrawRectangleRounded({ 90, (float)yOffset - 5, 620, 30 },
                            0.2f, 8, Fade(feedbackColor, 0.1f));
                        DrawText(feedback.c_str(), 100, yOffset, 20, feedbackColor);
                    }
                    yOffset += 35;
                }

                EndScissorMode();

                // Draw scroll indicator if content exceeds view area
                if (totalContentHeight > historyArea.height) {
                    float scrollBarHeight = (historyArea.height / totalContentHeight) * historyArea.height;
                    float scrollBarY = historyArea.y + (scrollOffset / maxScroll) *
                        (historyArea.height - scrollBarHeight);

                    // Draw scroll track
                    DrawRectangleRounded({ historyArea.x + historyArea.width + 5, historyArea.y,
                                        8, historyArea.height }, 1.0f, 2,
                        Fade(NEUTRAL_COLOR, 0.1f));

                    // Draw scroll thumb
                    DrawRectangleRounded({ historyArea.x + historyArea.width + 5, scrollBarY,
                                        8, scrollBarHeight }, 1.0f, 2,
                        Fade(NEUTRAL_COLOR, 0.5f));
                }
            }
        }

        // Reset game state
        if (IsKeyPressed(KEY_R) && gameOver) {
            ResetGame(startScreen, player1Number, player2Number, guess, feedbackMessage,
                player1Turn, settingUp, settingTurnLimit, gameOver,
                feedbackHistory, player1Turns, player2Turns, turnLimitInput, remainingTime,
                player1Name, player2Name, settingPlayer1Name, settingPlayer2Name);
        }
        else if (IsKeyPressed(KEY_M) && gameOver) {
            startScreen = true;
            ResetGame(startScreen, player1Number, player2Number, guess, feedbackMessage,
                player1Turn, settingUp, settingTurnLimit, gameOver,
                feedbackHistory, player1Turns, player2Turns, turnLimitInput, remainingTime,
                player1Name, player2Name, settingPlayer1Name, settingPlayer2Name);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
