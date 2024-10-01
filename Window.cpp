using namespace std;

#include <iostream>
#include <raylib.h>

#include "Boids.h"
#include "Obstacle.h"

int main()
{
    // Initialize the window
    constexpr int screenWidth = 1080;
    constexpr int screenHeight = 960;
    InitWindow(screenWidth, screenHeight, "Intermediate AI");
    SetTargetFPS(60); // Set FPS to 60
    
    // Create a flock of boids using a loop to make Marie happy
    constexpr int flockAmount = 100;
    std::vector<Boids> flock;
    
    // Groups Spawn
    for (int i = 0; i < flockAmount; ++i)
    {
        flock.emplace_back(i, screenHeight/2+200, 2, 2, blue, BLUE);
        flock.emplace_back(screenWidth/2-150, i, 2, 2, red, RED);
        flock.emplace_back(screenWidth/2-150, screenHeight/2-200, 2, 2, green, GREEN);
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
    constexpr float maxSpeed = 7.42f;

    constexpr Vector2 boundsMin = {10, 10};      // Minimum boundary (top-left corner)
    constexpr Vector2 boundsMax = {screenWidth-10, screenHeight-10}; // Maximum boundary (bottom-right corner)
    
    
    while (!WindowShouldClose())
    {
        // Update the simulation
        flock[0].simulateStep(flock, obstacles, minDistance, alignmentFactor, cohesionFactor, maxSpeed, boundsMin, boundsMax);

        // Draw everything
        BeginDrawing();
        ClearBackground(LIGHTGRAY);
        
        // Draw boids
        for (const Boids& boid : flock)
        {
            boid.DrawBoid();
        }
        
        // Draw obstacles
        for (const Obstacle& obstacle : obstacles)
        {
            obstacle.DrawObstacle();
        }
        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
