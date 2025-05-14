#include "../include/raylib.h"
#include "../include/raymath.h"
#include <stdlib.h>

#define WIDTH 1600
#define HEIGHT 900

#define DEFAULT_AK_VIEWMODEL load_cam((Vector3){-0.9, 1.95, 1.6}, (Vector3){-1, 1.95, -0.2})
#define ZOOMED_AK_VIEWMODEL (Vector3){0, 2.2, 1}

bool debug = false;

typedef struct animator {
    float anim_time;
    float duration;
} animator_t;

typedef struct weapon {
    Model model;
    Camera cam;

    Vector3 scale;
    Vector3 rotation_axis;
    Vector3 fixed_pos;

    int max_ammo;
    int curr_ammo;

    float accuracy;
    float max_spread;
    float last_shot_dt;

    bool is_spawning;
    bool is_reloading;
    animator_t animator;
} weapon_t;

// WEAPON FUNCTIONS
void scale_weapon(weapon_t* weapon, float scale)
{
    weapon->scale = (Vector3){scale, scale, scale};
}

void spawn_weapon(weapon_t* weapon)
{
    weapon->animator.anim_time += GetFrameTime();
    float t = weapon->animator.anim_time / weapon->animator.duration;
    t = 1 - powf(1 - t, 3);

    if (t >= 1.0f)
    {
        t = 1.0f;
        weapon->is_spawning = false;
    }

    weapon->cam.target.y = Lerp(3.5f, 1.95f, t);
}

void reload_weapon(weapon_t* weapon)
{
    weapon->animator.anim_time += GetFrameTime();
    float t = weapon->animator.anim_time / weapon->animator.duration;
    t = (1 - powf(1 - t, 3)) / 2;

    if (t >= 1.0f)
    {
        t = 1.0f;
        weapon->is_reloading = false;
        weapon->rotation_axis = Vector3Zero();

        int needed = 30 - weapon->curr_ammo;
        if (needed > 0)
        {
            int to_reload = (weapon->max_ammo >= needed) ? needed : weapon->max_ammo;
            weapon->curr_ammo += to_reload;
            weapon->max_ammo -= to_reload;
        }
    }
    float angle = sinf(t * PI) * 40.0f;

    weapon->rotation_axis = (Vector3){0, 0, angle};
}

void fire_weapon(Camera main_cam, weapon_t* weapon, BoundingBox cube)
{
    weapon->accuracy -= 0.05f;
    if (weapon->accuracy < 0.1f) weapon->accuracy = 0.1f;

    weapon->last_shot_dt = 0.0f;

    float spread = (1.0f - weapon->accuracy) * weapon->max_spread;
    float rand_yaw = ((float)rand() / RAND_MAX - 0.5f) * spread;
    float rand_pitch = ((float)rand() / RAND_MAX - 0.5f) * spread;

    Vector3 forward = Vector3Normalize(Vector3Subtract(main_cam.target, main_cam.position));
    Matrix yaw_mat = MatrixRotateY(rand_yaw);
    Matrix pitch_mat = MatrixRotate(Vector3CrossProduct(forward, (Vector3){0,1,0}), rand_pitch);
    Vector3 direction = Vector3Transform(forward, yaw_mat);
    direction = Vector3Transform(direction, pitch_mat);

    Ray ray = {
        .position = main_cam.position,
        .direction = direction,
    };

    weapon->curr_ammo = fmax(--weapon->curr_ammo, 0);
    if (weapon->curr_ammo <= 0 && !weapon->is_reloading && weapon->max_ammo > 0)
    {
        weapon->is_reloading = true;
        weapon->animator.duration = 0.5f;
        weapon->animator.anim_time = 0.0f;
    }

    BeginMode3D(main_cam);

    RayCollision hit = GetRayCollisionBox(ray, cube);
    Color color = BLUE;
    if (hit.hit)
        color = BLACK;

    DrawCube(hit.point, 0.1f, 0.1f, 0.1f, color);

    EndMode3D();
}

void draw_weapon(weapon_t weapon)
{
    BeginMode3D(weapon.cam);

    DrawModelEx(weapon.model, weapon.fixed_pos, weapon.rotation_axis, -weapon.rotation_axis.z, weapon.scale, WHITE);

    EndMode3D();

    int crosshair_size = 10;
    Vector2 center = { WIDTH / 2.0f, HEIGHT / 2.0f };

    DrawLine(
        center.x - crosshair_size / 2, center.y,
        center.x + crosshair_size / 2, center.y,
        BLACK
    );
    DrawLine(
        center.x, center.y - crosshair_size / 2,
        center.x, center.y + crosshair_size / 2,
        BLACK
    );

    if (debug)
    {
        float spread = (1.0f - weapon.accuracy) * weapon.max_spread;
        float max_spread = spread * 1000;
        DrawRectangleLines(center.x - max_spread / 2.0f, center.y - max_spread / 2.0f, max_spread, max_spread, ORANGE);
    }
}

void free_weapon(weapon_t weapon)
{
    UnloadModel(weapon.model);
}
// END WEAPON FUNCTIONS

Camera load_cam(Vector3 position, Vector3 target)
{
    Camera cam = {0};
    cam.position = position;
    cam.target = target;
    cam.up = (Vector3){0, 1, 0};
    cam.fovy = 60;
    cam.projection = CAMERA_PERSPECTIVE;

    return cam;
}

// GLOBAL DRAWING FUNCTIONS
void draw_floor(Camera cam, Vector3 cube_pos, float cube_size)
{
    BeginMode3D(cam);

    DrawGrid(10, 1.0f);
    DrawCube(cube_pos, cube_size, cube_size, cube_size, RED);

    EndMode3D();
}

void draw_hud(weapon_t weapon)
{
    int pad_x = 30;
    int pad_y = 20;
    DrawFPS(pad_x, pad_y);

    DrawText(TextFormat("Ammo: %d", weapon.curr_ammo), pad_x, 2 * pad_y, 20, BLACK);
    DrawText(TextFormat("Max ammo: %d", weapon.max_ammo), pad_x, 3 * pad_y, 20, BLACK);
}
// END GLOBAL DRAWING FUNCTIONS

void load_ak(weapon_t* ak47)
{
    ak47->model = LoadModel("./assets/ak47.glb");
    scale_weapon(ak47, 0.002);
    ak47->cam = DEFAULT_AK_VIEWMODEL;
    ak47->rotation_axis = (Vector3){0, 0, 1};
    ak47->fixed_pos = (Vector3){0, 0.7, 0};
    ak47->curr_ammo = 30;
    ak47->max_ammo = 90;
    ak47->accuracy = 1.0f;
    ak47->max_spread = 0.05f;
}

void update_ak(weapon_t* ak47, Camera main_cam, BoundingBox cube_bbox)
{
    ak47->last_shot_dt += GetFrameTime();
    if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT) && ak47->accuracy < 1.0f && ak47->last_shot_dt > 1.0f)
        ak47->accuracy = 1.0f;

    if (IsKeyDown(KEY_M) && !ak47->is_spawning)
    {
        ak47->animator.duration = 0.2f;
        ak47->animator.anim_time = 0.0f;
        ak47->is_spawning = true;
    }
    else if (IsKeyDown(KEY_R) && !ak47->is_reloading && ak47->curr_ammo != 30)
    {
        ak47->animator.duration = 0.5f;
        ak47->animator.anim_time = 0.0f;
        ak47->is_reloading = true;
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && ak47->curr_ammo > 0)
        fire_weapon(main_cam, ak47, cube_bbox);

    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
    {
        main_cam.fovy = 50;
        ak47->cam.position = ZOOMED_AK_VIEWMODEL;
        ak47->cam.target.x = 0;
    }
    else
    {
        main_cam.fovy = 60;
        ak47->cam = DEFAULT_AK_VIEWMODEL;
    }

    if (ak47->is_spawning)
        spawn_weapon(ak47);

    if (ak47->is_reloading)
        reload_weapon(ak47);
}

int main()
{
    InitWindow(WIDTH, HEIGHT, "FPS weapon view");
    SetTargetFPS(60);
    
    Camera main_cam = load_cam((Vector3){0, 2, 4}, (Vector3){0, 2, 3});

    weapon_t ak47 = {0};
    load_ak(&ak47);

    Vector3 cube_pos = Vector3Zero();
    int cube_size = 2;
    BoundingBox cube_bbox = {
        .min = (Vector3){
            cube_pos.x - cube_size / 2,
            cube_pos.y - cube_size / 2,
            cube_pos.z - cube_size / 2,
        },
        .max = (Vector3){
            cube_pos.x + cube_size / 2,
            cube_pos.y + cube_size / 2,
            cube_pos.z + cube_size / 2,
        },
    };

    DisableCursor();

    while (!WindowShouldClose())
    {
        UpdateCamera(&main_cam, CAMERA_FIRST_PERSON);

        if (IsKeyDown(KEY_C))
            debug = !debug;

        BeginDrawing();
        ClearBackground(WHITE);

        draw_hud(ak47);
        draw_floor(main_cam, cube_pos, cube_size);
        draw_weapon(ak47);

        update_ak(&ak47, main_cam, cube_bbox);

        EndDrawing();
    }

    free_weapon(ak47);
    CloseWindow();

    return 0;
}