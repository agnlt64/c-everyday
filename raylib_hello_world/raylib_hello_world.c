#include "include/raylib.h"

#define WIDTH 800
#define HEIGHT 600

int main()
{
    InitWindow(WIDTH, HEIGHT, "Raylib Hello World");
    SetTargetFPS(30);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLUE);
        DrawFPS(20, 30);
        DrawText("Hello World!", 20, 60, 20, WHITE);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}