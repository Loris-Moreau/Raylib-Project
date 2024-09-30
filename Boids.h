#pragma once
#include <raylib.h>
#include "Obstacle.h"

class Boids
{
public:
    float minDistance;
    float maxPeceiveDistance;
    float cohesionRadius;
    float maxSteer;

private:
    //Boids others;
    //Vector2 separate (Boids[] others);
    //Vector2 avoid_obstacles(Obstacle.obstacles[]);  //Obstacles hold a position and size, only rectangles for now
    //Vector2 align(Boids[] others);
    //Vector2 group(Boids[] others);

};
