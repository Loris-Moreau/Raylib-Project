using namespace std;

#include <iostream>
#include <raylib.h> 
#include "Grid.h"

int main()
{
    srand(time(nullptr));
    //R, G, B
    //R+G=Y, G+B=C, B+R=M
    constexpr Color choices[3] = {YELLOW, SKYBLUE, MAGENTA};
    const Color textColor = choices[rand()%3];
    
    // Init the window
    constexpr int screenWidth = 1080;
    constexpr int screenHeight = 960;
    InitWindow(screenWidth, screenHeight, "Intermediate AI");
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor())); // Set FPS to the refresh rate of the monitor

    constexpr int flockAmount = 2000/3; //as of 02/10/2024 : runs at 15~20 fps with 2000 boids
    std::vector<Boids> flock;
    
    // Groups Spawn
    for (int i = 0; i < flockAmount; ++i)
    {
        flock.emplace_back(i, screenHeight/2, 2, 2, blue, BLUE);
        flock.emplace_back(screenWidth/2-150, i, 2, 2, red, RED);
        flock.emplace_back(screenWidth/2+150, i, 2, 2, green, GREEN);
    }
    
    // Create a grid
    Grid grid(screenWidth, screenHeight, 25, 25);
    grid.spawnFromFile("spawn.txt");

    // Define the simulation parameters
    constexpr float minDistance = 25.0f;
    constexpr float alignmentFactor = 0.45f;
    constexpr float cohesionFactor = 0.45f;
    constexpr float maxSpeed = 7.5f;

    constexpr Vector2 boundsMin = {10, 10};  // (top-left corner)
    constexpr Vector2 boundsMax = {screenWidth-10, screenHeight-10};  // (bottom-right corner)
    
    while (!WindowShouldClose())
    {
        /*
        auto& gridBoids = grid.getBoids();
        for (Boids& boid : gridBoids)
        {
            boid.simulateStep(gridBoids, grid.getObstacles(), minDistance, alignmentFactor, cohesionFactor, maxSpeed, boundsMin, boundsMax);
        }
        */
        
        // Update the simulation
        flock[0].simulateStep(flock, grid.getObstacles(), minDistance, alignmentFactor, cohesionFactor, maxSpeed, boundsMin, boundsMax);
        
        BeginDrawing();
        ClearBackground(LIGHTGRAY);
        
        // Draw boids
        for (const Boids& boid : flock)
        {
            boid.DrawBoid();
        }
        
        grid.draw();

        // Runtime performance info
        DrawText(TextFormat("%02i FPS", GetFPS()), 15, 15, 20, textColor);
        DrawText(TextFormat("%03.03f ms", GetFrameTime()), 15, 45, 20, textColor);
        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
