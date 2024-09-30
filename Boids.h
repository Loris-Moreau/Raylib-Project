#pragma once

#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <cmath>
#include "Obstacle.h"

class Boids
{
public:
    // factors
    float separationFactor = 1.0f;
    
    Vector2 position;
    Vector2 velocity;

    Boids(float x, float y, float vx, float vy) : position({x, y}), velocity({vx, vy}) {}

    void applyRules(const std::vector<Boids>& flock,
                    const std::vector<Obstacle>& obstacles, 
                    float minDistance,
                    float alignmentFactor,
                    float cohesionFactor, 
                    float maxSpeed,
                    const Vector2& boundsMin,
                    const Vector2& boundsMax);
    
    void checkBoundaries(const Vector2& boundsMin, const Vector2& boundsMax);
    void updatePosition();
    void DrawBoid() const;
    
// Function to simulate a step in the simulation
void simulateStep(std::vector<Boids>& flock,
                  const std::vector<Obstacle>& obstacles,
                  float minDistance,
                  float alignmentFactor,
                  float cohesionFactor,
                  float maxSpeed,
                  const Vector2& boundsMin,
                  const Vector2& boundsMax);
};
