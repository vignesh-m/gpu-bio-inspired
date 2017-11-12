#ifndef __ANT_COLONY_H
#define __ANT_COLONY_H 1

#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include <fstream>
#include "prettyprint.hpp"

using namespace std;
typedef pair<int, double> edge_t;
typedef vector<int> vi;
typedef vector<double> vd;
typedef vector<vector<edge_t>> adj_t;
typedef vector<vector<double>> pmap_t;

class Graph
{
  public:
    int n;
    adj_t adj; // vertex, weight
    Graph(char *filename);
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


class Ant
{
  public:
    pmap_t &pmap;
    Graph &g;
    vi path;
    vi reached;
    int n;
    double cost;
    Ant(pmap_t &_ph, Graph &_g) : pmap(_ph), g(_g)
    {
        n = g.n;
        path = vi(n);
        reached = vi(n);
        cost = 0.0;
    };

    int next_vertex(int i,double &rcost);
    vi gen_sol();
    void ph_update();
};
int ant_colony_opt_tsp(vi &path, double &cost,Graph &g);

inline ostream &operator<<(ostream &out, const Ant &a)
{
    out<<"Ant path: "<<a.path<<" , cost: "<<a.cost;
    return out;
}
inline ostream &operator<<(ostream &out, const Graph &g)
{
    for (int i = 0; i < g.n; i++)
    {
        out << i << ": ";
        for (edge_t e : g.adj[i])
        {
            out << e << " ";
        }
        out << "\n";
    }
    return out;
}
int randint(int);
int randprob(double);
double rand01();
void coords_to_edge(string infile, string outfile);
void adjmat_to_edge(string infile, string outfile);
#endif