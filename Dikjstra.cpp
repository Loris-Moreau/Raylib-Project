#include <iostream>
using namespace std;

#include <climits>

//int inf = std::numeric_limits<int>::max();

enum
{
    V = 7
};

int minDistance(int dist[], bool sptSet[])
{
    int min = INT_MAX;
    int min_index = 0;

    for (int v = 0; v < V; v++)
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
    for (int i = 0; i < V; i++)
    {
        cout << i << " \t\t" << dist[i] << "\t" <<'\n';
    }
}

void dijkstra(int graph[V][V], int src)
{
    int dist[V];
	
    bool sptSet[V];
	
    for (int i = 0; i < V; i++)
    {
        dist[i] = INT_MAX, sptSet[i] = false;
    }

    dist[src] = 0;

    for (int count = 0; count < V - 1; count++)
    {
        int u = minDistance(dist, sptSet);

        sptSet[u] = true;

        for (int v = 0; v < V; v++)
        {
            if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX && dist[u] + graph[u][v] < dist[v])
            {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }
    printSolution(dist);
}
