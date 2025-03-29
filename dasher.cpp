#include "raylib.h"

struct AnimData
{
  Rectangle rec;
  Vector2 pos;
  int frame;
  float updateTime;
  float runningTime;
};

bool isOnGround(AnimData data, int windowHeight)
{
  return data.pos.y >= windowHeight - data.rec.height;
}

AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame)
{
  // update running time
  data.runningTime += deltaTime;
  if (data.runningTime >= data.updateTime)
  {
    data.runningTime = 0.0;
    // update animation frame
    data.rec.x = data.frame * data.rec.width;
    data.frame++;
    if (data.frame > maxFrame)
    {
      data.frame = 0;
    }
  }
  return data;
}

int main()
{
  // window dimensions
  int windowDimensions[2];
  windowDimensions[0] = 512;
  windowDimensions[1] = 380;

  InitWindow(windowDimensions[0], windowDimensions[1], "Dapper Dasher");

  // acceleration due to gravity value (pixels/second) per second
  // every second, velocity will change this amount per second
  const int gravity{1'000};

  // nebula hazard vars
  Texture2D nebula = LoadTexture("textures/nebula_spritesheet.png");

  const int sizeOfNebulae{3};
  AnimData nebulae[sizeOfNebulae]{};

  for (int i = 0; i < sizeOfNebulae; i++)
  {
    nebulae[i].rec.x = 0.0;
    nebulae[i].rec.y = 0.0;
    nebulae[i].rec.width = nebula.width / 8;
    nebulae[i].rec.height = nebula.height / 8;
    nebulae[i].pos.y = windowDimensions[1] - nebula.height / 8;
    nebulae[i].frame = 0;
    nebulae[i].runningTime = 0.0;
    nebulae[i].updateTime = 1.0 / 16.0;
    nebulae[i].pos.x = windowDimensions[0] + i * 300;
  }

  float finishLine{nebulae[sizeOfNebulae - 1].pos.x};

  int nebulaVelocity{-200}; // neb x velocity (pixels/second)

  // scarfy
  Texture2D scarfy = LoadTexture("textures/scarfy.png");

  AnimData scarfyData;
  scarfyData.rec.width = scarfy.width / 6;
  scarfyData.rec.height = scarfy.height;
  scarfyData.rec.x = 0;
  scarfyData.rec.y = 0;
  scarfyData.pos.x = windowDimensions[0] / 2 - scarfyData.rec.width / 2;
  scarfyData.pos.y = windowDimensions[1] - scarfyData.rec.height;
  scarfyData.frame = 0;
  scarfyData.updateTime = 1.0 / 12.0;
  scarfyData.runningTime = 0.0;

  // check if object is in the air
  bool isInAir{};
  // jump velocity
  const int jumpVel{-600}; // pixels/second instead of frame

  int velocity{0}; // measured in pixels per frame ( distance / time ) ex. 20 pixels in 2 frames = 10 pixels per frame

  Texture2D background = LoadTexture("textures/far-buildings.png");
  float bgX{};

  Texture2D midground = LoadTexture("textures/back-buildings.png");
  float mgX{};

  Texture2D foreground = LoadTexture("textures/foreground.png");
  float fgX{};

  bool collision{};

  SetTargetFPS(60);

  while (!WindowShouldClose()) // while loop is processed every frame
  {
    // initializing delta time
    const float dT{GetFrameTime()};

    BeginDrawing();

    ClearBackground(WHITE);

    // scroll the background
    bgX -= 20 * dT;
    if (bgX <= -background.width * 2)
    {
      bgX = 0.0;
    }

    // scroll the midground
    mgX -= 40 * dT;
    if (mgX <= -midground.width * 2)
    {
      mgX = 0.0;
    }

    // scroll the foreground
    fgX -= 80 * dT;
    if (fgX <= -foreground.width * 2)
    {
      fgX = 0.0;
    }

    // Draw Background

    Vector2 bg1Pos{bgX, 0.0};
    DrawTextureEx(background, bg1Pos, 0.0, 2.0, WHITE);
    Vector2 bg2Pos{bgX + background.width * 2, 0.0};
    DrawTextureEx(background, bg2Pos, 0.0, 2.0, WHITE);

    // Draw Midground
    Vector2 mg1Pos{mgX, 0.0};
    DrawTextureEx(midground, mg1Pos, 0.0, 2.0, WHITE);
    Vector2 mg2Pos{mgX + midground.width * 2, 0.0};
    DrawTextureEx(midground, mg2Pos, 0.0, 2.0, WHITE);

    // Draw Foreground
    Vector2 fg1Pos{fgX, 0.0};
    DrawTextureEx(foreground, fg1Pos, 0.0, 2.0, WHITE);
    Vector2 fg2Pos{fgX + foreground.width * 2, 0.0};
    DrawTextureEx(foreground, fg2Pos, 0.0, 2.0, WHITE);

    // perform ground check
    if (isOnGround(scarfyData, windowDimensions[1]))
    {
      // rectangle is on the ground
      isInAir = false;
      velocity = 0;
    }
    else
    {
      // apply gravity because the object is in the air
      isInAir = true;
      velocity += gravity * dT; // multiply by delta time to convert value to time instead of frame
    }

    // jump check
    if (!isInAir && IsKeyPressed(KEY_SPACE))
    {
      // negative value to make object move upward upon space key being pressed
      velocity += jumpVel;
    }

    for (int i = 0; i < sizeOfNebulae; i++)
    {
      // update nebula position (initializes off screen to the right and moves -x to the left)
      nebulae[i].pos.x += nebulaVelocity * dT;
    }

    // update finish line
    finishLine += nebulaVelocity * dT;

    // update scarfy position
    scarfyData.pos.y += velocity * dT; // multiply by delta time to convert value to time instead of frame

    // update scarfy animation
    if (!isInAir)
    {
      scarfyData = updateAnimData(scarfyData, dT, 5);
    }

    for (int i = 0; i < sizeOfNebulae; i++)
    {
      nebulae[i] = updateAnimData(nebulae[i], dT, 7);
    }

    for (AnimData nebula : nebulae)
    {
      float pad{50};
      Rectangle nebRec{
          nebula.pos.x + pad,
          nebula.pos.y + pad,
          nebula.rec.width - 2 * pad,
          nebula.rec.height - 2 * pad,
      };
      Rectangle scarfyRec{
          scarfyData.pos.x,
          scarfyData.pos.y,
          scarfyData.rec.width,
          scarfyData.rec.height,
      };
      if (CheckCollisionRecs(nebRec, scarfyRec))
      {
        collision = true;
      }
    }

    if (collision)
    {
      // lose the game
      DrawText("Game Over!", windowDimensions[0] / 4 + 30, windowDimensions[1] / 2, 40, RED);
    }
    else if (scarfyData.pos.x >= finishLine)
    {
      // won the game
      DrawText("You Win!", windowDimensions[0] / 4 + 40, windowDimensions[1] / 2, 40, GREEN);
    }
    else
    {
      for (int i = 0; i < sizeOfNebulae; i++)
      {
        // draw nebula hazard
        DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
      }
      DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
    }
    EndDrawing();
  }
  UnloadTexture(scarfy);
  UnloadTexture(nebula);
  UnloadTexture(background);
  UnloadTexture(midground);
  UnloadTexture(foreground);
  CloseWindow();
}