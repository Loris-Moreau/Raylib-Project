#include <complex.h>
using namespace std;

#include <iostream>
#include "raylib.h"

#include "Boids.h"
#include "Obstacle.h"

int main()
{
    // Initialize the window
    constexpr int screenWidth = 1080;
    constexpr int screenHeight = 960;
    InitWindow(screenWidth, screenHeight, "Intermediate AI");
    
    // Create a flock of boids using a loop
    int flockAmount = 1000;
    std::vector<Boids> flock;

    for (int i = 0; i < flockAmount; ++i)
    {
        float x = 1 + i ; // Starting x position
        float y = 100 ; // Alternate y positions for diversity

        // Create and add a new boid to the flock
        flock.emplace_back(x, y, 5, 5);
    }
    
    // Create some obstacles
    std::vector<Obstacle> obstacles =
    {
        Obstacle(60, 250, {125, 500}),
        Obstacle(65, 65, {500, 75}),
        Obstacle(110, 150, {850, 750}),
        Obstacle(75, 75, {500, 500})
    };

    // Define the simulation parameters
    float minDistance = 25.0f;
    float alignmentFactor = 0.45f;
    float cohesionFactor = 0.45f;
    float maxSpeed = 7.4f;

    Vector2 boundsMin = {10, 10};      // Minimum boundary (top-left corner)
    Vector2 boundsMax = {screenWidth-10, screenHeight-10}; // Maximum boundary (bottom-right corner)

    SetTargetFPS(60); // Set FPS to 60
    
    while (!WindowShouldClose())
    {
        // Update the simulation
        flock[0].simulateStep(flock, obstacles, minDistance, alignmentFactor, cohesionFactor, maxSpeed, boundsMin, boundsMax);

        // Draw everything
        BeginDrawing();
        ClearBackground(LIGHTGRAY);

        // Draw boids
        for (const auto& boid : flock)
        {
            boid.DrawBoid();
        }

        // Draw obstacles
        for (const auto& obstacle : obstacles)
        {
            obstacle.DrawObstacle();
        }

        EndDrawing();
    }

    CloseWindow(); // Close window and OpenGL context
    return 0;
}
