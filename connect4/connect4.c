#define MATRIX_IMPLEMENTATION
#include "include/matrix.h"

#include "include/raylib.h"

#include <stdbool.h>

#define WIDTH 800
#define HEIGHT 600

#define GRID_SIZE 7

typedef struct game_context {
    matrix_t* grid;
    bool game_over;
    int player;
} game_context;

void init_game(game_context* gc)
{
    gc->grid = mat_alloc(GRID_SIZE, GRID_SIZE);
    gc->game_over = false;
    gc->player = 0;
}

bool insert_token(game_context* gc, int col)
{
    if (col < 0 || col >= GRID_SIZE)
        return false;

    for (int row = gc->grid->rows - 1; row >= 0; row--)
    {
        if (mat_at(gc->grid, row, col) == 0)
        {
            mat_at(gc->grid, row, col) = gc->player + 1;
            return true;
        }
    }
    return false;
}

bool check_winner(game_context* gc)
{
    int target = gc->player + 1;
    int rows = gc->grid->rows;
    int cols = gc->grid->cols;

    int directions[4][2] = {
        {1, 0},
        {0, 1},
        {1, 1},
        {-1, 1}
    };

    mat_for(gc->grid, 
        if (mat_at(gc->grid, i, j) != target)
            continue;

        for (size_t d = 0; d < 4; d++)
        {
            int dx = directions[d][0];
            int dy = directions[d][1];
            int count = 0;

            for (size_t i2 = 0; i2 < 4; i2++)
            {
                int r = i + i2 * dy;
                int c = j + i2 * dx;

                if (r < 0 || r >= rows || c < 0 || c >= cols)
                    break;

                if (mat_at(gc->grid, r, c) == target)
                    count++;
                else
                    break;
            }

            if (count == 4)
                return true;
        }
    );
    return false;
}

int get_clicked_col(int cell_size, int offset_x)
{
    int mouse_x = GetMouseX();
    if (mouse_x < offset_x)
        return -1;

    int col = (mouse_x - offset_x) / cell_size;
    if (col >= GRID_SIZE)
        return -1;

    return col;
}

void handle_click(game_context* gc)
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        int cell_size = HEIGHT / GRID_SIZE;
        int offset_x = (WIDTH - GRID_SIZE * cell_size) / 2;

        int col = get_clicked_col(cell_size, offset_x);

        if (col != -1 && insert_token(gc, col))
        {
            if (check_winner(gc))
            {
                gc->game_over = true;
            }
            else
                gc->player = 1 - gc->player;
        }
    }

    if (IsKeyPressed(KEY_R) && gc->game_over)
        init_game(gc);
}

void draw_grid(matrix_t* grid)
{
    int cols = grid->cols;
    int rows = grid->rows;
    int cell_size = HEIGHT / rows;
    int offset_x = (WIDTH - cols * cell_size) / 2;
    int offset_y = (HEIGHT - rows * cell_size) / 2;

    mat_for(grid, 
        int value = mat_at(grid, i, j);
        int cx = offset_x + j * cell_size + cell_size / 2;
        int cy = offset_y + i * cell_size + cell_size / 2;

        Color color;
        switch (value)
        {
        case 0:
            color = RAYWHITE;
            break;
        case 1:
            color = RED;
            break;
        case 2:
            color = BLUE;
            break;
        default:
            break;
        }

        DrawCircle(cx, cy, cell_size / 2 - 5, color);
    );
}

void draw_text_centered(const char* text, int font_size)
{
    Vector2 text_size = MeasureTextEx(GetFontDefault(), text, font_size, 1);
    int pos_x = (WIDTH - text_size.x) / 2;
    int pos_y = (HEIGHT - text_size.y) / 2;
    DrawText(text, pos_x, pos_y, font_size, WHITE);
}

void draw_game(game_context gc)
{
    ClearBackground((Color){7, 69, 143});
    int font_size = 30;
    int padding = 20;

    if (gc.game_over)
    {
        const char* winner = (gc.player == 0) ? "Red" : "Blue";
        draw_text_centered(TextFormat("Winner is %s!\n\nPress R to restart", winner), font_size);
    }
    else
    {
        draw_grid(gc.grid);

        switch (gc.player)
        {
        case 0:
            DrawText("Red", padding, padding, font_size, WHITE);
            break;
        case 1:
            DrawText("Blue", padding, padding, font_size, WHITE);
            break;
        default:
            break;
        }
    }
}

int main()
{
    InitWindow(WIDTH, HEIGHT, "Connect 4");
    SetTargetFPS(30);

    game_context gc;
    init_game(&gc);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        
        handle_click(&gc);
        draw_game(gc);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}