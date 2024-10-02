/*#include "City.h"

#include <fstream>

void City::loadCityFromFile(const std::string& filename, std::vector<std::vector<TileType>>& _cityMap)
{
    cityMap = _cityMap;
    std::ifstream file(filename);
    std::string line;
    
    while (std::getline(file, line))
    {
        std::vector<TileType> row;
        for (char ch : line)
        {
            row.push_back((TileType)ch);  // Convert char to TileType
        }
        _cityMap.push_back(row);
    }
}

void City::drawCity()
{
    for (int y = 0; y < cityMap.size(); y++)
        {
        for (int x = 0; x < cityMap[y].size(); x++)
            {
            gridPosition = { (float)x * tileSize, (float)y * tileSize };

            switch (cityMap[y][x])
            {
            case HOUSE:
                DrawRectangleV(gridPosition, { tileSize, tileSize }, BLUE);
                break;
            case WORKPLACE:
                DrawRectangleV(gridPosition, { tileSize, tileSize }, RED);
                break;
            case ROAD:
                DrawRectangleV(gridPosition, { tileSize, tileSize }, DARKGRAY);
                break;
            case EMPTY:
            default:
                DrawRectangleV(gridPosition, { tileSize, tileSize }, LIGHTGRAY);
                break;
            }
        }
    }
}

void City::generateWaypoints()
{
    for (int y = 0; y < cityMap.size(); y++)
    {
        for (int x = 0; x < cityMap[y].size(); x++)
        {
            if (cityMap[y][x] == ROAD)
            {
                waypoints.push_back({ Vector2{ (float)x * 32 + 16, (float)y * 32 + 16 } });  // Center of road tile
            }
        }
    }
}
*/