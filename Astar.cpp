#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

constexpr int rows = 10;
constexpr int cols = 10;

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
    bool operator==(const Node& other) const
    {
        return x == other.x && y == other.y;
    }
};

float distance(const Node& node1, const Node& node2)
{
    return (float)sqrt(pow(node1.x - node2.x, 2) + pow(node1.y - node2.y, 2));
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
            return NULL;
    }
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
        auto currentNode = std::min_element(openList.begin(), openList.end(), [](const Node* a, const Node* b)
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
            std::reverse(path.begin(), path.end());
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
                if (std::find(closedList.begin(), closedList.end(), &child) != closedList.end())
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
            const Node& node = grid[i][j];
            if (std::find(path.begin(), path.end(), &node) != path.end())
            {
                std::cout << " * "; // Mark path nodes with *
            }
            else if (node.obstacle || node.terrain == Obstacle)
            {
                std::cout << " X "; // Mark obstacle nodes with X
            }
            else if(node.terrain == Normal)
            {
                std::cout << " . "; // Mark Normal nodes with .
            }
            else if(node.terrain == Challenging)
            {
                std::cout << " C "; // Mark Challenging nodes with C
            }
            else if(node.terrain == Difficult)
            {
                std::cout << " D "; // Mark Difficult nodes with D
            }
            else
            {
                std::cout << "E "; // Mark empty nodes with E
            }
        }
        std::cout << '\n';
    }
}

/*
int main()
{
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
    
    // Set Nodes
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
    
    // Call A* algorithm
    const std::vector<Node*> path = astar(start, goal, grid);

    // Print the path
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
        printGridWithPath(grid, path);
    }
    else
    {
        std::cout << "Insanity is doing the same thing over and over again and expecting different results. \n";
        printGridWithPath(grid, path);
    }
    return 0;
}
*/
