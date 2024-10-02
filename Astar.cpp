/*
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <string>
#include "City.h"

// Define the dimensions of the grid (city map)
constexpr int rows = 10;
constexpr int cols = 10;

enum Terrain
{
    Normal,        // Roads or empty spaces
    Challenging,   // Difficult terrain (e.g., gravel, rough paths)
    Difficult,     // Very difficult terrain (e.g., steep hills)
    RoadObstacle       // Obstacle, impassable
};

// Node struct representing a point in the city map for pathfinding
struct Node {
    int x, y;                  // Position of the node
    float g, h, f;             // Cost values (g: cost from start, h: heuristic to goal, f: total cost)
    bool roadObstacle;             // True if the node is an obstacle (like a building)
    Node* parent;              // Pointer to the parent node for path reconstruction
    Terrain terrain;           // The type of terrain
    float terrainCostMultiplier; // Cost multiplier based on terrain type
    
    bool operator==(const Node& other) const {
        return x == other.x && y == other.y;
    }
};

// A utility function to calculate the Euclidean distance between two nodes
float gridDistance(const Node& node1, const Node& node2) {
    return (float)sqrt(pow(node1.x - node2.x, 2) + pow(node1.y - node2.y, 2));
}

// Function to convert a tile to terrain based on the TileType
Terrain tileToTerrain(TileType tile) {
    switch (tile) {
        case ROAD:
        case HOUSE:
        case WORKPLACE:
        case ROAD_NORTH_SOUTH:
        case ROAD_EAST_WEST:
            return Normal;       
        case EMPTY:
            return RoadObstacle; 
    }
}

// Function to get the terrain cost multiplier based on the terrain type
float getTerrainCost(const Node& node) {
    switch (node.terrain) {
        case Normal:
            return 1.0f;
        case Challenging:
            return 1.5f;
        case Difficult:
            return 2.0f;
        case RoadObstacle:
        return NULL;
    }
}

// A* Pathfinding algorithm implementation
std::vector<Node*> astar(Node* start, const Node* goal, std::vector<std::vector<Node>>& grid) {
    start->g = 0;
    start->h = gridDistance(*start, *goal);
    start->f = start->g + start->h;

    std::vector<Node*> openList, closedList;
    openList.push_back(start);

    while (!openList.empty()) {
        // Get the node with the lowest f value
        auto currentNode = std::min_element(openList.begin(), openList.end(), [](const Node* a, const Node* b) {
            return a->f < b->f;
        });
        Node* current = *currentNode;
        openList.erase(currentNode);

        // If we have reached the goal
        if (*current == *goal) {
            std::vector<Node*> path;
            while (current != nullptr) {
                path.push_back(current);
                current = current->parent;
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        closedList.push_back(current);

        // Generate children (neighbors)
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                if (dx == 0 && dy == 0) continue;  // Skip the current node itself

                const int newX = current->x + dx;
                const int newY = current->y + dy;

                if (newX < 0 || newX >= rows || newY < 0 || newY >= cols) continue;  // Out of grid bounds

                Node& child = grid[newX][newY];

                // Check if the child is an obstacle
                if (child.roadObstacle) continue;

                // Child is on the closed list
                if (std::find(closedList.begin(), closedList.end(), &child) != closedList.end()) continue;

                const float tentative_g = current->g + gridDistance(*current, child) * child.terrainCostMultiplier;

                bool isNewPath = false;

                // Check if the child is already in the open list
                auto it = std::find(openList.begin(), openList.end(), &child);
                if (it == openList.end()) {
                    isNewPath = true;
                    child.h = gridDistance(child, *goal);
                    openList.push_back(&child);
                } else if (tentative_g < child.g) {
                    isNewPath = true;
                }

                if (isNewPath) {
                    child.parent = current;
                    child.g = tentative_g;
                    child.f = child.g + child.h;
                }
            }
        }
    }

    // Return an empty path if no path is found
    return {};
}

// Function to create a grid of nodes from the city map
std::vector<std::vector<Node>> createNodeGrid(const std::vector<std::vector<TileType>>& cityMap) {
    std::vector<std::vector<Node>> grid(rows, std::vector<Node>(cols));
    
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            grid[x][y].x = x;
            grid[x][y].y = y;
            grid[x][y].terrain = tileToTerrain(cityMap[y][x]);
            grid[x][y].terrainCostMultiplier = getTerrainCost(grid[x][y]);
            grid[x][y].roadObstacle = (grid[x][y].terrain == RoadObstacle);
        }
    }
    
    return grid;
}

// Function to load the city from a text file
void loadCityFromFile(const std::string& fileName, std::vector<std::vector<TileType>>& cityMap) {
    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << fileName << '\n';
        return;
    }

    cityMap.resize(rows, std::vector<TileType>(cols, EMPTY));
    std::string line;
    int row = 0;

    while (std::getline(file, line) && row < rows) {
        for (int col = 0; col < cols && col < line.size(); ++col) {
            cityMap[row][col] = static_cast<TileType>(line[col]);
        }
        row++;
    }

    file.close();
}

// Function to move a worker (or car) from a start to a goal position
void moveWorker(Node* start, Node* goal, std::vector<std::vector<TileType>>& cityMap) {
    std::vector<std::vector<Node>> grid = createNodeGrid(cityMap);
    std::vector<Node*> path = astar(start, goal, grid);
    
    if (path.empty()) {
        std::cout << "No path found!\n";
    } else {
        // Move worker along the path
        for (auto node : path) {
            std::cout << "Moving to (" << node->x << ", " << node->y << ")\n";
        }
    }
}

// Print the grid with the path
void printGridWithPath(const std::vector<std::vector<Node>>& grid, const std::vector<Node*>& path) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            const Node& node = grid[i][j];
            if (std::find(path.begin(), path.end(), &node) != path.end()) {
                std::cout << " * "; // Mark path nodes with *
            } else if (node.roadObstacle) {
                std::cout << " X "; // Mark obstacle nodes with X
            } else {
                std::cout << " . "; // Normal nodes
            }
        }
        std::cout << '\n';
    }
}
*/