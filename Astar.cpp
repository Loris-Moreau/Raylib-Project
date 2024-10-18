#include"Astar.h"

float Astar::distance(const Node& node1, const Node& node2)
{
    return (float)sqrt(pow(node1.x - node2.x, 2) + pow(node1.y - node2.y, 2));
}

float Astar::getTerrainCost(const Node& node)
{
    switch (node.terrain)
    {
        case Normal:
            return 1.0f;
        case Challenging:
            return 1.5f;
        case Difficult:
            return 2.0f;
        case terrainObstacle:
            return NULL;
        case BlueWork:
        case RedWork:
        case GreenWork:
        case BlueBase:
        case RedBase:
        case GreenBase:
            return 1.0f;
        case Road:
            return std::numeric_limits<float>::infinity();
    }
}

std::vector<Node*> Astar::astar(Node* start, const Node* goal, std::vector<std::vector<Node>>& grid)
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

void Astar::printGridWithPath(const std::vector<std::vector<Node>>& grid, const std::vector<Node*>& path)
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
            else if (node.obstacle || node.terrain == terrainObstacle)
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

void Astar::DrawPathWithGrid(std::vector<std::vector<Node>>& grid, const std::vector<Node*>& path)
{
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            const Node& node = grid[j][i];

            if (node.terrain == BlueWork) 
            {
                DrawRectangle(j * rectWidth, i * rectHeight, rectWidth - 1, rectHeight - 1, DARKBLUE);
            }
            else if (node.terrain == RedWork) 
            {
                DrawRectangle(j * rectWidth, i * rectHeight, rectWidth - 1, rectHeight - 1, RED);
            }
            else if (node.terrain == GreenWork) 
            {
                DrawRectangle(j * rectWidth, i * rectHeight, rectWidth - 1, rectHeight - 1, DARKGREEN);
            }
            else if (node.terrain == BlueBase) 
            {
                DrawRectangle(j * rectWidth, i * rectHeight, rectWidth - 1, rectHeight - 1, SKYBLUE);
            }
            else if (node.terrain == RedBase) 
            {
                DrawRectangle(j * rectWidth, i * rectHeight, rectWidth - 1, rectHeight - 1, MAGENTA);
            }
            else if (node.terrain == GreenBase) 
            {
                DrawRectangle(j * rectWidth, i * rectHeight, rectWidth - 1, rectHeight - 1, GREEN);
            }
            else if (std::find(path.begin(), path.end(), &node) != path.end())
            {
                DrawRectangle(j * rectWidth, i * rectHeight, rectWidth - 1, rectHeight - 1, DARKGRAY);
            }
            else if (node.terrain == Road)
            {
                DrawRectangle(j * rectWidth, i * rectHeight, rectWidth - 1, rectHeight - 1, BLACK);
            }
            else if (node.terrain == Normal)
            {
                DrawRectangle(j * rectWidth, i * rectHeight, rectWidth - 1, rectHeight - 1, LIGHTGRAY);
            }
            else if (node.terrain == Challenging)
            {
                DrawRectangle(j * rectWidth, i * rectHeight, rectWidth - 1, rectHeight - 1, ORANGE);
            }
            else if (node.terrain == Difficult)
            {
                DrawRectangle(j * rectWidth, i * rectHeight, rectWidth - 1, rectHeight - 1, DARKPURPLE);
            }
            else
            {
                DrawRectangle(j * rectWidth, i * rectHeight, rectWidth - 1, rectHeight - 1, WHITE);
            }
        }
    }
}
