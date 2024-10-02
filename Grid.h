#pragma once

#include <raylib.h>

class Grid
{
public:
    Grid(Vector2 _size);
    ~Grid();
    
    Vector2 gridSize = {10.0f, 10.0f};
    
    // tile size & scale
private:
     int scaling = 3;
     int tileSize = 10;
public:
     int scale = scaling * tileSize;

    
    void debugDraw() const;
};
