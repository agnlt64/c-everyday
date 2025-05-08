#include "../include/raylib.h"
#include <math.h>
#include <stdio.h>

#define WIDTH 1600
#define HEIGHT 900

#define NUM_SECTORS 37
#define RADIUS 300

typedef struct sector {
    int number;
    Color color;
} sector_t;

typedef struct game_context {
    sector_t sectors[NUM_SECTORS];
    float speed;
    float angle;
    int winning_number;
    bool spinning;
    int selected_number;
    int balance;
    int bet_amount;
} game_context;

void init_sectors(sector_t* sectors)
{
    for (size_t i = 0; i < NUM_SECTORS; i++)
    {
        sectors[i].number = i;

        if (i == 0)
            sectors[i].color = LIME;
        else if ((i >= 1 && i <= 10) || (i >= 19 && i <= 28))
            sectors[i].color = (i % 2) == 0 ? BLACK : RED;
        else
            sectors[i].color = (i % 2) == 0 ? RED : BLACK;
    }
}

void shuffle_sectors(sector_t* sectors)
{
    for (int i = NUM_SECTORS - 1; i > 1; i--)
    {
        int j = GetRandomValue(1, i);
        sector_t temp = sectors[i];
        sectors[i] = sectors[j];
        sectors[j] = temp;
    }

    for (size_t i = 0; i < NUM_SECTORS; i++)
        sectors[i].color = (i % 2 == 1) ? RED : BLACK;
    sectors[0].color = LIME;
}

void init_game(game_context* gc)
{
    init_sectors(gc->sectors);
    shuffle_sectors(gc->sectors);
    gc->speed = 0;
    gc->winning_number = -1;
    gc->angle = 0;
    gc->spinning = false;
    gc->bet_amount = 0;
    gc->balance = 100000;
    gc->selected_number = -1;
}

int get_clicked_sector(game_context* gc, Vector2 center)
{
    Vector2 mouse_pos = GetMousePosition();
    float dx = mouse_pos.x - center.x;
    float dy = mouse_pos.y - center.y;
    float distance = sqrtf(dx * dx + dy * dy);

    if (distance <= RADIUS)
    {
        float angle = atan2f(dy, dx);
        angle = fmodf(angle + PI/2 - gc->angle * DEG2RAD, 2 * PI);
        if (angle < 0) angle += 2 * PI;
        angle -= PI/2;

        int sector = (int)(angle * NUM_SECTORS / (2 * PI)) % NUM_SECTORS;
        return gc->sectors[sector].number;
    }
    return -1;
}

void handle_inputs(game_context* gc)
{
    if (!gc->spinning)
    {
        Vector2 center = {
            WIDTH / 2.0f,
            HEIGHT / 2.0f,
        };

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            gc->selected_number = get_clicked_sector(gc, center);

        int increment = 500;
        if (IsKeyDown(KEY_LEFT))
            gc->bet_amount = gc->bet_amount > increment ? gc->bet_amount - increment : 0;
        if (IsKeyDown(KEY_RIGHT))
            gc->bet_amount = gc->bet_amount < gc->balance ? gc->bet_amount + increment : gc->balance;
    }

    if (IsKeyPressed(KEY_SPACE) && !gc->spinning)
    {
        gc->speed = GetRandomValue(400, 700);
        gc->winning_number = -1;
        gc->spinning = true;
    }
}

void draw_slider(int amount)
{
    DrawRectangle(WIDTH/2 - 200, HEIGHT - 100, 400, 30, LIGHTGRAY);
    DrawRectangle(WIDTH/2 - 200, HEIGHT - 100, (amount - 10) * 400 / 99990, 30, DARKGRAY);
    const char* text = TextFormat("Amount: %d", amount);
    int font_size = 20;
    DrawText(text, WIDTH / 2 - MeasureText(text, font_size) / 2, HEIGHT - 140, font_size, BLACK);
}

void update_game(game_context* gc)
{
    if (gc->spinning)
    {
        float dt = GetFrameTime();
        gc->angle += gc->speed * dt;
        gc->speed -= 60 * dt;

        if (gc->speed <= 0)
        {
            gc->spinning = false;
            gc->speed = 0;

            float norm = fmodf(gc->angle, 360.0f);
            if (norm < 0)
                norm += 360.0f;

            float sector_angle = 360.0f / NUM_SECTORS;
            int sector = (int)((270.0f - norm + 0.5 * sector_angle) / sector_angle) % NUM_SECTORS;
            if (sector < 0) sector += NUM_SECTORS;
            gc->winning_number = gc->sectors[sector].number;

            if (gc->selected_number != -1)
            {
                if (gc->selected_number != gc->winning_number)
                    gc->balance = fmax(gc->balance - gc->bet_amount, 0);
                else
                    gc->balance += gc->bet_amount * NUM_SECTORS;
                gc->selected_number = -1;
                draw_slider(gc->balance);
            }
        }
    }
}

void draw_roulette_triangle(game_context gc, Vector2 center, int i, float start_angle, float end_angle)
{
    Vector2 p1 = {
        center.x + cosf(start_angle) * RADIUS,
        center.y + sinf(start_angle) * RADIUS,
    };

    Vector2 p2 = {
        center.x + cosf(end_angle) * RADIUS,
        center.y + sinf(end_angle) * RADIUS,
    };
    Color sector_color = gc.sectors[i].color;
    DrawTriangle(center, p2, p1, sector_color);

    float mid_angle = (start_angle + end_angle) / 2.0f;
    Vector2 text_pos = {
        center.x + cosf(mid_angle) * RADIUS * 0.6f,
        center.y + sinf(mid_angle) * RADIUS * 0.6f,
    };

    int roulette_font_size = 20;
    const char* text = TextFormat("%d", gc.sectors[i].number);
    DrawText(text, text_pos.x - MeasureText(text, roulette_font_size) / 2, text_pos.y - 6, roulette_font_size, WHITE);
}

void draw_selector(game_context gc, Vector2 center)
{
    DrawTriangle(
        (Vector2){center.x + 10, center.y - RADIUS - 30},
        (Vector2){center.x - 10, center.y - RADIUS - 30},
        (Vector2){center.x, center.y - RADIUS - 10},
        YELLOW
    );
}

void draw_hud(game_context gc)
{
    int font_size = 30;
    if (gc.winning_number >= 0)
    {
        const char* text = TextFormat("Winning number: %d", gc.winning_number);
        DrawText(text, WIDTH / 2 - MeasureText(text, font_size) / 2, 30, font_size, GOLD);
    }

    int pad_x = 20;
    DrawText(TextFormat("Balance: %d$", gc.balance), pad_x, font_size, font_size, GOLD);

    if (gc.selected_number != -1)
        DrawText(TextFormat("Betting on: %d", gc.selected_number), pad_x, font_size * 2, font_size, BLACK);

    if (gc.winning_number != -1)
    {
        if (gc.winning_number != gc.selected_number)
            DrawText("You lost!", pad_x, font_size * 3, font_size, RED);
        else
            DrawText("You won!", pad_x, font_size * 3, font_size, GREEN);
    }

    DrawText("Press SPACE to start", 20, HEIGHT - 40, font_size, BLACK);
    DrawText("Use arrow keys to bet", WIDTH - 400, HEIGHT - 40, font_size, BLACK);
}

void draw_game(game_context gc)
{
    Vector2 center = {
        WIDTH / 2.0f,
        HEIGHT / 2.0f,
    };
    float sector_angle = 2 * PI / NUM_SECTORS;

    for (size_t i = 0; i < NUM_SECTORS; i++)
    {
        float start_angle = gc.angle * DEG2RAD + i * sector_angle;
        float end_angle = start_angle + sector_angle;

        draw_roulette_triangle(gc, center, i, start_angle, end_angle);
        draw_selector(gc, center);
        draw_slider(gc.bet_amount);
        draw_hud(gc);
    }
}

int main()
{
    InitWindow(WIDTH, HEIGHT, "Roulette");
    SetTargetFPS(60);

    game_context gc;
    init_game(&gc);

    while (!WindowShouldClose())
    {
        handle_inputs(&gc);
        update_game(&gc);

        BeginDrawing();
        ClearBackground((Color){116, 59, 15});

        draw_game(gc);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}