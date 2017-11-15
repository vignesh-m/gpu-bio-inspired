#include <iostream>
#include "ant_colony.h"
#include <ctime>
#include <curand_kernel.h>

double ALPHA = 1;
double BETA = 1;

__device__ void gen_sol(int* path,double* cost,int n,double* gadj, double* pmap, curandState &s){
    int *reached = (int*) malloc(n*sizeof(int)); // reached
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

__device__ void atomicUpdate (double* pval_addr, double cost)  {  
	unsigned long long int* address_as_ull = (unsigned long long int*) pval_addr; 
  	unsigned long long int old = *address_as_ull, assumed, newVal; 
	double rCost = (1.0 / cost);
	double PH_EVAP = 0.5;
	double PH_STAY = 1 - PH_EVAP;

  	do { 
 		assumed = old;
		newVal = __double_as_longlong(rCost + PH_STAY * __longlong_as_double(assumed));
 		old = atomicCAS(address_as_ull, assumed, newVal); 
  	} while (assumed != old);
 
	//return __longlong_as_double(old);
}      


/*void ph_update(int n,int* path, double* cost, double* pmap){
    //printf("updating: ");
    //print_vi(path, n);
    //printf(", cost: %f\n", *cost);
    
    int v = path[0],u;
    for(int i=1; i<n;i++){
        u = path[i];
        // update ph at edge v-u
        //printf("update edge %d-%d\n",v,u);
        pmap[v*n+u] = (1- PH_EVAP)*pmap[v*n+u] + 1/(*cost);
        v = u;
    }
    u = 0;
    //printf("update edge %d-%d\n",v,u);
    pmap[v*n+u] = (1- PH_EVAP)*pmap[v*n+u] + 1/(*cost);
}*/


__device__ void ph_update (int n,int* path, double cost, double* pmap){
    //printf("updating: ");
    //print_vi(path, n);
    //printf(", cost: %f\n", cost);
    
    int i;
    for (i = 0; i < n-1; i++) {
        atomicUpdate(&pmap[path[i] * n + path[i+1]], cost);
    }
    atomicUpdate(&pmap[path[n-1] * n], cost);
}



__global__ void test_k(int n, int *paths, double* costs, double *gadj, double *p, int seed, double *cost, int *mini, int *path){
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

    
    unsigned long long oldCost, assumedCost, newCost;
    int oldMini, assumedMini, newMini;
    
    if(*rcost < *cost){
	unsigned long long *ullCost = (unsigned long long *) cost;
	oldCost = *ullCost;
	do { 
 	  assumedCost = oldCost;
	  newCost = __double_as_longlong(*rcost);
 	  oldCost = atomicCAS(ullCost, assumedCost, newCost); 
  	} while (assumedCost != oldCost);

	oldMini = *mini;
	do { 
 	  assumedMini = oldMini;
	  newMini = i;
 	  oldMini = atomicCAS(mini, assumedMini, newMini); 
  	} while (assumedMini != oldMini);
    }
    __syncthreads();

    if (*mini != -1) {
       if(i < n) {
            path[i]=paths[(*mini)*n+i];
       }
    }
    __syncthreads();

    ph_update(n, rpath, *rcost, p);
}



int ant_colony_opt_tsp(int n, double* gadj, int *path, double &cost)
{
    /*
        Ant Colony System, specific to TSP
        Takes weighted graph input <g>, fills up best found path in <path>
    */
    int NI = 1000,
        NA = 10;

    clock_t st = clock();
        
    double *p = new double[n*n];
    for(int i=0;i<n*n;i++) p[i]=1e-5;
    cost=1.0e10;

    int *paths = new int[NA*n]; // paths[ant] = ant's path
    double *costs = new double[NA];
    int *paths_d;
    double *costs_d, *gadj_d, *p_d;
    double *cost_d;
    int *mini_d;
    int *path_d;

    cudaMalloc(&paths_d, NA*n*sizeof(int));
    cudaMalloc(&costs_d, NA*sizeof(double));
    cudaMalloc(&gadj_d, n*n*sizeof(double));
    cudaMalloc(&p_d, n*n*sizeof(double));
    cudaMalloc(&cost_d, sizeof(double));
    cudaMalloc(&mini_d, sizeof(double));
    cudaMalloc(&path_d, n*sizeof(int));

    cudaMemcpy(gadj_d, gadj, n*n*sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(p_d, p, n*n*sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(cost_d, &cost, sizeof(double), cudaMemcpyHostToDevice);

    for (int iter = 0; iter < NI; iter++)
    {
        /*int *paths = new int[NA*n]; // paths[ant] = ant's path
        double *costs = new double[NA];
        int *paths_d;
        double *costs_d, *gadj_d, *p_d;
	double *cost_d;

        cudaMalloc(&paths_d, NA*n*sizeof(int));
        cudaMalloc(&costs_d, NA*sizeof(double));
        cudaMalloc(&gadj_d, n*n*sizeof(double));
        cudaMalloc(&p_d, n*n*sizeof(double));
	cudaMalloc(&cost_d, sizeof(double));

        cudaMemcpy(gadj_d, gadj, n*n*sizeof(double), cudaMemcpyHostToDevice);
        cudaMemcpy(p_d, p, n*n*sizeof(double), cudaMemcpyHostToDevice);*/

	int mini = -1;
	cudaMemcpy(mini_d, &mini, sizeof(int), cudaMemcpyHostToDevice);

        test_k<<<1,NA>>>(n,paths_d, costs_d, gadj_d, p_d,iter, cost_d, mini_d, path_d);

        cudaThreadSynchronize();

        //cudaMemcpy(paths, paths_d, NA*n*sizeof(int), cudaMemcpyDeviceToHost);
        //cudaMemcpy(costs, costs_d, NA*sizeof(double), cudaMemcpyDeviceToHost);
	//cudaMemcpy(&cost, cost_d, sizeof(double), cudaMemcpyDeviceToHost);

        
        /*for(int i=0;i<0;i++){
            print_vi(paths+i*n,n);
            printf(", cost=%f\n",costs[i]);
        }*/

        /*for (int i= 0; i< NA; i++)
        {
            double *rcost = costs+i;
            if(*rcost < cost){
                mini=i;
            }   
        */

	/*cudaMemcpy(&mini, mini_d, sizeof(int), cudaMemcpyDeviceToHost);
        if(mini!=-1){
            for(int i=0;i<n;i++) path[i]=paths[mini*n+i];
        }*/


        /*delete[] paths;
        delete[] costs;
        cudaFree(gadj_d);
        cudaFree(p_d);
        cudaFree(paths_d);
        cudaFree(costs_d);
	cudaFree(cost_d);*/
    }

    cudaMemcpy(path, path_d, n*sizeof(int), cudaMemcpyDeviceToHost);
    cudaMemcpy(&cost, cost_d, sizeof(double), cudaMemcpyDeviceToHost);

    delete[] paths;
    delete[] costs;
    cudaFree(gadj_d);
    cudaFree(p_d);
    cudaFree(paths_d);
    cudaFree(costs_d); 
    cudaFree(cost_d);
    cudaFree(path_d);

    delete[] p;
    clock_t en = clock();
    double t = (double)(en-st)/CLOCKS_PER_SEC;
    printf("Evaluated %d ants each\n",NI*NA);
    printf("Took time %f\n",t);
    return 0;
}
