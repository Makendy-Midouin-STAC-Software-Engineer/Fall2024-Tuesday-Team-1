#include "raylib.h"
#include <string>

int main() {
    // Initialize the Raylib window
    InitWindow(800, 600, "Numbrainer");
    SetTargetFPS(60);
    
	//Variable to store the player's input
    std::string playerInput = "";

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        // Capture digit input (0-9) and append to playerInput until it reaches 4 characters
        if (playerInput.length() < 4) {
            int key = GetCharPressed();

            // Check if the key is between '0' and '9'
            if (key >= '0' && key <= '9') {
                playerInput += (char)key;
            }
        }
        // Allow user to delete the last character with backspace  
        if (IsKeyPressed(KEY_BACKSPACE) && !playerInput.empty()) {
            playerInput.pop_back();
        }

        // Display "Numbrainer" at the top center of the window
        DrawText("NumBrainer", 250, 50, 40, DARKBLUE);
        //void DrawText(const char* text, int posX, int posY, int fontSize, Color color);
        DrawText("Enter a 4-digit number:", 100, 100, 20, DARKGRAY);
        DrawText(playerInput.c_str(), 350, 100, 20, DARKBLUE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
