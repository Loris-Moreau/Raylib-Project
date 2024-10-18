#pragma once

#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <cmath>
#include "Obstacle.h"

enum boidType { blue, red, green }; 

class Boids
{
public:
    // factors
    float separationFactor = 0.5f;
    float obstacleFactor = 1.0f;
    float repulsionStrength = 15.0f;

    int neighborLimit = 5;
    
    Vector2 position;
    Vector2 velocity;
    
    boidType type; 
    Color color;
    
    Boids(float x, float y, float vx, float vy, boidType t, Color c) : position({x, y}), velocity({vx, vy}), type(t), color(c) {}

    float radius = 3.5f; //size of boid
    
    void applyRules(std::vector<Boids>& flock,
                    const std::vector<Obstacle>& obstacles, 
                    float minDistance,
                    float alignmentFactor,
                    float cohesionFactor, 
                    float maxSpeed,
                    const Vector2& boundsMin,
                    const Vector2& boundsMax/*,
                    boidType predatorType,  // Pred for this boid group
                    float predatorAvoidFactor,
                    float preyAttractFactor*/
                    );
    
    void checkBoundaries(const Vector2& boundsMin, const Vector2& boundsMax);
    void updatePosition();
    void DrawBoid() const;
    
    static void simulateStep(std::vector<Boids>& flock,
                             const std::vector<Obstacle>& obstacles,
                             float minDistance,
                             float alignmentFactor,
                             float cohesionFactor,
                             float maxSpeed,
                             const Vector2& boundsMin,
                             const Vector2& boundsMax
    );
};
