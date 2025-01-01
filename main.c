#include "raylib.h"

typedef enum GameScreen { LOGO = 0, TITLE, GAMEPLAY, ENDING } GameScreen;

#define AMMO_CAPACITY 6

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 450;

struct GameState {
  int frameCounter;
  GameScreen screen;
  // Player
  Vector2 pos;
  const int radius;
  struct Vector2 clip[AMMO_CAPACITY];
  int bullets;
  struct Vector2 targets[AMMO_CAPACITY];
  struct Color targetsHit[AMMO_CAPACITY];
};

void DrawTargets(const int width, const int height, const Vector2 *targets,
                 const Color *targetsHit);
void DrawReticle(Vector2 *pos, const int *radius, Vector2 *clip, int bullets);

int main(void) {

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello, Raylib!");
  // Initial Variables
  //-----------------------------------------------------------------------------------
  struct GameState state = {.frameCounter = 0,
                            .screen = GAMEPLAY,
                            .pos = 0,
                            .radius = 20,
                            .clip = {{0}, {0}, {0}, {0}, {0}, {0}},
                            .bullets = 0};
  for (int i = 0; i < AMMO_CAPACITY; i++) {
    const struct Vector2 randomPos = {.x = GetRandomValue(10, SCREEN_WIDTH),
                                      .y = GetRandomValue(10, SCREEN_HEIGHT)};
    state.targets[i] = randomPos;
    state.targetsHit[i] = RED;
  }
  const int targetH = 25;
  const int targetW = 50;
  //-----------------------------------------------------------------------------------
  SetTargetFPS(60);
  HideCursor();

  while (!WindowShouldClose()) {

    BeginDrawing();
    ClearBackground(RAYWHITE);
    switch (state.screen) {
    case LOGO: {
      // TODO: Draw LOGO screen here
      state.frameCounter++;
      if (state.frameCounter > 180) {
        state.screen = TITLE;
      }

      DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);
      DrawText("WELCOME TOO POP SHOT", 20, 20, 40, RAYWHITE);
      DrawText("There are the qick and there are the dead!", 290, 220, 20,
               GRAY);
    } break;
    case TITLE: {
      // TODO: Draw TITLE screen here!
      if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
        state.screen = GAMEPLAY;
      }
      DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);
      DrawText("Press Space to Begin!", 20, 20, 40, LIGHTGRAY);
      DrawText("Instructions....", 120, 220, 20, RAYWHITE);
    } break;
    case GAMEPLAY: {
      // TODO: Draw GAMEPLAY screen here
      if (IsCursorOnScreen()) {
        state.pos = GetMousePosition();
        if (IsGestureDetected(GESTURE_TAP)) {

          // printf("Player bullets: %d\n", state.bullets);
          if (state.bullets != AMMO_CAPACITY) {
            state.clip[state.bullets] = GetMousePosition();
            state.bullets++;
          }

          for (int i = 0; i < AMMO_CAPACITY; i++) {
            const struct Vector2 origin = state.pos;
            const struct Rectangle target = {
                .x = state.targets[i].x,
                .y = state.targets[i].y,
                .width = targetW,
                .height = targetH,
            };
            const _Bool collision = CheckCollisionPointRec(origin, target);
            if (collision) {
              state.targetsHit[i] = GREEN;
            }
          }
          if (state.bullets == AMMO_CAPACITY) {
            state.screen = ENDING;
          }
        }
      }

      DrawTargets(targetW, targetH, state.targets, state.targetsHit);
      DrawReticle(&state.pos, &state.radius, state.clip, state.bullets);
    } break;
    case ENDING: {
      // TODO: Draw ENDING screen here!
      DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLUE);
      DrawText("ENDING SCREEN", 20, 20, 40, DARKBLUE);
      DrawText("PRESS ENTER or TAP to RETURN to TITLE SCREEN", 120, 220, 20,
               DARKBLUE);
    } break;
    default:
      break;
    }
    EndDrawing();
    //----------------------------------------------------------------------------------
  }

  //----------------------------------------------------------------------------------
  // Free
  //----------------------------------------------------------------------------------
  CloseWindow();
  //----------------------------------------------------------------------------------
  return 0;
}

void DrawTargets(const int width, const int height, const Vector2 *targets,
                 const Color *targetsHit) {
  for (int i = 0; i < AMMO_CAPACITY; i++) {
    const Vector2 targetWH = {.x = width, .y = height};
    DrawRectangleV(targets[i], targetWH, targetsHit[i]);
  };
}

void DrawReticle(Vector2 *pos, const int *radius, Vector2 *clip, int bullets) {
  DrawCircleLinesV(*pos, *radius, BLACK);
  DrawLine(pos->x - *radius, pos->y, pos->x + *radius, pos->y, BLACK);
  DrawLine(pos->x, pos->y + *radius, pos->x, pos->y - *radius, BLACK);
}
