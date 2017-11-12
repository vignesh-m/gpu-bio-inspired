#include "ant_colony.h"

void Graph::add_edge(int i, int j, double weight)
{
    adj[i].push_back(make_pair(j, weight));
    adj[j].push_back(make_pair(i, weight));
}

Graph::Graph(string filename)
{
    // read from file
    // Format:

    // N(no of vertices) M(no of edges)
    // <node 1> <node 2> <distance>
    ifstream f;
    f.open(filename, ios::in);
    int m;
    f >> n >> m;
    adj.resize(n);
    for (int i = 0; i < m; i++)
    {
        int u, v;
        double w;
        f >> u >> v >> w;
        add_edge(u, v, w);
    }
}
