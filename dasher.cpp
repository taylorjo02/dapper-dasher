#include "raylib.h"

struct AnimData
{
  Rectangle rec;
  Vector2 pos;
  int frame;
  float updateTime;
  float runningTime;
};

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

  // AnimData for Nebula
  AnimData nebData{
      {0.0, 0.0, nebula.width / 8, nebula.height / 8}, // rectangle rec
      {windowDimensions[0], windowDimensions[1] - nebula.height / 8}, // vector 2 pos
      0,                                               // int frame
      1.0 / 12.0,                                      // float updatetime
      0.0                                              // float runningTime
  };

  AnimData neb2Data{
      {0.0, 0.0, nebula.width / 8, nebula.height / 8},       // rectangle rec
      {windowDimensions[0] + 300, windowDimensions[1] - nebula.height / 8}, // vector 2 pos
      0,                                                     // int frame
      1.0 / 16.0,                                            // float updatetime
      0.0                                                    // float runningTime
  };

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

  SetTargetFPS(60);

  while (!WindowShouldClose()) // while loop is processed every frame
  {
    // initializing delta time
    const float dT{GetFrameTime()};

    BeginDrawing();

    ClearBackground(WHITE);

    // perform ground check
    if (scarfyData.pos.y >= windowDimensions[1] - scarfyData.rec.height)
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

    // update nebula position (initializes off screen to the right and moves -x to the left)
    nebData.pos.x += nebulaVelocity * dT;

    // update 2nd neb position
    neb2Data.pos.x += nebulaVelocity * dT;

    // update scarfy position
    scarfyData.pos.y += velocity * dT; // multiply by delta time to convert value to time instead of frame

    // update scarfy animation
    if (!isInAir)
    {
      // update running time
      scarfyData.runningTime += dT;

      // update animation frame
      if (scarfyData.runningTime >= scarfyData.updateTime)
      {
        scarfyData.runningTime = 0.0;
        scarfyData.rec.x = scarfyData.frame * scarfyData.rec.width;
        scarfyData.frame++;
        if (scarfyData.frame > 5)
        {
          scarfyData.frame = 0;
        }
      }
    }

    // update first nebula animation
    nebData.runningTime += dT;

    if (nebData.runningTime >= nebData.updateTime)
    {
      nebData.runningTime = 0.0;
      nebData.rec.x = nebData.frame * nebData.rec.width;
      nebData.frame++;
      if (nebData.frame > 7)
      {
        nebData.frame = 0;
      }
    }

    // update 2nd nebula animation
    neb2Data.runningTime += dT;

    if (neb2Data.runningTime >= neb2Data.updateTime)
    {
      neb2Data.runningTime = 0.0;
      neb2Data.rec.x = neb2Data.frame * neb2Data.rec.width;
      neb2Data.frame++;
      if (neb2Data.frame > 7)
      {
        neb2Data.frame = 0;
      }
    }

    // draw nebula hazard
    DrawTextureRec(nebula, nebData.rec, nebData.pos, WHITE);
    // 2nd nebula
    DrawTextureRec(nebula, neb2Data.rec, neb2Data.pos, RED);

    // draw scarfy
    DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);

    EndDrawing();
  }
  UnloadTexture(scarfy);
  UnloadTexture(nebula);
  CloseWindow();
}