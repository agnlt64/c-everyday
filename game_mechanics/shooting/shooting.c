#include "../include/raylib.h"
#include "../include/raymath.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define WIDTH 1600
#define HEIGHT 900

#define ENTITY_LIMIT 256
#define NUM_WEAPONS 4

#define DEFAULT_AK_VIEWMODEL load_cam((Vector3){-0.9, 1.95, 1.6}, (Vector3){-1, 1.95, -0.2})
#define ZOOMED_AK_VIEWMODEL (Vector3){0, 2.2, 1}

#define WEAPON_SPAWN_DURATION 0.2f

#define DEFAULT_DEAGLE_VIEWMODEL load_cam((Vector3){-1.3, 1.5, -0.9}, (Vector3){-0.3, 1.5, -0.9})

typedef struct animator {
    float anim_time;
    float duration;
} animator_t;

typedef enum {
    DEAGLE,
    AK47,
} weapon_type;

typedef struct weapon {
    Model model;
    Camera cam;

    Vector3 scale;
    Vector3 rotation_axis;
    Vector3 fixed_pos;

    int max_ammo;
    int curr_ammo;
    int mag_capacity;
    float damage;

    float accuracy;
    float max_spread;
    float last_shot_dt;
    float vertical_spray;
    float shoot_timer;
    float shoot_delay;
    bool can_rifle;

    bool is_spawning;
    bool is_reloading;
    bool is_shooting;
    animator_t animator;
} weapon_t;

typedef struct cube {
    Vector3 pos;
    BoundingBox box;
    float size;
    Color color;
    float speed;
    float hp;
} cube_t;

typedef struct game_context {
    Model map;
    float player_speed;

    cube_t cubes[ENTITY_LIMIT];
    cube_t bullet_impacts[ENTITY_LIMIT];

    int weapon_idx;
    weapon_t weapons[NUM_WEAPONS];
    int last_impact_idx;

    Camera cam;
} game_context;

float get_rand_float()
{
    return (float)rand() / RAND_MAX;
}

// CUBE FUNCTIONS
BoundingBox cube_bbox(cube_t cube)
{
    return (BoundingBox){
        .min = (Vector3){
            cube.pos.x - cube.size / 2,
            cube.pos.y - cube.size / 2,
            cube.pos.z - cube.size / 2,
        },
        .max = (Vector3){
            cube.pos.x + cube.size / 2,
            cube.pos.y + cube.size / 2,
            cube.pos.z + cube.size / 2,
        },
    };
}
// END CUBE FUNCTIONS

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

        int needed = weapon->mag_capacity - weapon->curr_ammo;
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

void fire_weapon(game_context* gc, weapon_t* weapon)
{
    weapon->is_shooting = true;
    weapon->accuracy -= 0.05f;
    if (weapon->accuracy < 0.1f) weapon->accuracy = 0.1f;

    weapon->last_shot_dt = 0.0f;

    float spread = (1.0f - weapon->accuracy) * weapon->max_spread;
    weapon->vertical_spray += spread;

    if (weapon->vertical_spray > 0.15f)
        weapon->vertical_spray = 0.15f;

    float rand_yaw = (get_rand_float() - 0.5f) * spread * 1.4f;

    Vector3 forward = Vector3Normalize(Vector3Subtract(gc->cam.target, gc->cam.position));
    Matrix yaw_mat = MatrixRotateY(rand_yaw);
    Vector3 direction = Vector3Transform(forward, yaw_mat);
    Matrix pitch_mat = MatrixRotate(Vector3CrossProduct(direction, (Vector3){0,1,0}), weapon->vertical_spray);
    direction = Vector3Transform(direction, pitch_mat);

    Ray ray = {
        .position = gc->cam.position,
        .direction = direction,
    };

    weapon->curr_ammo = fmax(--weapon->curr_ammo, 0);
    if (weapon->curr_ammo <= 0 && !weapon->is_reloading && weapon->max_ammo > 0)
    {
        weapon->is_reloading = true;
        weapon->animator.duration = 0.5f;
        weapon->animator.anim_time = 0.0f;
        weapon->vertical_spray = 0.0f;
    }

    for (size_t i = 0; i < ENTITY_LIMIT; i++)
    {
        cube_t cube = gc->cubes[i];
        RayCollision hit = GetRayCollisionBox(ray, cube_bbox(cube));

        if (hit.hit) 
        {
            gc->cubes[i].hp -= weapon->damage;

            cube_t impact = {0};
            impact.size = 0.1f;
            impact.pos = hit.point;
            impact.color = BLACK;
    
            gc->bullet_impacts[gc->last_impact_idx] = impact;
            gc->last_impact_idx = (gc->last_impact_idx + 1) % ENTITY_LIMIT;
            break;
        }
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
void draw_hud(weapon_t weapon)
{
    int pad_x = 30;
    int pad_y = 20;
    DrawFPS(pad_x, pad_y);

    DrawText(TextFormat("Ammo: %d", weapon.curr_ammo), pad_x, 2 * pad_y, 20, BLACK);
    DrawText(TextFormat("Max ammo: %d", weapon.max_ammo), pad_x, 3 * pad_y, 20, BLACK);
}

void draw_crosshair()
{
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
}
// END GLOBAL DRAWING FUNCTIONS

void load_ak(weapon_t* ak47)
{
    ak47->model = LoadModel("./assets/ak47.glb");
    scale_weapon(ak47, 0.002);
    ak47->cam = DEFAULT_AK_VIEWMODEL;
    ak47->rotation_axis = (Vector3){0, 0, 1};
    ak47->fixed_pos = (Vector3){0, 0.7, 0};
    ak47->mag_capacity = 30;
    ak47->curr_ammo = ak47->mag_capacity;
    ak47->max_ammo = 90;
    ak47->damage = 30;

    ak47->accuracy = 1.0f;
    ak47->max_spread = 0.05f;
    ak47->vertical_spray = 0.0f;
    ak47->shoot_timer = 0.0f;
    ak47->shoot_delay = 0.1f;
    ak47->can_rifle = true;
}

void load_deagle(weapon_t* deagle)
{
    deagle->model = LoadModel("./assets/deagle.glb");
    scale_weapon(deagle, 4);
    deagle->cam = DEFAULT_DEAGLE_VIEWMODEL;
    deagle->rotation_axis = Vector3Zero();
    deagle->fixed_pos = (Vector3){0, 0.7, 0};
    deagle->mag_capacity = 7;
    deagle->curr_ammo = deagle->mag_capacity;
    deagle->max_ammo = 35;
    deagle->damage = 20;

    deagle->accuracy = 1.0f;
    deagle->max_spread = 0.001f;
    deagle->vertical_spray = 0.0f;
    deagle->shoot_timer = 0.0f;
    deagle->shoot_delay = 0.5f;
    deagle->can_rifle = false;
}

void update_weapon(game_context* gc)
{
    weapon_t* weapon = &gc->weapons[gc->weapon_idx];
    weapon->last_shot_dt += GetFrameTime();
    
    if (IsKeyDown(KEY_R) &&
        !weapon->is_reloading &&
        weapon->curr_ammo != weapon->mag_capacity && 
        weapon->max_ammo != 0)
    {
        weapon->animator.duration = 0.5f;
        weapon->animator.anim_time = 0.0f;
        weapon->is_reloading = true;
    }

    weapon->shoot_timer += GetFrameTime();

    if (weapon->curr_ammo > 0 && !weapon->is_reloading)
    {
        if (weapon->can_rifle)
        {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            {
                float shoot_interval = weapon->shoot_delay;
                if (weapon->shoot_timer >= shoot_interval)
                {
                    weapon->is_shooting = !weapon->is_reloading;
                    fire_weapon(gc, weapon);
                    weapon->shoot_timer = 0.0f;
                }
            }
            else
            {
                weapon->is_shooting = false;
                weapon->vertical_spray = 0.0f;
                weapon->shoot_timer = weapon->shoot_timer > weapon->shoot_delay ? weapon->shoot_delay : weapon->shoot_timer;
                if (weapon->accuracy < 1.0f && weapon->last_shot_dt > 1.0f)
                    weapon->accuracy = 1.0f;
            }
        }
        else
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                fire_weapon(gc, weapon);
            else
            // todo prevent deagle spam
                weapon->is_shooting = false;
        }
    }

    if (weapon->is_spawning)
        spawn_weapon(weapon);

    if (weapon->is_reloading)
        reload_weapon(weapon);
}

void load_game(game_context* gc)
{
    srand(time(NULL));

    gc->cam = load_cam((Vector3){0, 8, 4}, (Vector3){0, 2, 3});
    gc->map = LoadModel("./assets/map.glb");
    gc->player_speed = 1.0f;
    
    for (size_t i = 0; i < 10; i++)
    {
        cube_t cube = {0};
        cube.pos = (Vector3){i + 2 * get_rand_float(), get_rand_float(), get_rand_float() * 4};
        cube.size = 0.5;
        cube.color = BLUE;
        cube.speed = 0.1f;
        cube.hp = 100;
        cube.box = cube_bbox(cube);
        gc->cubes[i] = cube;
    }

    weapon_t ak47 = {0};
    load_ak(&ak47);
    gc->weapons[AK47] = ak47;

    weapon_t deagle = {0};
    load_deagle(&deagle);
    gc->weapons[DEAGLE] = deagle;

    gc->last_impact_idx = 0;
    gc->weapon_idx = DEAGLE;
}

void draw_game(game_context gc)
{
    weapon_t weapon = gc.weapons[gc.weapon_idx];

    BeginMode3D(gc.cam);

    // floor
    DrawModel(gc.map, Vector3Zero(), 10, WHITE);

    // cubes & bullet impacts
    for (size_t i = 0; i < ENTITY_LIMIT; i++)
    {
        cube_t cube = gc.cubes[i];
        if (cube.hp <= 100 && cube.hp >= 50)
            DrawCube(cube.pos, cube.size, cube.size, cube.size, cube.color);
        else if (cube.hp < 50 && cube.hp >= 20)
            DrawCube(cube.pos, cube.size, cube.size, cube.size, YELLOW);
        else if (cube.hp < 20 && cube.hp > 0)
            DrawCube(cube.pos, cube.size, cube.size, cube.size, RED);

        cube_t impact = gc.bullet_impacts[i];
        if (impact.size > 0.0f)
            DrawCube(impact.pos, impact.size, impact.size, impact.size, impact.color);
    }

    EndMode3D();

    BeginMode3D(weapon.cam);

    // current weapon
    DrawModelEx(weapon.model, weapon.fixed_pos, weapon.rotation_axis, -weapon.rotation_axis.z, weapon.scale, WHITE);

    if (weapon.is_shooting && weapon.curr_ammo > 0)
    {
        if (gc.weapon_idx == AK47)
        {
            Vector3 ray_start = {0, 1.45, -1.5};
            Vector3 ray_dir = Vector3Normalize(Vector3Subtract(weapon.cam.target, ray_start));
            float radius = Clamp(get_rand_float() * 0.5f, 0.1f, 0.3f);
            float ray_len = get_rand_float() * 1.7f;
            Vector3 ray_end = Vector3Add(ray_start, Vector3Scale(ray_dir, ray_len));
    
            DrawSphere(ray_start, radius, ColorAlpha(YELLOW, 0.7));
            DrawLine3D(ray_start, ray_end, ColorAlpha(YELLOW, 0.9));
        }
        else
        {
            Vector3 ray_start = {1.2, 0.9, 0};
            float radius = Clamp(get_rand_float() * 0.5f, 0.1f, 0.2f);
            DrawSphere(ray_start, radius, YELLOW);
        }
    }

    EndMode3D();

    draw_crosshair();
    draw_hud(weapon);
}

void update_player(game_context* gc)
{
    Vector2 mouse_delta = GetMouseDelta();
    static float yaw = 0.0f;
    static float pitch = 0.0f;
    float sensityvity = 0.003f;

    yaw -= mouse_delta.x * sensityvity;
    pitch += mouse_delta.y * sensityvity;

    if (pitch > PI/2.0f) pitch = PI/2.0f;
    if (pitch < -PI/2.0f) pitch = -PI/2.0f;

    Vector3 forward = {
        cosf(pitch) * sinf(yaw),
        -sinf(pitch),
        cosf(pitch) * cosf(yaw)
    };

    Vector3 move_forward = Vector3Normalize((Vector3){forward.x, 0, forward.z});
    Vector3 move_right = Vector3Normalize(Vector3CrossProduct((Vector3){0, 1, 0}, move_forward));

    Vector3 vec_fwd = Vector3Scale(move_forward, gc->player_speed);
    Vector3 vec_right = Vector3Scale(move_right, gc->player_speed);

    if (IsKeyDown(KEY_W))
        gc->cam.position = Vector3Add(gc->cam.position, vec_fwd);
    if (IsKeyDown(KEY_S))
        gc->cam.position = Vector3Subtract(gc->cam.position, vec_fwd);
    if (IsKeyDown(KEY_A))
        gc->cam.position = Vector3Add(gc->cam.position, vec_right);
    if (IsKeyDown(KEY_D))
        gc->cam.position = Vector3Subtract(gc->cam.position, vec_right);

    gc->cam.target = Vector3Add(gc->cam.position, forward);
}

void update_game(game_context* gc)
{
    // UpdateCamera(&(gc->cam), CAMERA_FIRST_PERSON);
    update_player(gc);

    if (IsKeyDown(KEY_ONE) && gc->weapon_idx != DEAGLE)
    {
        gc->weapon_idx = DEAGLE;
        // gc->weapons[DEAGLE].is_spawning = true;
        // gc->weapons[DEAGLE].animator.duration = WEAPON_SPAWN_DURATION;
        // gc->weapons[DEAGLE].animator.anim_time = 0.0f;
    }
    else if (IsKeyDown(KEY_TWO) && gc->weapon_idx != AK47)
    {
        gc->weapon_idx = AK47;
        gc->weapons[AK47].is_spawning = true;
        gc->weapons[AK47].animator.duration = WEAPON_SPAWN_DURATION;
        gc->weapons[AK47].animator.anim_time = 0.0f;
    }

    for (size_t i = 0; i < 10; i++)
    {
        gc->cubes[i].pos.x += gc->cubes[i].speed;
        if (gc->cubes[i].pos.x > 10 || gc->cubes[i].pos.x < -6)
            gc->cubes[i].speed *= -1;
    }

    update_weapon(gc);
}

void free_game(game_context gc)
{
    UnloadModel(gc.map);
    for (size_t i = 0; i < NUM_WEAPONS; i++)
        free_weapon(gc.weapons[i]);
}

int main()
{
    InitWindow(WIDTH, HEIGHT, "Strike Counter");
    SetTargetFPS(60);

    game_context gc = {0};
    load_game(&gc);

    DisableCursor();

    while (!WindowShouldClose())
    {
        update_game(&gc);

        BeginDrawing();

        ClearBackground(WHITE);
        draw_game(gc);

        EndDrawing();
    }

    free_game(gc);
    CloseWindow();

    return 0;
}