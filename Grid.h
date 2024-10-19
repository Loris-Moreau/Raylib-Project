#pragma once

#include <fstream>
#include <iostream>
#include <raylib.h>
#include <sstream>
#include <vector>

#include "Astar.h"
#include "Boids.h"

class Grid
{
public:
    Grid(int _width, int _height, int _rows, int _cols);
    void spawnFromFile(const std::string& _filename, std::vector<std::vector<Node>>& grid);
    void draw() const;
    
    //std::vector<Boids>& getBoids() {return boids;}
    const std::vector<Obstacle>& getObstacles() const { return obstacles; }

private:
    int width;
    int height;
    int rows_;
    int cols_;
    
    //std::vector<Boids> boids;
    std::vector<Obstacle> obstacles;

    Astar astarion;
};
