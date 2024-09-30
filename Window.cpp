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

    // Create a flock of boids
    std::vector<Boids> flock =
    {
        Boids(100, 100, 2, 2),
        Boids(200, 200, -2, -2),
        Boids(150, 150, 1.5, -1.5),
        Boids(300, 300, -1, 1),
        Boids(100, 100, 2, 2),
        Boids(200, 200, -2, -2),
        Boids(150, 150, 1.5, -1.5),
        Boids(300, 300, -1, 1),
        Boids(100, 100, 2, 2),
        Boids(200, 200, -2, -2),
        Boids(150, 150, 1.5, -1.5),
        Boids(300, 300, -1, 1),
        Boids(100, 100, 2, 2),
        Boids(200, 200, -2, -2),
        Boids(150, 150, 1.5, -1.5),
        Boids(300, 300, -1, 1),
        Boids(100, 100, 2, 2),
        Boids(200, 200, -2, -2),
        Boids(150, 150, 1.5, -1.5),
        Boids(300, 300, -1, 1),
        Boids(100, 100, 2, 2),
        Boids(200, 200, -2, -2),
        Boids(150, 150, 1.5, -1.5),
        Boids(300, 300, -1, 1),
        Boids(100, 100, 2, 2),
        Boids(200, 200, -2, -2),
        Boids(150, 150, 1.5, -1.5),
        Boids(300, 300, -1, 1),
        Boids(100, 100, 2, 2),
        Boids(200, 200, -2, -2),
        Boids(150, 150, 1.5, -1.5),
        Boids(300, 300, -1, 1),
        Boids(100, 100, 2, 2),
        Boids(200, 200, -2, -2),
        Boids(150, 150, 1.5, -1.5),
        Boids(300, 300, -1, 1),
        Boids(100, 100, 2, 2),
        Boids(200, 200, -2, -2),
        Boids(150, 150, 1.5, -1.5),
        Boids(300, 300, -1, 1),
        Boids(100, 100, 2, 2),
        Boids(200, 200, -2, -2),
        Boids(150, 150, 1.5, -1.5),
        Boids(300, 300, -1, 1),
        Boids(100, 100, 2, 2),
        Boids(200, 200, -2, -2),
        Boids(150, 150, 1.5, -1.5),
        Boids(300, 300, -1, 1),
        Boids(100, 100, 2, 2),
        Boids(200, 200, -2, -2),
        Boids(150, 150, 1.5, -1.5),
        Boids(300, 300, -1, 1),
        Boids(100, 100, 2, 2),
        Boids(200, 200, -2, -2),
        Boids(150, 150, 1.5, -1.5),
        Boids(300, 300, -1, 1),
        Boids(100, 100, 2, 2),
        Boids(200, 200, -2, -2),
        Boids(150, 150, 1.5, -1.5),
        Boids(300, 300, -1, 1),
        Boids(100, 100, 2, 2),
        Boids(200, 200, -2, -2),
        Boids(150, 150, 1.5, -1.5),
        Boids(300, 300, -1, 1),
        Boids(100, 100, 2, 2),
        Boids(200, 200, -2, -2),
        Boids(150, 150, 1.5, -1.5),
        Boids(300, 300, -1, 1),
        Boids(100, 100, 2, 2),
        Boids(200, 200, -2, -2),
        Boids(150, 150, 1.5, -1.5),
        Boids(300, 300, -1, 1),
        Boids(100, 100, 2, 2),
        Boids(200, 200, -2, -2),
        Boids(150, 150, 1.5, -1.5),
        Boids(300, 300, -1, 1),
        Boids(100, 100, 2, 2),
        Boids(200, 200, -2, -2),
        Boids(150, 150, 1.5, -1.5),
        Boids(300, 300, -1, 1),
        Boids(100, 100, 2, 2),
        Boids(200, 200, -2, -2),
        Boids(150, 150, 1.5, -1.5),
        Boids(300, 300, -1, 1),
        Boids(100, 100, 2, 2),
        Boids(200, 200, -2, -2),
        Boids(150, 150, 1.5, -1.5),
        Boids(300, 300, -1, 1),
        Boids(100, 100, 2, 2),
        Boids(200, 200, -2, -2),
        Boids(150, 150, 1.5, -1.5),
        Boids(300, 300, -1, 1),
        Boids(100, 100, 2, 2),
        Boids(200, 200, -2, -2),
        Boids(150, 150, 1.5, -1.5),
        Boids(300, 300, -1, 1)
    };

    // Create some obstacles
    std::vector<Obstacle> obstacles =
    {
        Obstacle(45, 200, {300, 450}),
        Obstacle(40, 40, {500, 200}),
        Obstacle(110, 15, {600, 600})
    };

    // Define the simulation parameters
    float minDistance = 30.0f;
    float alignmentFactor = 0.45f;
    float cohesionFactor = 0.45f;
    float maxSpeed = 7.4f;

    Vector2 boundsMin = {0, 0};      // Minimum boundary (top-left corner)
    Vector2 boundsMax = {screenWidth, screenHeight}; // Maximum boundary (bottom-right corner)

    SetTargetFPS(60); // Set FPS to 60
    
    while (!WindowShouldClose())
    {
        // Update the simulation
        flock[0].simulateStep(flock, obstacles, minDistance, alignmentFactor, cohesionFactor, maxSpeed, boundsMin, boundsMax);

        // Draw everything
        BeginDrawing();
        ClearBackground(RAYWHITE);

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
