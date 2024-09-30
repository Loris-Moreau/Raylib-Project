using namespace std;

#include <iostream>
#include "raylib.h"

int main()
{
    cout << "Hello World" << '\n';

    InitWindow(300, 300, "Intermediate AI");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(DARKGRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
