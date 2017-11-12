#include <cstdio>
#include <iostream>
#include "ant_colony.h"

int main(){
    srand(time(NULL));

    cout << "Hello\n";
    Graph g = input_graph();
    print_graph(g);

    vi path;
    double cost;
    ant_colony_opt_tsp(path, cost, g);

    print_vi(path);
    cout<<" "<<cost<<"\n";

    printf("pls\n");
    cudaThreadSynchronize();
}
