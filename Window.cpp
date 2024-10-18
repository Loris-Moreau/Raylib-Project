using namespace std;

#include <iostream>
#include <raylib.h> 

#include "Astar.h"
#include "Grid.h"

constexpr int screenWidth = 1080;
constexpr int screenHeight = 960;

const int padding = 1;

bool startSelected = false;
bool endSelected = false;

constexpr int flockAmount = 5; //as of 02/10/2024 : runs at 15~20 fps with 2000 boids

int main()
{
    Astar Ahat;
    
    srand(time(nullptr));
    //R, G, B
    //R+G=Y, G+B=C, B+R=M
    //constexpr Color choices[3] = {YELLOW, SKYBLUE, MAGENTA};
    //const Color textColor = choices[rand()%3];
    constexpr Color textColor = YELLOW;
    
    // Init the window
    InitWindow(screenWidth, screenHeight, "Intermediate AI");
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor())); // Set FPS to the refresh rate of the monitor
    
    std::vector<Boids> flock;

    //Astar
    std::vector<std::vector<Node>> grid(rows, std::vector<Node>(cols));

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            grid[i][j].x = i;
            grid[i][j].y = j;
            grid[i][j].parent = nullptr;

            if (grid[i][j].obstacle)
            {
                grid[i][j].terrain = Road;
            }
            else
            {
                grid[i][j].terrain = Normal;
            }
            grid[i][j].terrainCostMultiplier = Ahat.getTerrainCost(grid[i][j]);
        }
    }

    Node* start = grid[0].data();
    Node* goal = &grid[rows - 1][cols - 1];
    
    // std::vector<Node*> astar(Node* start, const Node* goal, std::vector<std::vector<Node>>& grid)
    Ahat.astar(start, goal, grid);
    
    // Groups Spawn
    for (int i = 0; i < flockAmount; ++i)
    {
        flock.emplace_back(i, screenHeight/2, 2, 2, blue, BLUE);
        flock.emplace_back(screenWidth/2-150, i, 2, 2, red, RED);
        flock.emplace_back(screenWidth/2+150, i, 2, 2, green, GREEN);
    }
    
    // Create a grid
    Grid spawnGrid(screenWidth, screenHeight, 25, 25);
    spawnGrid.spawnFromFile("spawn.txt");
    
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
        flock[0].simulateStep(flock, spawnGrid.getObstacles(), minDistance, alignmentFactor, cohesionFactor, maxSpeed, boundsMin, boundsMax);
        
        BeginDrawing();
        ClearBackground(LIGHTGRAY);

        //Astar drawing
        Ahat.DrawPathWithGrid(grid, {});
        
        // Draw boids
        for (const Boids& boid : flock)
        {
            boid.DrawBoid();
        }
        
        spawnGrid.draw();

        // Runtime performance info
        DrawText(TextFormat("%02i FPS", GetFPS()), 15, 15, 20, textColor);
        DrawText(TextFormat("%03.03f ms", GetFrameTime()), 15, 45, 20, textColor);
        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
