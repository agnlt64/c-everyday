#include "../include/raylib.h"
#include "../include/raymath.h"

#define WIDTH 1600
#define HEIGHT 900

typedef struct weapon {
    Model model;
    Camera cam;

    Vector3 scale;
    Vector3 rotation_axis;
    float rotation_angle;
    Vector3 fixed_pos;
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

    DrawModelEx(weapon.model, weapon.fixed_pos, weapon.rotation_axis, weapon.rotation_angle, weapon.scale, WHITE);

    EndMode3D();
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
    Camera ak_cam = load_cam(Vector3Zero(), (Vector3){0, 0, 1});

    weapon_t ak47 = {0};
    ak47.model = LoadModel("./assets/ak47.glb");
    ak47.cam = ak_cam;
    ak47.scale = (Vector3){0.06, 0.06, 0.06};
    ak47.rotation_axis = (Vector3){0, 1, 1};
    ak47.rotation_angle = 180;
    ak47.fixed_pos = (Vector3){-0.2, -0.2, 0.5};

    while (!WindowShouldClose())
    {
        HideCursor();
        UpdateCamera(&main_cam, CAMERA_FIRST_PERSON);

        BeginDrawing();
        ClearBackground(WHITE);

        draw_floor(main_cam);
        draw_weapon(ak47);

        EndDrawing();
    }

    free_weapon(ak47);
    CloseWindow();

    return 0;
}