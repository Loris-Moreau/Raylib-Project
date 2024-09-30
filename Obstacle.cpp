#include "Obstacle.h"

Obstacle::Obstacle(int sizex, int sizey, Vector2 _position)
{
    size_x = sizex;
    size_y = sizey;
    position = _position;
}

Obstacle::~Obstacle() = default;

void Obstacle::DrawObstacle()
{
    DrawRectangle(static_cast<int>(position.x), static_cast<int>(position.y), size_x, size_y, BLACK);
}
