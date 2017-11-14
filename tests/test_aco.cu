#include <cstdio>
#include <iostream>
#include "ant_colony.h"

int main(){
    srand(time(NULL));

    cout << "Hello\n";
    Graph g = input_graph();
    int n=g.n;
    //print_graph(g);

    int *path = new int[n];
    double cost;
    double* gadj = new double[n*n];
    to_1d(gadj,g);
    ant_colony_opt_tsp(g.n, gadj, path, cost);

    print_vi(path,n);
    cout<<" "<<cost<<"\n";

    cudaDeviceSynchronize();
}
