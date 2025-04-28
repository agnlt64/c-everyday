#include "include/raylib.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define WIDTH 800
#define HEIGHT 900
#define GRID_SIZE 3

#define EMPTY ' '

typedef struct game_context {
    char grid[GRID_SIZE][GRID_SIZE];
    bool win;
    bool draw;
    char player;
} game_context;

void init_context(game_context* gc)
{
    for (size_t i = 0; i < GRID_SIZE; i++)
    {
        for (size_t j = 0; j < GRID_SIZE; j++)
        {
            gc->grid[i][j] = EMPTY;
        }
    }
    gc->player = 'x';
    gc->win = false;
    gc->draw = false;
}

bool check_winner(game_context* gc)
{
    // lines
    for (size_t i = 0; i < GRID_SIZE; i++)
    {
        int count = 0;
        if (gc->grid[i][0] == EMPTY)
            continue;
        
        for (size_t j = 0; j < GRID_SIZE; j++)
        {
            if (gc->grid[i][j] == gc->player)
                count++;
        }
        if (count == GRID_SIZE)
            return true;
    }

    // cols
    for (size_t j = 0; j < GRID_SIZE; j++)
    {
        int count = 0;
        if (gc->grid[0][j] == EMPTY)
            continue;

        for (size_t i = 0; i < GRID_SIZE; i++)
        {
            if (gc->grid[i][j] == gc->player)
                count++;
        }
        if (count == GRID_SIZE)
            return true;
    }

    // main diag
    if (gc->grid[0][0] != EMPTY)
    {
        int count = 0;
        for (size_t i = 0; i < GRID_SIZE; i++)
        {
            if (gc->grid[i][i] == gc->player)
                count++;
        }
        if (count == GRID_SIZE)
            return true;
    }

    // other diag
    if (gc->grid[0][GRID_SIZE - 1] != EMPTY)
    {
        int count = 0;
        for (size_t i = 0; i < GRID_SIZE; i++)
        {
            if (gc->grid[i][GRID_SIZE - i - 1] == gc->player)
                count++;
        }
        if (count == GRID_SIZE)
            return true;
    }

    return false;
}

int draw_grid(game_context* gc, int padding, Font font)
{
    int cell_size = (WIDTH - (padding * (GRID_SIZE + 1))) / GRID_SIZE;
    for (size_t i = 0; i < GRID_SIZE; i++)
    {
        for (size_t j = 0; j < GRID_SIZE; j++)
        {
            int x = padding + j * (cell_size + padding);
            int y = padding + i * (cell_size + padding);
            DrawRectangleLines(x, y, cell_size, cell_size, WHITE);

            if (gc->grid[i][j] == 'o')
            {
                int cx = x + cell_size / 2;
                int cy = y + cell_size / 2;
                int radius = cell_size / 2 - padding;
                DrawCircleLines(cx, cy, radius, WHITE);
            }
            else if (gc->grid[i][j] == 'x')
            {
                DrawLine(x + padding, y + padding, x + cell_size - padding, y + cell_size - padding, WHITE);
                DrawLine(x + cell_size - padding, y + 20, x + padding, y + cell_size - padding, WHITE);
            }
            
            Vector2 pos = {
                padding, WIDTH + padding * 1.5
            };
            
            char* turn = gc->player == 'x' ? "X" : "O";
            const char* text = "no text";
            int font_size = 50;

            if (gc->win)
                text = TextFormat("Player %s has won! Press R to reset the game.", turn);
            else if (gc->draw)
                text = "It's a draw. Press R to reset the game.";
            else
                text = TextFormat("Current turn: %s", turn);

            DrawTextEx(font, text, pos, font_size, 1, WHITE);
        }
    }
    return cell_size;
}

bool is_grid_full(game_context* gc)
{
    for (size_t i = 0; i < GRID_SIZE; i++)
    {
        for (size_t j = 0; j < GRID_SIZE; j++)
        {
            if (gc->grid[i][j] == EMPTY)
                return false;
        }
    }
    return true;
}

void handle_click(game_context* gc, int cell_size, int padding)
{
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Vector2 mouse = GetMousePosition();

        for (size_t i = 0; i < GRID_SIZE; i++)
        {
            for (size_t j = 0; j < GRID_SIZE; j++)
            {
                int x = padding + j * (cell_size + padding);
                int y = padding + i * (cell_size + padding);

                Rectangle cell = CLITERAL(Rectangle){
                    x, y, cell_size, cell_size
                };

                if (CheckCollisionPointRec(mouse, cell))
                {
                    if (gc->grid[i][j] == EMPTY)
                    {
                        gc->grid[i][j] = gc->player;
                        if (check_winner(gc))
                            gc->win = true;
                        else if (is_grid_full(gc))
                            gc->draw = true;
                        else
                            gc->player = gc->player == 'x' ? 'o' : 'x';
                    }
                }
            }
        }
    }

    if ((gc->win || gc->draw) && IsKeyPressed(KEY_R))
        init_context(gc);
}

int main()
{
    InitWindow(WIDTH, HEIGHT, "Tic Tac Toe Game");
    SetTargetFPS(60);

    Font font = LoadFontEx("./fonts/Tuffy/Tuffy-Regular.ttf", 96, 0, 0);
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
    
    game_context gc = {0};
    init_context(&gc);
    
    int padding = 20;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        int cell_size = draw_grid(&gc, padding, font);
        handle_click(&gc, cell_size, padding);
        EndDrawing();
    }

    UnloadFont(font);
    CloseWindow();

    return 0;
}