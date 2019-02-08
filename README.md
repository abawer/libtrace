# libtrace
A simple heap allocation tracer for linux cpp applications.
Would track down memory leaks, double frees, free of unallocated memory, using delete for new [] scenarios and vice versa.

To use it with your code:
1. create the libtrace.so by running make all at project folder.
2. add #include "inc/trace.h" as the very first line of your main.cpp file.
3. compile your code and link it to lib/libtrace.so, for example: g++ main.cpp -I./inc -L./lib -ltrace
4. run your executable to be checked.

libtrace code does not use memory allocations to trace down memory allocations but rather a simple array of metadata. You can control the array size with -DMAX_ALLOCS=10000 (this is the default) compilation flag specifiying the maximal number of tracked allocations.
