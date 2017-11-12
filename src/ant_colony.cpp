#include <iostream>
#include "ant_colony.h"

double PH_EVAP = 0.5;
double ALPHA = 1;
double BETA = 1;

int Ant::next_vertex(int i, double &rcost)
{
    // Random
    // int idx = randint(n);
    // for (int j = 0; j < n; j++)
    // {
    //     int ci = (idx + j) % n;
    //     if (!reached[ci])
    //     {
    //         return ci;
    //     }ix
    // }
    // weight by pmap[i][j] / g.adj[i][j].second
    // TODO weight by pmap[i][j]^alpha / g.adj[i][j].second^beta
    vd cprob, cost;
    vi vert;
    double tot = 0;
    int nleft = 0;
    for (edge_t e : g.adj[i])
    {
        int j = e.first;
        double w = e.second;
        if (!reached[j])
        {
            double p = pmap[i][j] / w;
            // double p = 1.0;
            cprob.push_back(p + tot);
            vert.push_back(j);
            cost.push_back(w);
            tot += p;
            nleft++;
        }
    }
    double rnd = rand01();
    rnd = rnd * tot;
    // cout << cprob << " " << rnd << "\n";
    // cout << vert << "\n";
    for (int k = 0; k < nleft; k++)
    {
        if (rnd < cprob[k])
        {
            // cout << vert[k] << " chosen\n";
            rcost = cost[k];
            return vert[k];
        }
    }
    return -1;
}
vi Ant::gen_sol()
{
    int n = g.n;

    reached[0] = 1;
    path[0] = 0;
    for (int i = 1; i < n; i++)
    {
        // pick next vertex - i already filled
        double rcost=0;
        int j = next_vertex(path[i - 1], rcost);
        cost += rcost;
        path[i] = j;
        reached[j] = 1;
    }
    // add path[n-1]-path[0] cost
    for(edge_t e: g.adj[path[n-1]]){
        if(e.first==0) cost += e.second;
    }
    return path;
}
void Ant::ph_update(){
    int v = path[0],u;
    for(int i=1; i<n;i++){
        u = path[i];
        // update ph at edge v-u
        pmap[v][u] = (1- PH_EVAP)*pmap[v][u] + 1/cost;
        v = u;
    }
    u = 0;
    pmap[v][u] = (1- PH_EVAP)*pmap[v][u] + 1/cost;
}
int ant_colony_opt_tsp(vi &path, double &cost,Graph &g)
{
    /*
        Ant Colony System, specific to TSP
        Takes weighted graph input <g>, fills up best found path in <path>
    */
    int n = g.n;
    int NI = 100,
        NA = 100;
        
    pmap_t p(n, vd(n, 1e-5));
    cost=1.0e10;
    for (int iter = 0; iter < NI; iter++)
    {
        vector<Ant> ants(NA, Ant(p, g));
        for (int i= 0; i< NA; i++)
        {
            ants[i].gen_sol();
            if(ants[i].cost<cost){
                cost = ants[i].cost;
                path = ants[i].path;
            }
        }
        for (int i= 0; i< NA; i++)
        {
            ants[i].ph_update();
        }
        cout<<ants[0]<<"\n";
    }
}
