#include "../include/raylib.h"
#include "../include/raymath.h"

#define WIDTH 1600
#define HEIGHT 900

#define DEFAULT_AK_VIEWMODEL load_cam((Vector3){-0.9, 1.95, 1.6}, (Vector3){-1, 1.95, -0.2})

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

    bool is_spawning;
    bool is_reloading;
    animator_t animator;
} weapon_t;

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

void draw_floor(Camera cam)
{
    BeginMode3D(cam);

    DrawGrid(10, 1.0f);

    EndMode3D();
}

void draw_weapon(weapon_t weapon)
{
    BeginMode3D(weapon.cam);

    DrawModelEx(weapon.model, weapon.fixed_pos, weapon.rotation_axis, -weapon.rotation_axis.z, weapon.scale, WHITE);

    EndMode3D();
}

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
    }
    float angle = sinf(t * PI) * 40.0f;

    weapon->rotation_axis = (Vector3){0, 0, angle};
}

void free_weapon(weapon_t weapon)
{
    UnloadModel(weapon.model);
}

int main()
{
    InitWindow(WIDTH, HEIGHT, "FPS weapon view");
    SetTargetFPS(60);
    
    Camera main_cam = load_cam((Vector3){0, 2, 4}, (Vector3){0, 2, 3});

    weapon_t ak47 = {0};
    ak47.model = LoadModel("./assets/ak47.glb");
    scale_weapon(&ak47, 0.002);
    ak47.cam = DEFAULT_AK_VIEWMODEL;
    ak47.rotation_axis = (Vector3){0, 0, 1};
    ak47.fixed_pos = (Vector3){0, 0.7, 0};

    DisableCursor();

    while (!WindowShouldClose())
    {
        UpdateCamera(&main_cam, CAMERA_FIRST_PERSON);

        BeginDrawing();
        ClearBackground(WHITE);

        draw_floor(main_cam);
        draw_weapon(ak47);

        if (IsKeyDown(KEY_M) && !ak47.is_spawning)
        {
            ak47.animator.duration = 0.2f;
            ak47.animator.anim_time = 0.0f;
            ak47.is_spawning = true;
        }
        else if (IsKeyDown(KEY_R) && !ak47.is_reloading)
        {
            ak47.animator.duration = 0.5f;
            ak47.animator.anim_time = 0.0f;
            ak47.is_reloading = true;
        }

        if (ak47.is_spawning)
            spawn_weapon(&ak47);

        if (ak47.is_reloading)
            reload_weapon(&ak47);

        EndDrawing();
    }

    free_weapon(ak47);
    CloseWindow();

    return 0;
}