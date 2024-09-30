#pragma once

#include <raylib.h>

class Obstacle
{
public:
    Obstacle(int sizex, int sizey, Vector2 position);
    ~Obstacle();

    void DrawObstacle() const;

    Vector2 position = Vector2{0, 0};

    int size_x = 5;
    int size_y = 5;
};
