#include <raylib.h>

int main()
{
  // window dimensions
  const int windowWidth{512};
  const int windowHeight{380};

  InitWindow(windowWidth, windowHeight, "Dapper Dasher");

  // acceleration due to gravity value (pixels/second) per second
  // every second, velocity will change this amount per second
  const int gravity{1'000};

  // nebula hazard vars
  Texture2D nebula = LoadTexture("textures/nebula_spritesheet.png");

  Rectangle nebulaRectangle{0.0, 0.0, nebula.width / 8, nebula.height / 8};

  Vector2 nebulaPosition{windowWidth, windowHeight - nebulaRectangle.height};

  int nebulaVelocity{-200}; // neb x velocity (pixels/second)

  // scarfy
  Texture2D scarfy = LoadTexture("textures/scarfy.png");

  Rectangle scarfyRectangle;
  scarfyRectangle.width = scarfy.width / 6;
  scarfyRectangle.height = scarfy.height;
  scarfyRectangle.x = 0;
  scarfyRectangle.y = 0;

  Vector2 scarfyPosition;

  scarfyPosition.x = windowWidth / 2 - scarfyRectangle.width / 2;
  scarfyPosition.y = windowHeight - scarfyRectangle.height;

  // nebula animation frame
  int nebFrame{};

  const float nebUpdateTime{1.0 / 12.0};

  float nebRunningTime{};

  // scarfy animation frame
  int frame{};
  // amount of time before we update animation frame
  const float updateTime{1.0 / 12.0};

  float runningTime{};
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
    if (scarfyPosition.y >= windowHeight - scarfyRectangle.height)
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
    nebulaPosition.x += nebulaVelocity * dT;

    // update scarfy position
    scarfyPosition.y += velocity * dT; // multiply by delta time to convert value to time instead of frame

    
    // update scarfy
    if (!isInAir)
    {
      // update running time
      runningTime += dT;
      
      // update animation frame
      if (runningTime >= updateTime)
      {
        runningTime = 0.0;
        scarfyRectangle.x = frame * scarfyRectangle.width;
        frame++;
        if (frame > 5)
        {
          frame = 0;
        }
      }
    }
    
    nebRunningTime += dT;
    
    if (nebRunningTime >= nebUpdateTime)
    {
      nebRunningTime = 0.0;
      nebulaRectangle.x = nebFrame * nebulaRectangle.width;
      nebFrame++;
      if (nebFrame > 7)
      {
        nebFrame = 0;
      }
    }

    // draw nebula hazard
    DrawTextureRec(nebula, nebulaRectangle, nebulaPosition, WHITE);
    
    // draw scarfy
    DrawTextureRec(scarfy, scarfyRectangle, scarfyPosition, WHITE);

    EndDrawing();
  }
  UnloadTexture(scarfy);
  UnloadTexture(nebula);
  CloseWindow();
}