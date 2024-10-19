using namespace std;

#include <raylib.h> 

#include "Grid.h"

constexpr int screenWidth = 1080;
constexpr int screenHeight = 960;

bool startSelected = false;
bool endSelected = false;

constexpr int flockAmount = 10; //as of 02/10/2024 : runs at 15~20 fps with 2000 boids

Astar Ahat;

Node* blueStart;
Node* blueGoal;

Node* redStart;
Node* redGoal;

Node* greenStart;
Node* greenGoal;


std::vector<std::vector<Node>> grid(rows, std::vector<Node>(cols));

void initHat()
{
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            grid[i][j].x = i;
            grid[i][j].y = j;
            grid[i][j].parent = nullptr;

            if(grid[i][j].terrain == Normal)
            {
                grid[i][j].terrain = terrainObstacle;
                grid[i][j].obstacle = true;
            }
            else if (grid[i][j].obstacle)
            {
                grid[i][j].terrain = terrainObstacle;
            }
            grid[i][j].terrainCostMultiplier = Ahat.getTerrainCost(grid[i][j]);
        }
    }
    
    constexpr int minLoop = 1;
    constexpr int maxLoop = 17;
    constexpr int xSlideFactor = 1;
    for(int i = minLoop; i <= maxLoop; i++) 
    {
        // four walls
        grid[minLoop + xSlideFactor][i].terrain = Road;
        grid[i + xSlideFactor][minLoop].terrain = Road;
        grid[maxLoop + xSlideFactor][i].terrain = Road;
        grid[i + xSlideFactor][maxLoop].terrain = Road;
        // Center Cross
        grid[i + xSlideFactor][maxLoop/2+minLoop].terrain = Road; 
        grid[maxLoop/2+minLoop + xSlideFactor][i].terrain = Road;
    }

    // Destinations
    /// Blue
    grid[maxLoop + 1 + xSlideFactor][maxLoop].terrain = BlueWork;
    blueGoal = &grid[maxLoop + 1 + xSlideFactor][maxLoop];
    /// Red
    grid[maxLoop/2][maxLoop - 1].terrain = RedWork;
    redGoal = &grid[maxLoop/2][maxLoop - 1];
    /// Green
    grid[maxLoop/2 + maxLoop/4 + xSlideFactor + 1][2].terrain = GreenWork;
    greenGoal = &grid[maxLoop/2 + maxLoop/4 + xSlideFactor + 1][2];
    
    // Start
    ///Blue
    blueStart = &grid[maxLoop/2 + xSlideFactor + 2][maxLoop/2 + 2];
    grid[maxLoop/2 + xSlideFactor + 2][maxLoop/2 + 2].terrain = BlueBase; 
    ///Red
    grid[maxLoop/2 + xSlideFactor][4].terrain = RedBase; 
    redStart = &grid[maxLoop/2 + xSlideFactor][4];
    ///Green
    grid[minLoop][maxLoop/2].terrain = GreenBase;
    greenStart = &grid[minLoop][maxLoop/2];
}

int main()
{
    constexpr bool randColor = true;
    Color textColor = SKYBLUE;
    if(randColor)
    {
        srand((int)time(nullptr));
        //R, G, B
        //R+G=Y, G+B=C, B+R=M
        constexpr Color choices[3] = {YELLOW, SKYBLUE, MAGENTA};
        textColor = choices[rand()%3];
    }
    
    // Init the window
    InitWindow(screenWidth, screenHeight, "Intermediate AI");
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor())); // Set FPS to the refresh rate of the monitor
    const Image iconImage = LoadImage("Fox.png");
    SetWindowIcon(iconImage);
    
    std::vector<Boids> flock;

    //Astar
    initHat();
    
    // std::vector<Node*> astar(Node* start, const Node* goal, std::vector<std::vector<Node>>& grid)
    Ahat.astar(blueStart, blueGoal, grid);
    Ahat.astar(redStart, redGoal, grid);
    Ahat.astar(greenStart, greenGoal, grid);
    
    // Groups Spawn
    int b;
    for (int i = 0; i < flockAmount/3; ++i)
    {
        if(i >= screenWidth || i >= screenHeight || i < 20)
        {
            b = screenHeight/2;
        }
        else
        {
            b = i;
        }
        flock.emplace_back(b, screenHeight/2, 2, 2, blue, BLUE);
        flock.emplace_back(screenWidth/2-150, b, 2, 2, red, RED);
        flock.emplace_back(screenWidth/2+150, b, 2, 2, green, GREEN);
    }
    
    // Create a grid
    Grid spawnGrid(screenWidth, screenHeight, 20, 20);
    spawnGrid.spawnFromFile("spawn.txt", grid);
    
    // Define the simulation parameters
    constexpr float minDistance = 25.0f;
    constexpr float alignmentFactor = 0.45f;
    constexpr float cohesionFactor = 0.45f;
    constexpr float maxSpeed = 4.5f;

    constexpr Vector2 boundsMin = {10, 10};  // (top-left corner)
    constexpr Vector2 boundsMax = {screenWidth-10, screenHeight-10};  // (bottom-right corner)

    /// Debug
    Ahat.printGridWithPath(grid, Ahat.astar(blueStart, blueGoal, grid));
    //Ahat.printGridWithPath(grid, Ahat.astar(redStart, redGoal, grid));
    //Ahat.printGridWithPath(grid, Ahat.astar(greenStart, greenGoal, grid));
    /// haha
    
    while (!WindowShouldClose())
    {
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

    UnloadImage(iconImage);
    
    CloseWindow();
    return 0;
}
