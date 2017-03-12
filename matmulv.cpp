#include <iostream>
#include <omp.h>
#include <stdlib.h>
#include <chrono>
using namespace std;
void Serial(int *a, int *b, int *c, int R, int C){
	for(int i = 0; i < R; ++i)
        for(int j = 0; j < C; ++j)
            for(int k = 0; k < C; ++k)
            {
                c[i*C+j] += a[i*C+k] * b[k*C+j];
            }
}
void Only_vector(int *a, int *b, int *c, int R, int C){
	for(int i = 0; i < R; ++i)
        for(int j = 0; j < C; ++j)
        	#pragma omp simd //reduction (+:c[i*C+j])
            for(int k = 0; k < C; ++k)
            {
                c[i*C+j] += a[i*C+k] * b[k*C+j];
            }
}
void Parallel_without_vector(int *a, int *b, int *c, int R, int C){
	#pragma omp parallel for
	for(int i = 0; i < R; ++i)
        for(int j = 0; j < C; ++j)
            for(int k = 0; k < C; ++k)
            {
                c[i*C+j] += a[i*C+k] * b[k*C+j];
            }
}
void Parallel_with_vector(int *a, int *b, int *c, int R, int C){
		#pragma omp parallel for
		for(int i = 0; i < R; ++i)
        	for(int j = 0; j < C; ++j)
        		#pragma omp simd //reduction (+:c[i*C+j])
            	for(int k = 0; k < C; ++k)
            	{
                	c[i*C+j] += a[i*C+k] * b[k*C+j];
            	}
}
int main(int argc, char* argv[]){
	int *a, *b, *c, *d, R, C, nt;
	if (argc >= 4){
        R = atoi(argv[1]);
        if (argc == 4){
        	C = atoi(argv[2]);
            nt = atoi(argv[3]);
        }
    } else {
        fprintf(stderr, "usage: %s <Number of rows> <Number of columns> <Number of threads>\n", argv[0]);
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
    d = (int *) malloc(R * C * sizeof (int));
	if (d == NULL) {
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
        
    auto start0 = std::chrono::high_resolution_clock::now();
    Serial(a, b, c, R, C);
    auto end0 = std::chrono::high_resolution_clock::now();
    cout << "Serial run time in millisecs: " << (std::chrono::duration_cast<chrono::milliseconds>(end0 - start0)).count() << "\n";

    auto start1 = std::chrono::high_resolution_clock::now();
    Only_vector(a, b, c, R, C);
    auto end1 = std::chrono::high_resolution_clock::now();
    cout << "Only vector run time in millisecs: " << (std::chrono::duration_cast<chrono::milliseconds>(end1 - start1)).count() << "\n";

    auto start2 = std::chrono::high_resolution_clock::now();
    Parallel_without_vector(a, b, c, R, C);
    auto end2 = std::chrono::high_resolution_clock::now();
    cout << "Parallel without vector run time in millisecs: " << (std::chrono::duration_cast<chrono::milliseconds>(end2 - start2)).count() << "\n";

    auto start3 = std::chrono::high_resolution_clock::now();
    Parallel_with_vector(a, b, c, R, C);
    auto end3 = std::chrono::high_resolution_clock::now();
    cout << "Parallel with vector run time in millisecs: " << (std::chrono::duration_cast<chrono::milliseconds>(end3 - start3)).count() << "\n";

	return 0;
}
