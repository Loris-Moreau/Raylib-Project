using namespace std;

#include <iostream>
#include "raylib.h"

#include "Boids.h"
#include "Obstacle.h"

int main()
{
    // Initialize the window
    constexpr int screenWidth = 800;
    constexpr int screenHeight = 600;
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
        Boids(300, 300, -1, 1)
    };

    // Create some obstacles
    std::vector<Obstacle> obstacles =
    {
        Obstacle(50, 50, {400, 300}),
        Obstacle(80, 80, {600, 400}),
        Obstacle(40, 40, {250, 200})
    };

    // Define the simulation parameters
    float minDistance = 30.0f;
    float alignmentFactor = 0.05f;
    float cohesionFactor = 0.01f;
    float maxSpeed = 3.0f;

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
