#include <cstdio>
#include <iostream>
#include "ant_colony.h"

__global__ void test_k(){
    printf("id: %d\n",threadIdx.x);
}

int main(){
    srand(time(NULL));

    cout << "Hello\n";
    Graph g(s);
    print_graph(g);
    test_k<<<1,2>>>();
    printf("pls\n");
    cudaThreadSynchronize();
}
