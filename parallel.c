#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define SIZE 100000
#define THREADS 16

int main() {
    int i;
    double* A = malloc(SIZE * sizeof(double));
    if (A == NULL) {
        printf("Allocation failure!\n");
    }

    FILE* file = fopen("numbers.txt", "r");
    if (file == NULL) {
        perror("File couldn't open!");
        free(A);
    }

    for (i = 0; i < SIZE; i++) {
        if (fscanf_s(file, "%lf", &A[i]) != 1) {
            printf("Error reading data at element %d!\n", i);
            fclose(file);
            free(A);
        }
    }
    fclose(file);

    double total = 0.0;
    omp_set_num_threads(THREADS);

    double threadTime[THREADS] = { 0 };
    double setParallel, endParallel;
    printf("Number of threads: %d\n", omp_get_max_threads());
    puts("-------------------------------------------------------------------------------");
    setParallel = omp_get_wtime();
#pragma omp parallel reduction(+:total) private(i)
    {
        int threadID = omp_get_thread_num();
        double start = omp_get_wtime();

#pragma omp for schedule(static)
        for (i = 0; i < SIZE; i++) {
            total += A[i];
        }

        double end = omp_get_wtime();
        threadTime[threadID] = end - start;

        int threadCount = omp_get_num_threads();
        int chunk = SIZE / threadCount;
        printf("Thread [%02d] processed elements from [%05d] to [%05d] within %f seconds.\n", threadID, (threadID * chunk), ((threadID + 1) * chunk - 1), threadTime[threadID]);
    }
    endParallel = omp_get_wtime();

    double longestTime = 0.0;
    for (i = 0; i < THREADS; i++) {
        if (threadTime[i] > longestTime) {
            longestTime = threadTime[i];
        }
    }
    puts("-------------------------------------------------------------------------------");
    printf("The total sum of numbers from the text file: %33lf.\n", total);
    printf("The longest thread execution time: %35f seconds.\n", longestTime);
    printf("The total parallel region execution time: %28f seconds.\n", endParallel - setParallel);
    puts("-------------------------------------------------------------------------------");
    free(A);
    return 0;
}
