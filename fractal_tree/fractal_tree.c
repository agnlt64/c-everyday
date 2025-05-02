#include "include/raylib.h"
#include <math.h>

void draw_fractal_tree(int x1, int y1, float len, float angle, int depth, float zoom)
{
    if (depth == 0 || len < 2)
        return;

    int x2 = x1 + (int)(len * zoom * cos(angle));
    int y2 = y1 - (int)(len * zoom * sin(angle));

    DrawLine(x1, y1, x2, y2, WHITE);

    draw_fractal_tree(x2, y2, len * 0.7f, angle - PI / 6, depth- 1, zoom);
    draw_fractal_tree(x2, y2, len * 0.7f, angle + PI / 6, depth- 1, zoom);
}

int main()
{
    const int width = 800;
    const int height = 600;
    float zoom = 1.0f;

    InitWindow(width, height, "Fractal Tree");
    SetTargetFPS(60);

    int depth = 2;

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_UP))
            depth++;

        if (IsKeyPressed(KEY_DOWN))
            depth--;

        if (depth < 1) depth = 1;
        if (depth > 10) depth = 10;

        float scroll = GetMouseWheelMove();
        zoom += scroll * 0.1f;

        if (zoom < 1.0f) zoom = 1.0f;
        if (zoom > 1.7f) zoom = 1.7f;

        BeginDrawing();
        ClearBackground(BLACK);

        DrawText("Press Up/Down to change depth", 10, 10, 20, WHITE);
        DrawText(TextFormat("Depth: %d", depth), 10, 40, 20, WHITE);

        draw_fractal_tree(width / 2, height - 50, 100.0f, PI / 2, depth, zoom);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}