#include <iostream>
#include <omp.h>
#include <stdlib.h>
#include <chrono>
using namespace std;
int main(int argc, char* argv[]){
	int *a, *b, *c, R, C, nt;
	if (argc >= 4){
        R = atoi(argv[1]);
        if (argc == 4){
        	C = atoi(argv[2]);
            nt = atoi(argv[3]);
        }
    } else {
        fprintf(stderr, "usage: %s <Number of rows> <Number of columns> <number_of_threads>\n", argv[0]);
        exit(-1);
    }
    omp_set_num_threads(nt);
	a = (int *) malloc(R * C * sizeof (int));
	if (a == NULL) {
        cerr << "Error : Your size is too much.\n";
        exit(1);
    }
    b = (int *) malloc(R * C * sizeof (int));
	if (b == NULL) {
        cerr << "Error : Your size is too much.\n";
        exit(1);
    }
    c = (int *) malloc(R * C * sizeof (int));
	if (c == NULL) {
        cerr << "Error : Your size is too much.\n";
        exit(1);
    }
    for(int i = 0; i < R; ++i)
        for(int j = 0; j < C; ++j)
        {
            a[i*C+j]=i+1;
            b[i*C+j]=i*i;
            c[i*C+j]=0;
        }
    auto start = std::chrono::high_resolution_clock::now();
    #pragma omp parallel for
    for(int i = 0; i < R; ++i)
        for(int j = 0; j < C; ++j)
        	#pragma vector aligned
        	#pragma ivdep
            for(int k = 0; k < C; ++k)
            {
                c[i*C+j] += a[i*C+k] * b[k*C+j];
            }
    auto end = std::chrono::high_resolution_clock::now();
    cout << "time in millisecs: " << (std::chrono::duration_cast<chrono::milliseconds>(end - start)).count() << "\n";
	return 0;
}
