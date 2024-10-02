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
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor())); // Set FPS to 60
    
    // Create a flock of boids using a loop
    constexpr int flockAmount = 500 ;
    std::vector<Boids> flock;
    
    // Groups Spawn
    for (int i = 0; i < flockAmount; ++i)
    {
        flock.emplace_back(i, screenHeight/2+150, 2, 2, blue, BLUE);
        flock.emplace_back(screenWidth/2-150, i, 2, 2, red, RED);
        flock.emplace_back(screenWidth/2+150, i, 2, 2, green, GREEN);
    }
    
    // Create some obstacles
    const std::vector<Obstacle> obstacles =
    {
        Obstacle(screenWidth/18, screenHeight/3, {screenWidth/9.0f, screenHeight/2.5f}),
        Obstacle(screenWidth/18, screenHeight/3, {screenWidth/1.25f, screenHeight/4.0f}),
        
        Obstacle(screenWidth/4, screenHeight/15, {screenWidth/2.7f, screenHeight/6.0f}),
        Obstacle(screenWidth/9, screenHeight/8, {screenWidth/2.5f, screenHeight/1.35f})
    };
    
    // Define the simulation parameters
    constexpr float minDistance = 25.0f;
    constexpr float alignmentFactor = 0.45f;
    constexpr float cohesionFactor = 0.45f;
    constexpr float maxSpeed = 10.0f;

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

        std::cout << GetFPS() << '\n';
        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
