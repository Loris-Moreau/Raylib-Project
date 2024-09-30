using namespace std;

#include <iostream>
#include "raylib.h"
#include "Obstacle.h"

int main()
{
    cout << "Hello World" << '\n';

    InitWindow(960, 960, "Intermediate AI");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(DARKGRAY);
        Obstacle(1,1,Vector2{1,1});
        DrawObstacle();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
