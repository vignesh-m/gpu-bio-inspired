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
    char *s = "../graphs/test03";
    Graph g(s);
    cout << g << "\n";

    vi path;
    double cost;
    ant_colony_opt_tsp(path, cost,g);
    cout<<path<<" "<<cost<<"\n";

    // convert();
}

