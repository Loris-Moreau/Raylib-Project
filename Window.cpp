using namespace std;

#include <iostream>
#include <raylib.h> 
#include "Grid.h"

int main()
{
    // Init the window
    constexpr int screenWidth = 1080;
    constexpr int screenHeight = 960;
    InitWindow(screenWidth, screenHeight, "Intermediate AI");
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor())); // Set FPS to the refresh rate of the monitor
    
    //int frameSkip = 2; // Update every "frameSkip" frames
    //int frameCount = 0;
    
    // Create a grid
    Grid grid(screenWidth, screenHeight, 25, 25);
    grid.spawnFromFile("spawn.txt");

    // Define the simulation parameters
    constexpr float minDistance = 25.0f;
    constexpr float alignmentFactor = 0.45f;
    constexpr float cohesionFactor = 0.45f;
    constexpr float maxSpeed = 2.0f;

    constexpr Vector2 boundsMin = {10, 10};  // (top-left corner)
    constexpr Vector2 boundsMax = {screenWidth-10, screenHeight-10};  // (bottom-right corner)
    
    
    while (!WindowShouldClose())
    {
        //if(frameCount % frameSkip == 0)
        //{
            auto& gridBoids = grid.getBoids();
            for (Boids& boid : gridBoids)
            {
                boid.simulateStep(gridBoids, grid.getObstacles(), minDistance, alignmentFactor, cohesionFactor, maxSpeed, boundsMin, boundsMax);
            }
        //}
        
        //frameCount++;
        
        BeginDrawing();
        ClearBackground(LIGHTGRAY);
        
        grid.draw();
        
        std::cout << "FPS : " << GetFPS() << '\n';
        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
