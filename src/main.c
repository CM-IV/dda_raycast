#include "../include/raylib.h"
#include "../include/raymath.h"
#include <string.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define TARGET_FPS 60
#define MAP_WIDTH 51
#define MAP_HEIGHT 38

static int world_map[MAP_WIDTH * MAP_HEIGHT] = {0};

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "DDA Raycasting");

    Vector2 player = { (float)MAP_WIDTH / 2, (float)MAP_HEIGHT / 2 };
    Vector2 cell_size = { 20, 20 };
    
    SetTargetFPS(TARGET_FPS);

    while (!WindowShouldClose()) {
        float frame_time = GetFrameTime();
        float move_speed = frame_time * 10.0f;

        Vector2 mouse_pos = GetMousePosition();
        Vector2 mouse_cell = Vector2Divide(mouse_pos, cell_size);

        Vector2 ray_start = player;
        Vector2 ray_dir = Vector2Normalize(Vector2Subtract(mouse_cell, player));
        Vector2 ray_len_1d;

        
        Vector2 ray_unit_step_sz = {
            sqrt(1 + (ray_dir.y / ray_dir.x) * (ray_dir.y / ray_dir.x)),
            sqrt(1 + (ray_dir.x / ray_dir.y) * (ray_dir.x / ray_dir.y))
        };

        int map_check_x = ray_start.x;
        int map_check_y = ray_start.y;
        int step_x, step_y;
        int tile_found = 0;
        float distance = 0.0f;

        // key events
        if (IsKeyDown(KEY_D)) player.x += move_speed;
        if (IsKeyDown(KEY_A)) player.x -= move_speed;
        if (IsKeyDown(KEY_W)) player.y -= move_speed;
        if (IsKeyDown(KEY_S)) player.y += move_speed;
        
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            int cell_x = (int)mouse_cell.x;
            int cell_y = (int)mouse_cell.y;
            
            if (cell_x >= 0 && cell_x < MAP_WIDTH && cell_y >= 0 && cell_y < MAP_HEIGHT) {
                world_map[cell_y * MAP_WIDTH + cell_x] = 1;
            }
        }

        if (IsKeyPressed(KEY_C)) {
            memset(world_map, 0, sizeof(world_map));
        }
        
        BeginDrawing();
        ClearBackground(BLACK);
        
        // draw the grid
        for (int y = 0; y < MAP_HEIGHT; y++) {
            for (int x = 0; x < MAP_WIDTH; x++) {
                int screen_x = x * cell_size.x;
                int screen_y = y * cell_size.y;
                
                Color tileColor = DARKGRAY;
                if (world_map[(y) * MAP_WIDTH + (x)] == 1) {
                    tileColor = BLUE;
                }
                
                DrawRectangle(screen_x, screen_y, cell_size.x, cell_size.y, tileColor);
                
                // draw grid lines
                DrawRectangleLines(screen_x, screen_y, cell_size.x, cell_size.y, GRAY);

            }
        }

        // draw the player
        Vector2 player_screen = Vector2Multiply(player, cell_size);
        DrawCircleV(player_screen, 20, RED);

        // draw the mouse
        DrawCircleV(mouse_pos, 20, GREEN);

        if (ray_dir.x < 0) {
            step_x = -1;
            ray_len_1d.x = (ray_start.x - map_check_x) * ray_unit_step_sz.x;
        } else {
            step_x = 1;
            ray_len_1d.x = ((map_check_x + 1.0f) - ray_start.x) * ray_unit_step_sz.x;
        }

        if (ray_dir.y < 0) {
            step_y = -1;
            ray_len_1d.y = (ray_start.y - map_check_y) * ray_unit_step_sz.y;
        } else {
            step_y = 1;
            ray_len_1d.y = ((map_check_y + 1.0f) - ray_start.y) * ray_unit_step_sz.y;
        }


        while (tile_found == 0 && distance < SCREEN_WIDTH) {
            if (ray_len_1d.x < ray_len_1d.y) {
                map_check_x += step_x;
                distance = ray_len_1d.x;
                ray_len_1d.x += ray_unit_step_sz.x;
            } else {
                map_check_y += step_y;
                distance = ray_len_1d.y;
                ray_len_1d.y += ray_unit_step_sz.y;
            }

            // collision here
            if (map_check_x >= 0 && map_check_x < MAP_WIDTH && map_check_y >= 0  && map_check_y < MAP_HEIGHT) {
                if (world_map[map_check_y * MAP_WIDTH + map_check_x] == 1) {
                    tile_found = 1;
                }
            }
        }

        Vector2 intersection;
        if (tile_found == 1) {
            intersection = Vector2Add(ray_start, Vector2Scale(ray_dir, distance));
            Vector2 intersection_screen = Vector2Multiply(intersection, cell_size);
            if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
                DrawLineV(player_screen, mouse_pos, PINK);
                DrawCircleV(intersection_screen, 8.0f, YELLOW);
            }
        } else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            DrawLineV(player_screen, mouse_pos, PINK);
        }

        
        DrawFPS(10, 10);
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}
