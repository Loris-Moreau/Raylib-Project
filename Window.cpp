using namespace std;

#include <iostream>
#include "raylib.h"
#include "Obstacle.h"

int width = 1000;
int height = 1000;

int main()
{
    cout << "Hello World" << '\n';

    InitWindow(width, height, "Intermediate AI");
    SetTargetFPS(60);
    
    Obstacle o = Obstacle(50,200,Vector2{475,static_cast<float>(width)/2});
    Obstacle k = Obstacle(200,50,Vector2{200,250});
    
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(DARKGRAY);
        
        o.DrawObstacle();
        k.DrawObstacle();
        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
