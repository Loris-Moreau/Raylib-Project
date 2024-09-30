#pragma once
#include <array>
#include <raylib.h>

class Obstacle
{
public:
    Obstacle(int sizex, int sizey, Vector2 position);
    ~Obstacle();
    
    void DrawObstacle();
    
    //obstacles[Obstacle];

private:
    int size_x = 5;
    int size_y = 5;
    Vector2 position = Vector2{0, 0};
};
