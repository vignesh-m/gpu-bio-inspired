#include "ant_colony.h"

void convert(){
    string in ="../graphs/test03_adj";
    string out ="../graphs/test03";
    adjmat_to_edge(in ,out);
}

int main()
{
    srand(time(NULL));

    cout << "Hello\n";
    string s = "../graphs/test03";
    Graph g(s);
    print_graph(g);

    vi path;
    double cost;
    ant_colony_opt_tsp(path, cost,g);
    print_vi(path);
    cout<<" "<<cost<<"\n";

    // convert();
}

