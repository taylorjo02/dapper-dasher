#include <raylib.h>

int main()
{
  // window dimensions
  const int windowWidth{512};
  const int windowHeight{380};

  InitWindow(windowWidth, windowHeight, "Dapper Dasher");

  // acceleration due to gravity value (pixels/frame) per frame
  // every frame, velocity will change this amount per frame
  const int gravity{1};

  Texture2D scarfy = LoadTexture("textures/scarfy.png");

  Rectangle scarfyRectangle;
  scarfyRectangle.width = scarfy.width/6;
  scarfyRectangle.height = scarfy.height;
  scarfyRectangle.x = 0;
  scarfyRectangle.y = 0;

  Vector2 scarfyPosition;

  scarfyPosition.x = windowWidth/2 - scarfyRectangle.width/2;
  scarfyPosition.y = windowHeight - scarfyRectangle.height;

  
  // check if object is in the air
  bool isInAir{};
  // jump velocity
  const int jumpVel{-22};

  int velocity{0}; // measured in pixels per frame ( distance / time ) ex. 20 pixels in 2 frames = 10 pixels per frame


  SetTargetFPS(60);

  while (!WindowShouldClose()) // while loop is processed every frame
  {
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
      velocity += gravity;
    }

    // jump check
    if (!isInAir && IsKeyPressed(KEY_SPACE))
    {
      // negative value to make object move upward upon space key being pressed
      velocity += jumpVel;
    }

    // update position
    scarfyPosition.y += velocity;

    DrawTextureRec(scarfy, scarfyRectangle, scarfyPosition, WHITE);

    EndDrawing();
  }
  UnloadTexture(scarfy);
  CloseWindow();
}