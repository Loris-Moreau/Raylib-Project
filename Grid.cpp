#include "Grid.h"

Grid::Grid(int _width, int _height, int _rows, int _cols)
    : width(_width), height(_height), rows_(_rows), cols_(_cols) {}

void Grid::spawnFromFile(const std::string& _filename, std::vector<std::vector<Node>>& grid)
{
    std::ifstream file(_filename);
    std::string line;
    
    int x, y;
    std::string type;
    
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            const Node& node = grid[j][i];
            if (node.obstacle || node.terrain == terrainObstacle || node.terrain == Normal)
            {
                type = "Obstacle";
                x = node.x;
                y = node.y;
            }
        }
    }
    
    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << _filename << '\n';
        return;
    }
    
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        iss >> type;
        
        /*if (grid.obstacle == true)
        {
            type = "Obstacle";
        }*/
        if (type == "Obstacle")
        {
            iss >> x >> y >> width >> height;
            obstacles.emplace_back(width, height, Vector2{(float)x, (float)y});
        }
        
        /*
        else if (type == "BOID")
        {
            float x, y, vx = 1, vy = 1;
            std::string colorType;
            iss >> x >> y >> vx >> vy >> colorType;

            boidType type = blue; // Default to blue
            Color color = BLUE;

            if (colorType == "red")
            {
                type = red;
                color = RED;
            }
            else if (colorType == "green")
            {
                type = green;
                color = GREEN;
            }

            // Groups Spawn
            constexpr int flockAmount = 100;

            // Create a single boid and assign velocity
            for (int i = 0; i < flockAmount; ++i)
            {
                boids.emplace_back(x + (float)i, y + (float)i, vx, vy, type, color);
            }
        }
        */
    }
    
    file.close();
}

// Draw func
void Grid::draw() const
{
    // obstacles
    for (const Obstacle& obstacle : obstacles)
    {
        obstacle.DrawObstacle();
    }

    /*
    // boids
    for (const Boids& boid : boids)
    {
        boid.DrawBoid();
    }
    */
}
