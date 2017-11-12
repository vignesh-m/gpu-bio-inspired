#include <cstdlib>
#include "ant_colony.h"

int randint(int n)
{
    // return number from {0,1,... n-1}
    return rand() % n;
}

int randprob(double p)
{
    // return 1 with probability p
    double d = ((double)rand()) / RAND_MAX;
    return d < p;
}

double rand01()
{
    return ((double)randint(100)) / 100.0;
}

void coords_to_edge(string infile, string outfile){
    // accepts format
    // n
    // <node> <x coord> <y coord>
    // outputs our format (from graph read)
    ifstream in(infile);
    ofstream out(outfile);
    int n;
    in>>n;
    vd x(n),y(n);
    int r;
    for(int i=0;i<n;i++) in>>r>>x[i]>>y[i];
    out<<n<<" "<<(n*(n-1))/2<<"\n";
    for(int i=0;i<n;i++) for(int j=i+1;j<n;j++) {
        double d = sqrt((x[i]-x[j])*(x[i]-x[j]) + (y[i]-y[j])*(y[i]-y[j]));
        out<<i<<" "<<j<<" "<<d<<"\n";
    }
}

void adjmat_to_edge(string infile, string outfile){
    // accepts format
    // n
    // a00 a01 ....
    // .
    // outputs our format (from graph read)
    ifstream in(infile);
    ofstream out(outfile);
    int n;
    in>>n;
    out<<n<<" "<<n*n<<"\n";
    for(int i=0;i<n;i++) for(int j=0;j<n;j++){
        double w;
        in>>w;
        out<<i<<" "<<j<<" "<<w<<"\n";
    }
}