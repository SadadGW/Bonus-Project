#include <stdio.h>
#include <stdlib.h>

#define SIZE 100000

int main() {
    double* a;
    double sum = 0.0;

    a = (double*)malloc(SIZE * sizeof(double));
    if (a == NULL) {
        printf("Was unable to allocate!\n");
        return 1;
    }

    FILE* file = fopen("numbers.txt", "r");
    if (file == NULL) {
        printf("File open failure!\n");
        free(a);
        return 1;
    }

    for (int i = 0; i < SIZE; i++) {
        if (fscanf_s(file, "%lf", &a[i]) != 1) {
            printf("Failed to read data for element %d.\n", i);
            fclose(file);
            free(a);
            return 1;
        }
        sum += a[i];
    }

    fclose(file);
    printf("The total sum is %lf\n", sum);
    free(a);

    return 0;
}
