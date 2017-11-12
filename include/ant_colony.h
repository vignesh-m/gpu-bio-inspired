#ifndef __ANT_COLONY_H
#define __ANT_COLONY_H 1

#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;
typedef pair<int, double> edge_t;
typedef vector<int> vi;
typedef vector<double> vd;
typedef vector<vector<double> > adj_t;
typedef vector<vector<double> > pmap_t;

class Graph
{
    // adj -> adjacency matrix
  public:
    int n;
    adj_t adj; // vertex, weight
    Graph(string filename);
    Graph() {}
    Graph(int _n) {
        n = _n;
        adj = adj_t(n, vd(n));
    }
    Graph(adj_t _adj) : adj(_adj)
    {
        n = adj.size();
    }

    // vector<edge_t> &operator[](int idx) const
    // {
    //     return adj[idx];
    // }
    void add_edge(int i, int j, double weight = 0);
};


int ant_colony_opt_tsp(int n, double* gadj,int *path, double &cost);


inline void print_vi(vi a){
    for(int i=0;i<a.size();i++) cout<<a[i]<<" ";
}
inline __host__ __device__ void print_vi(int* a, int n){
    for(int i=0;i<n;i++) printf("%d ",a[i]);
}
inline void print_graph(Graph &g)
{
    for (int i = 0; i < g.n; i++)
    {
        cout << i << ": ";
        for (int j=0;j<g.n;j++)
        {
            cout<<g.adj[i][j]<<" ";
        }
        cout << "\n";
    }
}
int randint(int);
int randprob(double);
double rand01();
void coords_to_edge(string infile, string outfile);
Graph input_graph();
void adjmat_to_edge(string infile, string outfile);
void to_1d(double* gadj,Graph &g);
#endif
