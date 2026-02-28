#include <raylib.h>
#include <raymath.h>

#include "dynamic_array.h"

#define REAL_FPS 30
#define TARGET_FPS 480 
#define TARGET_DT (1./TARGET_FPS)
#define GRAVITY 1000.0f
#define MAX_PARTICLES 40

static int particle_counter = 0; 

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

bool circle_ccd(Vector2 *position1, Vector2 *velocity1,
		Vector2 *position2, Vector2 *velocity2,
		float radius, float dt);

void resolve_collision(Vector2 *position1, Vector2 *velocity1,
		       Vector2 *position2, Vector2 *velocity2,
		       float radius);

bool circle_ccd(Vector2 *position1, Vector2 *velocity1,
		Vector2 *position2, Vector2 *velocity2,
		float radius, float dt)
{
  Vector2 relative_position = Vector2Subtract(*position2, *position1);
  Vector2 relative_velocity = Vector2Subtract(*velocity2, *velocity1);
  float radius_sum = 2 * radius;
  
  float a = Vector2DotProduct(relative_velocity, relative_velocity);
  if (a < 1e-8f)
    return false;

  float b = 2 * Vector2DotProduct(relative_position, relative_velocity);
  float c = Vector2DotProduct(relative_position, relative_position) - radius_sum * radius_sum;

  float discriminant = b*b - 4*a*c;
  if (discriminant < 0)
    return false;

  float sqrt_disc = sqrtf(discriminant);
  float t1 = (-b - sqrt_disc) / (2*a);
  float t2 = (-b + sqrt_disc) / (2*a);

  float t = t1;
  if (t < 0 || t > dt)
    t = t2;
  if (t < 0 || t > dt)
    return false;

  *position1 = Vector2Add(*position1, Vector2Scale(*velocity1, t));
  *position2 = Vector2Add(*position2, Vector2Scale(*velocity2, t));
  resolve_collision(position1, velocity1, position2, velocity2, radius); 

  float remaining_dt = dt - t;
  *position1 = Vector2Add(*position1, Vector2Scale(*velocity1, remaining_dt));
  *position2 = Vector2Add(*position2, Vector2Scale(*velocity2, remaining_dt));

  return true; 
}


void resolve_collision(Vector2 *position1, Vector2 *velocity1,
		       Vector2 *position2, Vector2 *velocity2,
		       float radius)
{
  Vector2 collision_vector = Vector2Subtract(*position2, *position1);
  float dist = Vector2Length(collision_vector);
  if (0.f == dist)
    return;
  if (dist >= 2*radius)
    return;

  Vector2 unit_collision_vector = Vector2Scale(collision_vector, 1.f / dist);
  Vector2 relative_velocity = Vector2Subtract(*velocity1, *velocity2);
  float velocity_along_normal = Vector2DotProduct(relative_velocity, unit_collision_vector);

  if (velocity_along_normal > 0)
    return;
  
  float e = 1.f; 
  float j = -(1 + e) * velocity_along_normal / 2;
  Vector2 impulse = Vector2Scale(unit_collision_vector, j);
  
  *velocity1 = Vector2Add(*velocity1, impulse);
  *velocity2 = Vector2Subtract(*velocity2, impulse);

  float overlap = (2 * radius - dist) / 2.f;
  *position1 = Vector2Subtract(*position1, Vector2Scale(unit_collision_vector, overlap));
  *position2 = Vector2Add(*position2, Vector2Scale(unit_collision_vector, overlap));
}


int main(int argc, char **argv)
{
  InitWindow(1080, 720, "Subframes");
  SetTargetFPS(REAL_FPS);

  Vector2s positions  = {0};
  Vector2s velocities = {0};
  Colors colors = {0};

  positions.capacity = MAX_PARTICLES;
  positions.items = malloc(sizeof(Vector2) * positions.capacity);

  velocities.capacity = MAX_PARTICLES;
  velocities.items = malloc(sizeof(Vector2) * velocities.capacity);

  colors.capacity = MAX_PARTICLES;
  colors.items = malloc(sizeof(Color) * colors.capacity);
  
  float radius = 20.f;
    
  int w =  GetScreenWidth();
  int h = GetScreenHeight();

  Vector2 prev_winpos = GetWindowPosition();
  
  bool show_info = true; 
  
  while(!WindowShouldClose())
    {      
      if (IsKeyPressed(KEY_F))
        show_info = !show_info;
      
      float real_dt = GetFrameTime();
      
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && particle_counter < MAX_PARTICLES)
	{
	  if ((GetMousePosition().x > radius) && (GetMousePosition().x < w - radius) &&
	      (GetMousePosition().y > radius) && (GetMousePosition().y < h - radius))
	    {
	      particle_counter++; 
	      DA_APPEND(&positions, GetMousePosition());
	      DA_APPEND(&velocities, ((Vector2) { GetRandomValue(-200, 200), GetRandomValue(-200, 0) }));
	      
	      Color random_color = ColorFromHSV(GetRandomValue(0,360), 0.3, 0.9);
	      DA_APPEND(&colors, random_color);
	    }
	}

      BeginDrawing(); 
      ClearBackground(GetColor(0x181818FF));

      if (show_info)
	{
	  char buffer[64];
	  sprintf(buffer, "Current points: %zu / Max points: %d", positions.count, MAX_PARTICLES);
	  DrawText(buffer, 10, 10, 20, WHITE); 
	}
      
      for (float t = 0; t < real_dt; t += TARGET_DT)
	{ 
	  for (size_t i = 0; i < positions.count; i++)
	    {
	      float value = (TARGET_DT/real_dt)*10.f;
	      velocities.items[i].y += GRAVITY * TARGET_DT;
	      
	      float nx = positions.items[i].x + velocities.items[i].x * TARGET_DT;  	 
	      if (nx - radius <= 0)
		{
		  positions.items[i].x = radius; 
		  velocities.items[i].x *= -1;
		}
	      else if (nx + radius >= w)
		{
		  positions.items[i].x = w - radius; 
		  velocities.items[i].x *= -1;
		}
	      else
		{
		  positions.items[i].x = nx; 
		}
	      
	      float ny = positions.items[i].y + velocities.items[i].y * TARGET_DT;  	 
	      if (ny - radius <= 0)
		{
		  positions.items[i].y = radius; 
		  velocities.items[i].y *= -1;
		}
	      else if (ny + radius >= h)
		{
		  positions.items[i].y = h - radius; 
		  velocities.items[i].y *= -1;
		}
	      else
		{
		  positions.items[i].y = ny; 
		}
	      
	      for (size_t j = i; j < positions.count; j++)
		{
		  if (i == j)
		    continue;
		  
		  circle_ccd(&positions.items[i], &velocities.items[i],
			     &positions.items[j], &velocities.items[j],
			     radius, TARGET_DT);
		}
	
	      if (nx + radius < 0 || nx - radius > w ||
		  ny + radius < 0 || ny - radius > h)
		{
		  DA_REMOVE(&positions, i);
		  DA_REMOVE(&velocities, i);
		  DA_REMOVE(&colors, i);
		  particle_counter--;
		  i--;
		  continue; 
		}

	      float f = t / real_dt;
	      DrawRing(positions.items[i], radius * 0.8f, radius, 0, 360, 100, ColorAlpha(colors.items[i], f));
		
	    }
	}
      
      EndDrawing();
    }
 
  CloseWindow();

  free(positions.items);
  free(velocities.items);
  free(colors.items);

  return 0; 
}
