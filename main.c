#include <raylib.h>
#include <raymath.h>

#include "dynamic_array.h"

#define PLAYER_SPEED 1600
#define REAL_FPS 30
#define TARGET_FPS 480 
#define TARGET_DT (1./TARGET_FPS)
#define GRAVITY 1000.0f

typedef struct {
  int key;
  Vector2 vec;
} Control; 

static Control controls[] = 
  {
    {KEY_S, {0, 1}},
    {KEY_W, {0, -1}},
    {KEY_A, {-1, 0}},
    {KEY_D, {1, 0}}
  };
static size_t length_controls = sizeof(controls) / sizeof(controls[0]);

typedef struct {
  Vector2 *items;
  size_t count;
  size_t capacity; 
} Vector2s;

typedef struct {
  Color *items;
  size_t count;
  size_t capacity; 
}Colors; 

int main(int argc, char **argv)
{
  InitWindow(800, 600, "Subframes");
  SetTargetFPS(REAL_FPS);

  Vector2s positions  = {0};
  Vector2s velocities = {0};
  Colors colors = {0};
  
  float radius = 20.f;
    
  int w =  GetScreenWidth();
  int h = GetScreenHeight();

  while(!WindowShouldClose())
    {
      float real_dt = GetFrameTime();

      BeginDrawing();
      ClearBackground(GetColor(0x181818FF));

      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
	  DA_APPEND(&positions, GetMousePosition());
	  DA_APPEND(&velocities, ((Vector2) {500, 500}));
	  DA_APPEND(&colors, ((Color) {200, 122, 255, 255}));
	}
      
      for (float t = 0.; t < real_dt; t += TARGET_DT)
	{
	  for (size_t i = 0; i < positions.count; ++i)
	    {
	      velocities.items[i].y += GRAVITY*TARGET_DT;
	      float nx = positions.items[i].x + velocities.items[i].x*TARGET_DT;
	      if (nx - radius <= 0 || nx + radius >= w)
		{
		  velocities.items[i].x *= -1;
		}
	      else
		{
		  positions.items[i].x = nx;
		}

	      float ny = positions.items[i].y + velocities.items[i].y*TARGET_DT;
	      if (ny - radius <= 0 || ny + radius >= h)
		{
		  velocities.items[i].y *= -1;
		}
	      else
		{
		  positions.items[i].y = ny;
		}

	      float f = t/real_dt;
	      DrawRing(positions.items[i], radius*0.8f, radius, 0, 360, 100, ColorAlpha(colors.items[i], f));
	    }
	}

      EndDrawing(); 
    }

  CloseWindow(); 
  return 0; 
}
