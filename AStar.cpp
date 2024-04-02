#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include "raylib.h"

bool noUpdate = true;

int height = 960;
int width = height;

constexpr int rows = 10;
constexpr int cols = 10;
int cellSizeX = width / cols;
int cellSizeY = height / rows;

// Colors
Color emptyColor = {125,125,125,255}; 
Color pathColor = DARKGRAY; 
Color normalColor = GREEN; 
Color difficultColor = DARKGREEN; 
Color challengingColor = ORANGE; 
Color obstacleColor = SKYBLUE;

enum Terrain 
{
    Normal,
    Challenging,
    Difficult,
    Obstacle
};

struct Node
{
    int x, y; // Position of the node
    float g, h, f; // Cost values
    bool obstacle; // Flag to indicate if the node is an obstacle
    Node* parent; // Pointer to the parent node
    Terrain terrain; // Terrain type of the node
    float terrainCostMultiplier; // Cost multiplier based on terrain type
    bool operator == (const Node& other) const
    {
        return x == other.x && y == other.y;
    }
};

float distance(const Node& node1, const Node& node2)
{
    return sqrt(pow(node1.x - node2.x, 2) + pow(node1.y - node2.y, 2));
    // Narrowing conversion from 'double' to 'float' [bugprone-narrowing-conversion]
}

float getTerrainCost(const Node& node)
{
    switch (node.terrain)
    {
        case Normal:
            return 1.0f;
        case Challenging:
            return 1.5f;
        case Difficult:
            return 2.0f;
        case Obstacle:
            return std::numeric_limits<float>::infinity();
    }
    return 0;
}

std::vector<Node*> astar(Node* start, const Node* goal, std::vector<std::vector<Node>>& grid)
{
    start->terrain = Normal;
    start->terrainCostMultiplier = getTerrainCost(*start);
    
    std::vector<Node*> openList, closedList;
    start->g = 0;
    start->h = distance(*start, *goal);
    start->f = start->g + start->h;
    openList.push_back(start);
    
    while (!openList.empty())
    {
        // Get the node with the lowest f value
        auto currentNode = min_element(openList.begin(), openList.end(), [](const Node* a, const Node* b)
        {
            return a->f < b->f;
        });
        Node* current = *currentNode;
        openList.erase(currentNode);

        closedList.push_back(current);

        // Found the goal
        if (*current == *goal)
        {
            std::vector<Node*> path;
            while (current != nullptr)
            {
                path.push_back(current);
                current = current->parent;
            }
            reverse(path.begin(), path.end());
            return path;
        }

        // Generate children
        for (int dx = -1; dx <= 1; ++dx)
        {
            for (int dy = -1; dy <= 1; ++dy)
            {
                if (dx == 0 && dy == 0)
                {
                    continue; // Skip the current node
                }
                const int newX = current->x + dx;
                const int newY = current->y + dy;

                if (newX < 0 || newX >= rows || newY < 0 || newY >= cols)
                {
                    continue; // Out of grid bounds
                }

                Node& child = grid[newX][newY];

                // Check if the child is an obstacle
                if (child.obstacle)
                {
                    continue;
                }
                
                // Child is on the closedList
                if (find(closedList.begin(), closedList.end(), &child) != closedList.end())
                {
                    continue;
                }

                const float tentative_g = current->g + distance(*current, child) * child.terrainCostMultiplier;
                bool isNewPath = false;

                // Check if the child is already in the open list
                auto it = find(openList.begin(), openList.end(), &child);
                if (it == openList.end())
                {
                    isNewPath = true;
                    child.h = distance(child, *goal);
                    openList.push_back(&child);
                }
                else if (tentative_g < child.g)
                {
                    isNewPath = true;
                }
                if (isNewPath)
                {
                    child.parent = current;
                    child.g = tentative_g;
                    child.f = child.g + child.h;
                }
            }
        }
    }
    // If no path is found, return an empty path
    return {};
}

void printGridWithPath(const std::vector<std::vector<Node>>& grid, const std::vector<Node*>& path)
{
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            const Node& node = grid[j][i];
            if (find(path.begin(), path.end(), &node) != path.end())
            {
                //std::cout << " * "; // Mark path nodes with *
                DrawRectangle(j * cellSizeX, i * cellSizeY, cellSizeX-1, cellSizeY-1, pathColor);
            }
            else if (node.obstacle || node.terrain == Obstacle)
            {
                //std::cout << " X "; // Mark obstacle nodes with X
                DrawRectangle(j * cellSizeX, i * cellSizeY, cellSizeX-1, cellSizeY-1, obstacleColor);
            }
            else if(node.terrain == Normal)
            {
                //std::cout << " . "; // Mark Normal nodes with .
                DrawRectangle(j * cellSizeX, i * cellSizeY, cellSizeX-1, cellSizeY-1, normalColor);
            }
            else if(node.terrain == Challenging)
            {
                //std::cout << " C "; // Mark Challenging nodes with C
                DrawRectangle(j * cellSizeX, i * cellSizeY, cellSizeX-1, cellSizeY-1, challengingColor);
            }
            else if(node.terrain == Difficult)
            {
                //std::cout << " D "; // Mark Difficult nodes with D
                DrawRectangle(j * cellSizeX, i * cellSizeY, cellSizeX-1, cellSizeY-1, difficultColor);
            }
            else
            {
                //std::cout << "E "; // Mark empty nodes with E
                DrawRectangle(j * cellSizeX, i * cellSizeY, cellSizeX-1, cellSizeY-1, emptyColor);
            }
        }
        //std::cout << '\n';
    }
}

void ResetNodes(std::vector<std::vector<Node>>& grid)
{
    for(auto& row : grid)
    {
        for(auto& node : row)
        {
            node.g = 0;
            node.f = 0;
            node.parent = nullptr;
        }
    }
}

void InitHeuristics(std::vector<std::vector<Node>>& grid, Vector2 goal)
{
    for (int x = 0; x < rows; ++x)
    {
        for (int y = 0; y < cols; ++y)
        {
            const float dx = static_cast<float>(x) - goal.x;
            const float dy = static_cast<float>(y) - goal.y;
            grid[x][y].h = sqrt(dx * dx + dy * dy);
        }
    }
}

bool startSelected = false;
bool endSelected = false;

void initWindow()
{
    InitWindow(width, height, "PathFinding AStar");
    SetWindowMonitor(GetMonitorCount()-1);
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
}

int main()
{
    initWindow();
    
    std::vector<std::vector<Node>> grid(rows, std::vector<Node>(cols));
   
    // Initialize grid with nodes and set obstacles
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            grid[i][j].x = i;
            grid[i][j].y = j;
            grid[i][j].parent = nullptr;
            
            // Assign terrain types
            if (grid[i][j].obstacle)
            {
                grid[i][j].terrain = Obstacle;
            }
            else
            {
                grid[i][j].terrain = Normal;
            }
            grid[i][j].terrainCostMultiplier = getTerrainCost(grid[i][j]);
        }
    }
    
    Node* start = grid[0].data(); // Set start node
    const Node* goal = &grid[rows - 1][cols - 1];  // Set goal node
    
    // Set Terrain Nodes
    grid[1][6].obstacle = true;
    grid[1][7].obstacle = true;
    grid[1][8].obstacle = true;
    
    grid[2][0].obstacle = true;
    grid[2][1].obstacle = true;
    grid[2][2].obstacle = true;
    grid[2][3].obstacle = true;
    grid[2][4].obstacle = true;
    grid[2][5].obstacle = true;
    
    grid[4][5].obstacle = true;
    grid[4][6].obstacle = true;
    grid[4][7].obstacle = true;
    grid[4][8].obstacle = true;
    grid[4][9].obstacle = true;
    
    grid[6][6].obstacle = true;
    grid[6][7].obstacle = true;
    grid[6][8].obstacle = true;
    grid[6][9].obstacle = true;
    
    grid[8][5].obstacle = true;
    grid[8][6].obstacle = true;
    grid[8][7].obstacle = true;
    grid[8][8].obstacle = true;
    
    grid[5][5].terrain = Challenging;
    
    grid[6][0].terrain = Challenging;
    grid[6][1].terrain = Challenging;
    grid[6][2].terrain = Challenging;
    grid[6][3].terrain = Challenging;

    grid[5][3].terrain = Difficult;
    
    grid[8][2].terrain = Difficult;
    grid[8][3].terrain = Difficult;
    grid[8][4].terrain = Difficult;
    grid[8][9].terrain = Difficult;

    // RayLib Window Loop
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(emptyColor);
        
        // Reset start and end selection flags
        if (IsKeyPressed(KEY_R))
        {
            startSelected = false;
            endSelected = false;
            ResetNodes(grid);
        }
        if (!start) startSelected = false;
        if (!goal) endSelected = false;
        
        // Handle user input to select start and end nodes
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            const Vector2 mousePos = GetMousePosition();
            const int x = static_cast<int>(mousePos.x) / cellSizeX;
            const int y = static_cast<int>(mousePos.y) / cellSizeY;
            if (x >= 0 && x < rows && y >= 0 && y < cols)
            {
                if (!startSelected)
                {
                    start = &grid[x][y];
                    startSelected = true;
                    printf("start ");
                }
                else if (!endSelected && &grid[x][y] != start)
                {
                    goal = &grid[x][y];
                    endSelected = true;
                    printf(" end \n");
                }
            }
            noUpdate = false;
        }
        // Handle user input to toggle obstacles
        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
        {
            const Vector2 mousePos = GetMousePosition();
            const int x = static_cast<int>(mousePos.x) / cellSizeX;
            const int y = static_cast<int>(mousePos.y) / cellSizeY;
            if (x >= 0 && x < rows && y >= 0 && y < cols)
            {
                grid[x][y].obstacle = !grid[x][y].obstacle;
                grid[x][y].terrain = grid[x][y].obstacle ? Obstacle : Normal;
                grid[x][y].terrainCostMultiplier = getTerrainCost(grid[x][y]);
            }
            noUpdate = false;
        }

        // Print Grid with no path to at least have something show up 
        printGridWithPath(grid, {});

        // make sure a valid Start, End node are selected
        if (startSelected && endSelected && !start->obstacle && !goal->obstacle)
        {
            // Call A* algorithm
            const std::vector<Node*> path = astar(start, goal, grid);

            // Print the path
            /*
            if(!noUpdate)
            {
                if (!path.empty())
                {
                    std::cout << "Path found :\n";
                    for (const auto& node : path)
                    {
                        std::cout << getTerrainCost(*node) << " -> ";
                        std::cout << "(" << node->x << ", " << node->y << ") | ";
                    }
                    std::cout << '\n';

                    // Print the grid with the path marked
                    std::cout << "Grid with path :\n";
                }
                else
                {
                    std::cout << "Insanity is doing the same thing over and over again and expecting different results. \n";
                }
                
            }
            */

            // print the grid with the Full Computed Path (if valid, else returns an empty path)
            printGridWithPath(grid, path);
            if (start != nullptr) DrawRectangle(start->x * cellSizeX, start->y * cellSizeY, cellSizeX - 1, cellSizeY - 1, YELLOW);
            if (goal != nullptr) DrawRectangle(goal->x * cellSizeX, goal->y * cellSizeY, cellSizeX - 1, cellSizeY - 1, RED);
            noUpdate = true;
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
