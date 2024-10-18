#pragma once 

#include <algorithm>
#include <cmath>
#include <iostream>
#include <raylib.h>
#include <vector>

constexpr int rows = 20;
constexpr int cols = 20;

constexpr int rectWidth = 50;
constexpr int rectHeight = 50;

enum Terrain
{
    Normal,
    Challenging,
    Difficult,
    terrainObstacle,
    BlueWork,
    RedWork,
    GreenWork,
    BlueBase,
    RedBase,
    GreenBase,
    Road
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

class Astar
{
public:
    std::vector<Node*> astar(Node* start, const Node* goal, std::vector<std::vector<Node>>& grid);
    
    void DrawPathWithGrid(std::vector<std::vector<Node>>& grid, const std::vector<Node*>& path);
    void printGridWithPath(const std::vector<std::vector<Node>>& grid, const std::vector<Node*>& path);

    float distance(const Node& node1, const Node& node2);
    float getTerrainCost(const Node& node);
    
private: 
    int rows = 20;
    int cols = 20;
    int rectWidth = 50;
    int rectHeight = 50;
};
