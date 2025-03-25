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

  // rectangle dimensions
  const int recWidth{50};
  const int recHeight{80};
  
  // check if object is in the air
  bool isInAir{};
  // jump velocity
  const int jumpVel{-22};

  int posY{windowHeight - recHeight};
  int velocity{0}; // measured in pixels per frame ( distance / time ) ex. 20 pixels in 2 frames = 10 pixels per frame


  SetTargetFPS(60);

  while (!WindowShouldClose()) // while loop is processed every frame
  {
    BeginDrawing();

    ClearBackground(WHITE);

    // perform ground check
    if (posY >= windowHeight - recHeight)
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
    posY += velocity;

    DrawRectangle(windowWidth / 2, posY, recWidth, recHeight, BLUE);

    EndDrawing();
  }
  CloseWindow();
}