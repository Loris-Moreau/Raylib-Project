/*#pragma once

#include <string>
#include <vector>
#include "raylib.h"

// Enums to represent city tiles and terrain types
enum TileType
{
    EMPTY = '.',      // Empty space
    HOUSE = 'H',      // House (Spawn point)
    WORKPLACE = 'W',  // Workplace (Target)
    ROAD = 'R',       // Road (Path cars/workers can take)
    ROAD_NORTH_SOUTH = 'N',  // Directional Road (North-South)
    ROAD_EAST_WEST = 'E'     // Directional Road (East-West)
};

 struct Waypoint
 {
     Vector2 position;
 };

class City
{
public:
    // city layout data
    std::vector<std::vector<TileType>> cityMap;

    Vector2 gridPosition;
    
    float tileSize = 32.0f; // treat as int (only round numbers)
    
    void loadCityFromFile(const std::string& filename, std::vector<std::vector<TileType>>& _cityMap);
    void drawCity();

    //roads
    std::vector<Waypoint> waypoints;
    void generateWaypoints();
};
*/