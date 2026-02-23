#include <raylib.h>
#include <raymath.h>

#define PLAYER_SPEED 1600
#define REAL_FPS 30
#define TARGET_FPS 480 
#define TARGET_DT (1./TARGET_FPS)

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

int main(int argc, char **argv)
{
  InitWindow(800, 600, "Subframes");
  SetTargetFPS(REAL_FPS);

  Vector2s positions = {0};
  float radius = 20.f;
    
  int w =  GetScreenWidth();
  int h = GetScreenHeight();

  Camera2D camera =
    {
      .zoom = 1.f,
      .offset = { w/2, h/2 }
    };

  while(!WindowShouldClose())
    {
      float real_dt = GetFrameTime();

      BeginDrawing();
      ClearBackground(GetColor(0x181818FF));
      BeginMode2D(camera);
      
      for (float t = 0.; t < real_dt; t += TARGET_DT)
	{
	  Vector2 velocity = {0, 0};
	  float f = real_dt / TARGET_DT;
	
	  for (size_t i = 0; i < length_controls; ++i)
	    {
	      if (IsKeyDown(controls[i].key))
		{
		  velocity = Vector2Add(velocity, controls[i].vec); 
		}
	    }
	  
	  velocity = Vector2Normalize(velocity);
	  velocity = Vector2Scale(velocity, PLAYER_SPEED);
	  position = Vector2Add(position, Vector2Scale(velocity, TARGET_DT));
	  
	  DrawRing(position, radius*0.8f, radius, 0, 360, 100, RED);
	}

      EndMode2D();
      EndDrawing(); 
    }

  CloseWindow(); 
  return 0; 
}
