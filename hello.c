#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	omp_set_num_threads(16);
#pragma omp parallel
	{
		printf("Hello World! With love, Thread %02d.\n",
			omp_get_thread_num());
	}
}
