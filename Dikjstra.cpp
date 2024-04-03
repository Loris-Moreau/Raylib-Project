#include <iostream>
using namespace std;

#include <climits>
#include "raylib.h"
//int inf = std::numeric_limits<int>::max();

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

enum
{
	start = 7
};

/*
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
*/

int minDistance(int dist[], bool sptSet[])
{
	int min = INT_MAX;
	int min_index = 0;

	for (int v = 0; v < start; v++)
	{
		if (sptSet[v] == false && dist[v] <= min)
		{
			min = dist[v], min_index = v;
		}
	}
	return min_index;
}

void printSolution(int dist[])
{
	cout << "Vertex \t\t Distance from Source" << '\n';
	for (int i = 0; i < start; i++)
	{
		cout << i << " \t\t" << dist[i] << "\t" <<'\n';
	}
}

void dijkstra(int graph[start][start], int src)
{
	int dist[start];
	
	bool sptSet[start];
	
	for (int i = 0; i < start; i++)
	{
		dist[i] = INT_MAX, sptSet[i] = false;
	}

	dist[src] = 0;

	for (int count = 0; count < start - 1; count++)
	{
		int u = minDistance(dist, sptSet);

		sptSet[u] = true;

		for (int v = 0; v < start; v++)
		{
			if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX && dist[u] + graph[u][v] < dist[v])
			{
				dist[v] = dist[u] + graph[u][v];
			}
		}
	}
	printSolution(dist);
}


void initWindow()
{
	InitWindow(width, height, "PathFinding AStar");
	SetWindowMonitor(GetMonitorCount()-1);
	SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
}

int main()
{
	initWindow();
	
	int graph[start][start] =
	{
		0, 10, 15, NULL, 30, NULL, NULL,
		NULL, 0, NULL, NULL, NULL, 57, NULL,
		15, NULL, 0, 16, NULL, NULL, 52,
		NULL, NULL, 13, 0, NULL, NULL, NULL,
		30, NULL, NULL, NULL, 0, 11, 34,
		NULL, 49, NULL, NULL, 12, 0, NULL,
		NULL, NULL, 63, NULL, 35, NULL, 0
	};
	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(emptyColor);
		
		dijkstra(graph, 0);
		
		// if (start != nullptr) DrawRectangle(start->x * cellSizeX, start->y * cellSizeY, cellSizeX - 1, cellSizeY - 1, YELLOW);
		// if (goal != nullptr) DrawRectangle(goal->x * cellSizeX, goal->y * cellSizeY, cellSizeX - 1, cellSizeY - 1, RED);
		EndDrawing();
	}
	CloseWindow();
	return 0;
}
