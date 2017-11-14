#if __CUDA_ARCH__ < 600 __device__ double atomicAdd(double* address, double val) { unsigned long long int* address_as_ull = (unsigned long long int*)address; unsigned long long int old = *address_as_ull, assumed; do { assumed = old; old = atomicCAS(address_as_ull, assumed, __double_as_longlong(val + __longlong_as_double(assumed))); // Note: uses integer comparison to avoid hang in case of NaN (since NaN != NaN) } while (assumed != old); return __longlong_as_double(old); }

Read more at: http://docs.nvidia.com/cuda/cuda-c-programming-guide/index.html#ixzz4yNcgfg8l 
Follow us: @GPUComputing on Twitter | NVIDIA on Facebook
