#include "include/raylib.h"
#include <stdio.h>
#include <stdbool.h>

#define WIDTH 800
#define HEIGHT 600

#define SQUARE_SIZE 20
#define MAX_SNAKE_LEN 256
#define SNAKE_SPEED 5

#define RIGHT (Vector2){1, 0}
#define LEFT (Vector2){-1, 0}
#define UP (Vector2){0, -1}
#define DOWN (Vector2){0, 1}

typedef struct game_context {
    Vector2 snake[MAX_SNAKE_LEN];
    int snake_len;
    Vector2 direction;
    Vector2 food;
    bool game_over;
} game_context;

Vector2 get_food()
{
    return (Vector2){
        GetRandomValue(0, WIDTH / SQUARE_SIZE - 1),
        GetRandomValue(0, HEIGHT / SQUARE_SIZE - 1)
    };
}

void init_game(game_context* gc)
{
    for (size_t i = 0; i < MAX_SNAKE_LEN; i++)
        gc->snake[i] = (Vector2){0, 0};

    gc->snake_len = 2;
    gc->direction = RIGHT;
    gc->food = get_food();
    gc->game_over = false;
}

void handle_input(game_context* gc)
{
    if (IsKeyPressed(KEY_UP) && gc->direction.y != 1)
        gc->direction = UP;
    if (IsKeyPressed(KEY_DOWN) && gc->direction.y != -1)
        gc->direction = DOWN;
    if (IsKeyPressed(KEY_LEFT) && gc->direction.x != 1)
        gc->direction = LEFT;
    if (IsKeyPressed(KEY_RIGHT) && gc->direction.x != -1)
        gc->direction = RIGHT;

    if (IsKeyPressed(KEY_R) && gc->game_over)
    {
        gc->game_over = false;
        init_game(gc);
    }
}

void move_snake(game_context* gc)
{
    for (int i = gc->snake_len - 1; i > 0; i--)
        gc->snake[i] = gc->snake[i - 1];

    gc->snake[0].x += gc->direction.x / SNAKE_SPEED;
    gc->snake[0].y += gc->direction.y / SNAKE_SPEED;
}

void handle_collisions(game_context* gc)
{
    Vector2 snake = gc->snake[0];

    Rectangle snake_head = {
        snake.x * SQUARE_SIZE,
        snake.y * SQUARE_SIZE,
        SQUARE_SIZE,
        SQUARE_SIZE
    };

    Rectangle food_rect = {
        gc->food.x * SQUARE_SIZE,
        gc->food.y * SQUARE_SIZE,
        SQUARE_SIZE,
        SQUARE_SIZE
    };

    if (CheckCollisionRecs(snake_head, food_rect))
    {
        gc->snake_len++;
        gc->food = get_food();
    }

    if (
        snake.x < 0 || snake.x >= WIDTH / SQUARE_SIZE ||
        snake.y < 0 || snake.y >= HEIGHT / SQUARE_SIZE
    )
    {
        gc->game_over = true;
    }

    for (size_t i = 1; i < gc->snake_len; i++)
    {
        if (snake.x == gc->snake[i].x && snake.y == gc->snake[i].y)
        {
            gc->game_over = true;
            break;
        }
    }
}

void draw_game(game_context gc)
{
    if (gc.game_over)
    {
        const char* text = "Game over! Press R to restart.";
        int font_size = 35;
        Vector2 text_size = MeasureTextEx(GetFontDefault(), text, font_size, 1);
        int pos_x = (GetScreenWidth() - text_size.x) / 2;
        int pos_y = (GetScreenHeight() - text_size.y) / 2;
        DrawText(text, pos_x, pos_y, font_size, BLACK);
    }
    else
    {
        DrawRectangle(
            gc.food.x * SQUARE_SIZE,
            gc.food.y * SQUARE_SIZE,
            SQUARE_SIZE,
            SQUARE_SIZE,
            RED
        );

        for (size_t i = 0; i < gc.snake_len; i++)
            DrawRectangle(
                gc.snake[i].x * SQUARE_SIZE,
                gc.snake[i].y * SQUARE_SIZE,
                SQUARE_SIZE,
                SQUARE_SIZE,
                DARKGREEN
            );
    }
}

int main()
{
    InitWindow(WIDTH, HEIGHT, "Snake Game");
    SetTargetFPS(60);

    game_context gc;
    init_game(&gc);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        handle_input(&gc);
        move_snake(&gc);
        handle_collisions(&gc);
        draw_game(gc);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}