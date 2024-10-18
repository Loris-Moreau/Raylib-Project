using namespace std;

#include <iostream>
#include <raylib.h> 

#include "Grid.h"

constexpr int screenWidth = 1080;
constexpr int screenHeight = 960;

const int padding = 1;

bool startSelected = false;
bool endSelected = false;

constexpr int flockAmount = 1000; //as of 02/10/2024 : runs at 15~20 fps with 2000 boids

Astar Ahat;

Node* start;
Node* goal;
std::vector<std::vector<Node>> grid(rows, std::vector<Node>(cols));

void initHat()
{
    //std::vector<std::vector<Node>> grid(rows, std::vector<Node>(cols));

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
    
    start = grid[1].data();
    goal = &grid[rows - 1][cols - 1];

    constexpr int minLoop = 1;
    constexpr int maxLoop = 17;
    constexpr int xSlideFactor = 1;
    for(int i = minLoop; i <= maxLoop; i++) // four walls
    {
        grid[minLoop + xSlideFactor][i].terrain = Road;
        grid[i + xSlideFactor][minLoop].terrain = Road;
        grid[maxLoop + xSlideFactor][i].terrain = Road;
        grid[i + xSlideFactor][maxLoop].terrain = Road;
        
        grid[i + xSlideFactor][maxLoop/2+minLoop].terrain = Road; // Center Cross
        grid[maxLoop/2+minLoop + xSlideFactor][i].terrain = Road;
    }

    //Destinations
    grid[maxLoop + 1 + xSlideFactor][maxLoop].terrain = BlueWork; 
    grid[maxLoop/2][maxLoop - 1].terrain = RedWork; 
    grid[maxLoop/2 + maxLoop/4 + xSlideFactor + 1][2].terrain = GreenWork;
    // Start
    grid[maxLoop/2 + xSlideFactor + 2][maxLoop/2 + 2].terrain = BlueBase; 
    grid[maxLoop/2 + xSlideFactor][4].terrain = RedBase; 
    grid[minLoop][maxLoop/2].terrain = GreenBase;
}

int main()
{
    bool randColor = true;
    Color textColor = SKYBLUE;
    if(randColor)
    {
        srand(time(nullptr));
        //R, G, B
        //R+G=Y, G+B=C, B+R=M
        constexpr Color choices[3] = {YELLOW, SKYBLUE, MAGENTA};
        textColor = choices[rand()%3];
    }
    
    //
    
    // Init the window
    InitWindow(screenWidth, screenHeight, "Intermediate AI");
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor())); // Set FPS to the refresh rate of the monitor
    
    std::vector<Boids> flock;

    //Astar
    initHat();
    
    // std::vector<Node*> astar(Node* start, const Node* goal, std::vector<std::vector<Node>>& grid)
    Ahat.astar(start, goal, grid);
    
    // Groups Spawn
    for (int i = 0; i < flockAmount/3; ++i)
    {
        flock.emplace_back(i, screenHeight/2, 2, 2, blue, BLUE);
        flock.emplace_back(screenWidth/2-150, i, 2, 2, red, RED);
        flock.emplace_back(screenWidth/2+150, i, 2, 2, green, GREEN);
    }
    
    // Create a grid
    Grid spawnGrid(screenWidth, screenHeight, 25, 25);
    spawnGrid.spawnFromFile("spawn.txt", grid);
    
    // Define the simulation parameters
    constexpr float minDistance = 25.0f;
    constexpr float alignmentFactor = 0.45f;
    constexpr float cohesionFactor = 0.45f;
    constexpr float maxSpeed = 7.5f;

    constexpr Vector2 boundsMin = {10, 10};  // (top-left corner)
    constexpr Vector2 boundsMax = {screenWidth-10, screenHeight-10};  // (bottom-right corner)

    /// Debug
    Ahat.printGridWithPath(grid, {});
    ///
    
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
