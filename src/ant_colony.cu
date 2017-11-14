#include <iostream>
#include "ant_colony.h"
#include <curand_kernel.h>

double PH_EVAP = 0.5;
double ALPHA = 1;
double BETA = 1;

__device__ void gen_sol(int* path,double* cost,int n,double* gadj, double* pmap, curandState &s){
    int *reached = (int*) malloc(n*sizeof(int));
    for(int i=0;i<n;i++) reached[i]=0;
    int *vert= (int*) malloc(n*sizeof(int));
    double *cprob= (double*) malloc(n*sizeof(double));
    reached[0] = 1;
    path[0] = 0;
    *cost = 0;
    for (int i = 1; i < n; i++)
    {
        int u=path[i-1];
        // pick next vertex - i already filled
        // weight by pmap[i][j] / g.adj[i][j].second
        // TODO weight by pmap[i][j]^alpha / g.adj[i][j].second^beta
        int sz=0;
        double tot=0.0;
        for(int v=0; v<n;v++){
            if(reached[v]) continue;       
            //printf("u %d v %d p %f g %f\n",u,v,pmap[u*n+v],gadj[u*n+v]);
            double p = pmap[u*n+v] / gadj[u*n + v];
            
            cprob[sz] = p+tot;
            vert[sz]=v;
            sz++;
            tot+=p;
        
        }
        double rnd =curand_uniform(&s)*tot;
        //printf("rnd %f\n",rnd);
        for (int k = 0; k < sz; k++)
        {
            if (rnd < cprob[k])
            {
                int v=vert[k];
                //printf("picking vertex %d\n",v);
                path[i]=v;
                reached[v]=1;
                *cost += gadj[u*n+v];
                break;
            }
        }
        //path[i]=u+1;
        //reached[i]=1;
        //continue;
    }   
    // add path[n-1]-path[0] cost
    *cost += gadj[path[n-1]*n + path[0]];
}


void ph_update(int n,int* path, double* cost, double* pmap){
    int v = path[0],u;
    for(int i=1; i<n;i++){
        u = path[i];
        // update ph at edge v-u
        pmap[v*n+u] = (1- PH_EVAP)*pmap[v*n+u] + 1/(*cost);
        v = u;
    }
    u = 0;
    pmap[v*n+u] = (1- PH_EVAP)*pmap[v*n+u] + 1/(*cost);
}



__global__ void test_k(int n, int *paths, double* costs, double *gadj, double *p, int seed){
    int i = threadIdx.x;
    //printf("id: %d %d\n",i, n);
    if(i >=n ) return;
    
    curandState s;
    curand_init(i, seed, 0, &s);

    int *rpath = paths+i*n;
    double *rcost = costs+i;
    for(int i=0;i<n;i++) rpath[i]=-1;
    gen_sol(rpath, rcost, n,gadj, p, s);
    //printf("id %d cost %f cost' %f\n",i,*rcost,costs[i]);

}



int ant_colony_opt_tsp(int n, double* gadj, int *path, double &cost)
{
    /*
        Ant Colony System, specific to TSP
        Takes weighted graph input <g>, fills up best found path in <path>
    */
    int NI = 100,
        NA = 100;
        
    double *p = new double[n*n];
    for(int i=0;i<n*n;i++) p[i]=1e-5;
    cost=1.0e10;
    for (int iter = 0; iter < NI; iter++)
    {
        int *paths = new int[NA*n]; // paths[ant] = ant's path
        double *costs = new double[NA];
        int *paths_d;
        double *costs_d, *gadj_d, *p_d;
        cudaMalloc(&paths_d, NA*n*sizeof(int));
        cudaMalloc(&costs_d, NA*sizeof(double));
        cudaMalloc(&gadj_d, n*n*sizeof(double));
        cudaMalloc(&p_d, n*n*sizeof(double));

        cudaMemcpy(gadj_d, gadj, n*n*sizeof(double), cudaMemcpyHostToDevice);
        cudaMemcpy(p_d, p, n*n*sizeof(double), cudaMemcpyHostToDevice);

        test_k<<<1,NA>>>(n,paths_d, costs_d, gadj_d, p_d,iter);

        cudaThreadSynchronize();

        cudaMemcpy(paths, paths_d, NA*n*sizeof(int), cudaMemcpyDeviceToHost);
        cudaMemcpy(costs, costs_d, NA*sizeof(double), cudaMemcpyDeviceToHost);
        
        for(int i=0;i<0;i++){
            print_vi(paths+i*n,n);
            printf(", cost=%f\n",costs[i]);
        }

        int mini=-1;
        for (int i= 0; i< NA; i++)
        {
            int *rpath = paths+i*n;
            double *rcost = costs+i;
            if(*rcost < cost){
                mini=i;
                cost = *rcost;
            }   
            ph_update(n,rpath, rcost, p);
        }
        if(mini!=-1){
            for(int i=0;i<n;i++) path[i]=paths[mini*n+i];
        }

    }
    return 0;
}
