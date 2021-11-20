#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>
#include <iostream>

#define NUM_THREADS 10

struct Thread_Args {
    int from_row;
    int batch_size;
    int column;
};

int** array;
int* result;


#pragma region Utils
void fill_matrix(int** matrix, int row, int column) {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            matrix[i][j] = rand();
        }
    }
}

int get_max_arr_element(int* arr, int length) {
    int max = INT_MIN;
    for (int i = 0; i < length; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}
#pragma endregion Utils


#pragma region Example1
void *find_max_elem(void *thread_info) {
    int max = INT_MIN;
    Thread_Args* args = (Thread_Args*)thread_info;
    int to_row = args->from_row + args->batch_size;
    for (int i = args->from_row; i < to_row; i++) {
        for (int j = 0; j < args->column; j++) {
            if (array[i][j] > max) {
                max = array[i][j];
            }
        }
    }
    int index = args->from_row / args->batch_size;
    result[index] = max;
    return 0;
}

void example1(int row, int column) {
    result = (int*)malloc(NUM_THREADS * sizeof(int));
    pthread_t threads[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        Thread_Args* ta = new Thread_Args();
        ta->batch_size = row / NUM_THREADS;
        ta->column = column;
        ta->from_row = i * ta->batch_size;
        pthread_create(&threads[i], NULL, find_max_elem, (void*)ta);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    int max_element = get_max_arr_element(result, NUM_THREADS);
}
#pragma endregion Example1

int main()
{
#pragma region Example1
    srand(time(NULL));
    const int row = 100;
    const int column = 1000000;
    array = (int**)malloc(row * sizeof(int*));
    for (int i = 0; i < row; i++)
        array[i] = (int*)malloc(column * sizeof(int));
    fill_matrix(array, row, column);
    double min_time = 10.0;
    for (int i = 0; i < 10; i++) {
        clock_t begin = clock();
        example1(row, column);
        clock_t end = clock();
        double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        if (time_spent < min_time) {
            min_time = time_spent;
        }
    }
    printf("The elapsed time is %f milliseconds\n", min_time * 1000);
    for (int i = 0; i < row; i++)
        free(array[i]);
    free(array);
#pragma endregion Example1

#pragma region Example2
#pragma endregion Example2
}
