#include "include/raylib.h"
#include "include/raymath.h"
#include <math.h>
#include <stdlib.h>

#define WIDTH 800
#define HEIGHT 600

#define SEGMENTS 40
#define NB_CIRCLES 7
#define CIRCLE_RADIUS 40

#define GRAVITY 98

typedef struct line {
    Vector2 p1;
    Vector2 p2;
} line_t;

typedef struct circle {
    float radius;
    float start_angle;
    float end_angle;
    int speed;
    line_t lines[SEGMENTS];
} circle_t;

typedef struct ball {
    Vector2 pos;
    Vector2 velocity;
    int radius;
} ball_t;

void init_circles(circle_t* circles)
{
    for (size_t i = 0; i < NB_CIRCLES; i++)
    {
        circles[i].radius = (i + 1) * CIRCLE_RADIUS;
        circles[i].start_angle = GetRandomValue(0, 20);
        circles[i].end_angle = GetRandomValue(310, 350);
        circles[i].speed = GetRandomValue(30, 90);
    }
}

void init_ball(ball_t* ball, Vector2 init_pos)
{
    ball->pos = init_pos;
    ball->radius = 5;
    ball->velocity = (Vector2){200, 200};
}

void draw_arc(circle_t* circle, Vector2 center, Color color)
{
    float step = (circle->end_angle - circle->start_angle) / SEGMENTS;

    for (size_t i = 0; i < SEGMENTS; i++)
    {
        float angle1 = (circle->start_angle + i * step) * DEG2RAD;
        float angle2 = (circle->start_angle + (i + 1) * step) * DEG2RAD;

        line_t line = {0};
        line.p1 = (Vector2){
            center.x + cosf(angle1) * circle->radius,
            center.y + sinf(angle1) * circle->radius,
        };
        line.p2 = (Vector2){
            center.x + cosf(angle2) * circle->radius,
            center.y + sinf(angle2) * circle->radius,
        };
        circle->lines[i] = line;

        DrawLineV(line.p1, line.p2, color);
    }
}

void draw_circles(circle_t* circles, Vector2 center, float dt)
{
    for (size_t i = 0; i < NB_CIRCLES; i++)
    {
        float rotation = circles[i].start_angle;
        rotation += circles[i].speed * dt;
        circles[i].start_angle = rotation;
        circles[i].end_angle = rotation + 320;
        draw_arc(&circles[i], center, WHITE);
    }
}

void update_ball(ball_t* ball, float dt)
{
    ball->velocity.y += GRAVITY * dt;
    ball->pos.x += ball->velocity.x * dt;
    ball->pos.y += ball->velocity.y * dt;
}

float dist_to_center(ball_t ball, Vector2 center)
{
    return (ball.pos.x - center.x) * (ball.pos.x - center.x) +
           (ball.pos.y - center.y) * (ball.pos.y - center.y);
}

Vector2 get_normal(line_t line)
{
    Vector2 line_dir = Vector2Subtract(line.p2, line.p1);
    Vector2 normal = (Vector2){-line_dir.y, line_dir.x};
    return Vector2Normalize(normal);
}

void update_circles(circle_t* circles, ball_t* ball, Vector2 center, int* num_circles)
{
    int r = circles[0].radius;
    if (dist_to_center(*ball, center) > r*r)
    {
        for (size_t i = 0; i < *num_circles - 1; i++)
            circles[i] = circles[i + 1];
        (*num_circles)--;
    }

    for (size_t i = 0; i < *num_circles; i++)
    {
        circle_t circle = circles[i];
        for (size_t j = 0; j < SEGMENTS; j++)
        {
            line_t line = circle.lines[j];
            if (CheckCollisionCircleLine(ball->pos, ball->radius, line.p1, line.p2))
            {
                Vector2 normal = get_normal(line);
                ball->velocity = Vector2Reflect(ball->velocity, normal);

                Vector2 offset = Vector2Scale(normal, 0.5f);
                ball->pos = Vector2Add(ball->pos, offset);

                return;
            }
        }
    }
}

int main()
{
    InitWindow(WIDTH, HEIGHT, "Bouncing ball");
    SetTargetFPS(60);

    Vector2 center = {
        WIDTH / 2.0f,
        HEIGHT / 2.0f,
    };

    ball_t ball = {0};
    init_ball(&ball, center);

    int num_circles = NB_CIRCLES;
    circle_t circles[NB_CIRCLES] = {0};
    init_circles(circles);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        DrawCircleV(ball.pos, ball.radius, WHITE);

        float dt = GetFrameTime();
        draw_circles(circles, center, dt);
        update_ball(&ball, dt);

        if (num_circles > 0)
            update_circles(circles, &ball, center, &num_circles);
        else
            break;

        EndDrawing();
    }

    CloseWindow();

    return 0;
}