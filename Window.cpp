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
    SetTargetFPS(60); // Set FPS to 60
    
    // Create a flock of boids using a loop
    constexpr int flockAmount = 1000;
    std::vector<Boids> flock;

    for (int i = 0; i < flockAmount; ++i)
    {
        float x = screenWidth / 2.0f;
        if (i < screenWidth-10)
        {
            x = static_cast<float>(i); // Starting x position
        }
        
        float y = screenHeight / 2.0f; // starting y position

        // Create and add a new boid to the flock
        flock.emplace_back(x, y, 5, 5);
    }
    
    // Create some obstacles
    const std::vector<Obstacle> obstacles =
    {
        Obstacle(60, 250, {125, 500}),
        Obstacle(65, 65, {500, 75}),
        Obstacle(110, 150, {850, 750}),
        Obstacle(75, 75, {500, 500})
    };

    // Define the simulation parameters
    constexpr float minDistance = 25.0f;
    constexpr float alignmentFactor = 0.45f;
    constexpr float cohesionFactor = 0.45f;
    constexpr float maxSpeed = 7.4f;

    constexpr Vector2 boundsMin = {10, 10};      // Minimum boundary (top-left corner)
    constexpr Vector2 boundsMax = {screenWidth-10, screenHeight-10}; // Maximum boundary (bottom-right corner)

    
    while (!WindowShouldClose())
    {
        // Update the simulation
        flock[0].simulateStep(flock, obstacles, minDistance, alignmentFactor, cohesionFactor, maxSpeed, boundsMin, boundsMax);

        // Draw everything
        BeginDrawing();
        ClearBackground(LIGHTGRAY);

        // Draw obstacles
        for (const Obstacle& obstacle : obstacles)
        {
            obstacle.DrawObstacle();
        }
        
        // Draw boids
        for (const Boids& boid : flock)
        {
            boid.DrawBoid();
        }
        
        EndDrawing();
    }

    CloseWindow(); // Close window and OpenGL context
    return 0;
}
