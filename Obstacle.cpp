#include "Obstacle.h"

Obstacle::Obstacle(int _sizeX, int _sizeY, Vector2 _position)
{
    size_x = _sizeX;
    size_y = _sizeY;
    position = _position;
}

Obstacle::~Obstacle() = default;

void Obstacle::DrawObstacle() const
{
    DrawRectangle(static_cast<int>(position.x), static_cast<int>(position.y), size_x, size_y, BLACK);
}
