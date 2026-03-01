#include <stdio.h>
#include <stdlib.h> 
#include <raylib.h>
#include <raymath.h>

#include "dynamic_array.h"

#define REAL_FPS 30
#define TARGET_FPS 480.0f
#define TARGET_DT (1.0f / TARGET_FPS)
#define GRAVITY 1000.0f
#define MAX_PARTICLES 50

typedef struct {
    Vector2 items[MAX_PARTICLES];
    size_t count;
    size_t capacity;
} Vector2s;

typedef struct {
    Color items[MAX_PARTICLES];
    size_t count;
    size_t capacity;
} Colors;

void resolve_collision(Vector2 *position1, Vector2 *velocity1,
                       Vector2 *position2, Vector2 *velocity2,
                       float radius)
{
    Vector2 collision_vector = Vector2Subtract(*position2, *position1);
    float dist = Vector2Length(collision_vector);
    float combined_radius = radius * 2;

    if (dist == 0.0f) return;
    if (dist >= combined_radius) return;

    Vector2 unit_collision_vector = Vector2Scale(collision_vector, 1.0f / dist);
    float overlap = combined_radius - dist;

    Vector2 separation = Vector2Scale(unit_collision_vector, overlap * 0.5f);
    *position1 = Vector2Subtract(*position1, separation);
    *position2 = Vector2Add(*position2, separation);

    Vector2 relative_velocity = Vector2Subtract(*velocity1, *velocity2);
    float velocity_along_normal = Vector2DotProduct(relative_velocity, unit_collision_vector);

    if (velocity_along_normal > 0) return;

    float e = 0.8f;

    if (velocity_along_normal > -100.0f * TARGET_DT)
      {
        e = 0.0f;
      }
    
    float j = -(1 + e) * velocity_along_normal / 2.0f;

    Vector2 impulse = Vector2Scale(unit_collision_vector, j);
    *velocity1 = Vector2Add(*velocity1, impulse);
    *velocity2 = Vector2Subtract(*velocity2, impulse);
}

int main(int argc, char **argv)
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1080, 720, "Subframes Physics");
    SetTargetFPS(REAL_FPS);

    Vector2s positions = {0};
    Vector2s velocities = {0};
    Colors colors = {0};

    positions.capacity = MAX_PARTICLES;

    velocities.capacity = MAX_PARTICLES;

    colors.capacity = MAX_PARTICLES;

    float radius = 20.0f;
    int particle_counter = 0;
    bool show_info = true;

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_F)) show_info = !show_info;

        int w = GetScreenWidth();
        int h = GetScreenHeight();

        float real_dt = GetFrameTime();
	
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && particle_counter < MAX_PARTICLES)
        {
            Vector2 mouse_position = GetMousePosition();
            if ((mouse_position.x > radius) && (mouse_position.x < w - radius) &&
                (mouse_position.y > radius) && (mouse_position.y < h - radius))
            {
                particle_counter++;
                DA_APPEND(&positions, mouse_position);
                Vector2 random_velocity = { (float)GetRandomValue(-200, 200), (float)GetRandomValue(-200, 0) };
                DA_APPEND(&velocities, random_velocity);
                Color random_color = ColorFromHSV((float)GetRandomValue(0, 360), 0.7f, 0.9f);
                DA_APPEND(&colors, random_color);
            }
        }

        for (float t = 0; t < real_dt; t += TARGET_DT)
        {
            for (size_t i = 0; i < positions.count; i++)
            {
                velocities.items[i].y += GRAVITY * TARGET_DT;
                positions.items[i].x += velocities.items[i].x * TARGET_DT;
                positions.items[i].y += velocities.items[i].y * TARGET_DT;
            }

	    int solver_iterations = 8;
	    for (int k = 0; k < solver_iterations; k++)
	      {
		for (size_t i = 0; i < positions.count; i++)
		  {
		    if (positions.items[i].x - radius <= 0)
		      {
			positions.items[i].x = radius;
			if (velocities.items[i].x < 0)
			  velocities.items[i].x *= -0.8f; 
		      }
		    else if (positions.items[i].x + radius >= w)
		      {
			positions.items[i].x = w - radius;
			if (velocities.items[i].x > 0)
			  velocities.items[i].x *= -0.8f;
		      }
		
		    if (positions.items[i].y - radius <= 0)
		      {
			positions.items[i].y = radius;
			if (velocities.items[i].y < 0)
			  velocities.items[i].y *= -0.8f;
		      }
		    else if (positions.items[i].y + radius >= h)
		      {
			positions.items[i].y = h - radius;
			if (velocities.items[i].y > 0)
			  velocities.items[i].y *= -0.8f;
		      }
		  }
		
		for (size_t i = 0; i < positions.count; i++)
		  {
		    for (size_t j = i + 1; j < positions.count; j++)
		      {
			resolve_collision(&positions.items[i], &velocities.items[i],
					  &positions.items[j], &velocities.items[j],
					  radius);
		      }
		  }
	      }
        }

        BeginDrawing();
        ClearBackground(GetColor(0x181818FF));

        if (show_info)
        {
            char buffer[128];
            sprintf(buffer, "Points: %zu/%d | Target Physics FPS: %.0f", 
                    positions.count, MAX_PARTICLES, TARGET_FPS);
            DrawText(buffer, 10, 10, 20, WHITE);
        }

        for (size_t i = 0; i < positions.count; i++)
        {
            DrawRing(positions.items[i], radius * 0.8f, radius, 0, 360, 36, colors.items[i]);
        }

        EndDrawing();
    }

    CloseWindow();
    
    return 0;
}
