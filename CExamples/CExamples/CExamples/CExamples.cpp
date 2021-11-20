#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>
#include <iostream>

#define NUM_THREADS_FIRST_EXAMPLE 10
#define NUM_THREADS_SECOND_EXAMPLE 5
#define SUDOKU_SIZE 9 

struct Thread_Args {
	int from_row;
	int batch_size;
	int column;
};

struct Sudoku_dictionary {
	int number;
	int count;
};

int** matrix;
int* result;
int sudoku[SUDOKU_SIZE][SUDOKU_SIZE] = {
	{1, 4, 7, 0, 0, 0, 0, 0, 3},
	{2, 5, 0, 0, 0, 1, 0, 0, 0},
	{3, 0, 9, 0, 0, 0, 0, 0, 0},
	{0, 8, 0, 0, 2, 0, 0, 0, 4},
	{0, 0, 0, 4, 1, 0, 0, 2, 0},
	{9, 0, 0, 0, 0, 0, 6, 0, 0},
	{0, 0, 3, 0, 0, 0, 0, 0, 9},
	{4, 0, 0, 0, 0, 2, 0, 0, 0},
	{0, 0, 1, 0, 0, 8, 0, 0, 7},
};

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

bool all_elements_uniq(int* arr)
{
	int origVal = 0, newVal = 0;
	for (int i = 0; i < SUDOKU_SIZE; i++) {
		origVal = arr[i];
		for (int k = i + 1; k < SUDOKU_SIZE; k++) {
			if (origVal != 0 && origVal == arr[k])
			{
				return false;
			}
		}
	}
	return true;
}
#pragma endregion Utils


#pragma region Example1
void* find_max_elem(void* thread_info) {
	int max = INT_MIN;
	Thread_Args* args = (Thread_Args*)thread_info;
	int to_row = args->from_row + args->batch_size;
	for (int i = args->from_row; i < to_row; i++) {
		for (int j = 0; j < args->column; j++) {
			if (matrix[i][j] > max) {
				max = matrix[i][j];
			}
		}
	}
	int index = args->from_row / args->batch_size;
	result[index] = max;
	return 0;
}

void example1(int row, int column) {
	result = (int*)malloc(NUM_THREADS_FIRST_EXAMPLE * sizeof(int));
	pthread_t threads[NUM_THREADS_FIRST_EXAMPLE];
	for (int i = 0; i < NUM_THREADS_FIRST_EXAMPLE; i++) {
		Thread_Args* ta = new Thread_Args();
		ta->batch_size = row / NUM_THREADS_FIRST_EXAMPLE;
		ta->column = column;
		ta->from_row = i * ta->batch_size;
		pthread_create(&threads[i], NULL, find_max_elem, (void*)ta);
	}
	for (int i = 0; i < NUM_THREADS_FIRST_EXAMPLE; i++) {
		pthread_join(threads[i], NULL);
	}
	int max_element = get_max_arr_element(result, NUM_THREADS_FIRST_EXAMPLE);
	free(result);
}
#pragma endregion Example1

#pragma region Example2

typedef struct TrippleSquareValidatorParams {
	int sudokuRowNum;
	int index_for_result;
};

void* tripple_square_validator(void* params)
{
	TrippleSquareValidatorParams* args = (TrippleSquareValidatorParams*)params;
	for (int i = args->sudokuRowNum * 3; i < (args->sudokuRowNum + 1) * 3; i += 3)
	{
		for (int j = 0; j < SUDOKU_SIZE; j += 3)
		{
			int* row_elements = (int*)malloc(SUDOKU_SIZE * sizeof(int));
			int index = 0;
			for (int sqi = i; sqi < i + 3; sqi++)
			{
				for (int sqj = j; sqj < j + 3; sqj++)
				{
					int currentElement = sudoku[sqi][sqj];
					row_elements[index++] = currentElement;
				}
			}
			if (!all_elements_uniq(row_elements))
			{
				free(row_elements);
				result[args->index_for_result] = 0;
				return NULL;
			}
			free(row_elements);
		}
	}
	result[args->index_for_result] = 1;
	return NULL;
}

void* columns_validator(void* index_for_result_pointer) {
	int index_for_result = (int)index_for_result_pointer;
	for (int i = 0; i < SUDOKU_SIZE; i++)
	{
		int* row_elements = (int*)malloc(SUDOKU_SIZE * sizeof(int));
		for (int j = 0; j < SUDOKU_SIZE; j++)
		{
			int currentElement = sudoku[j][i];
			row_elements[j] = currentElement;
		}
		if (!all_elements_uniq(row_elements))
		{
			free(row_elements);
			result[index_for_result] = 0;
			return NULL;
		}
		free(row_elements);
	}
	result[index_for_result] = 1;
	return NULL;
}

void* rows_validator(void* index_for_result_pointer) {
	int index_for_result = (int)index_for_result_pointer;
	for (int i = 0; i < SUDOKU_SIZE; i++)
	{
		int* row_elements = (int*)malloc(SUDOKU_SIZE * sizeof(int));
		for (int j = 0; j < SUDOKU_SIZE; j++)
		{
			int currentElement = sudoku[i][j];
			row_elements[j] = currentElement;
		}
		if (!all_elements_uniq(row_elements))
		{
			free(row_elements);
			result[index_for_result] = 0;
			return NULL;
		}
	}
	result[index_for_result] = 1;
	return NULL;
}
bool is_sudoku_valid() {
	result = (int*)malloc(NUM_THREADS_SECOND_EXAMPLE * sizeof(int));
	pthread_t threads[NUM_THREADS_SECOND_EXAMPLE];
	pthread_create(&threads[0], NULL, rows_validator, (void*)0);
	pthread_create(&threads[1], NULL, columns_validator, (void*)1);
	for (int i = 2; i < 5; i++) {
		TrippleSquareValidatorParams* ta = new TrippleSquareValidatorParams();
		ta->index_for_result = i;
		ta->sudokuRowNum = i - 2;
		pthread_create(&threads[i], NULL, tripple_square_validator, (void*)ta);
	}
	for (int i = 0; i < NUM_THREADS_SECOND_EXAMPLE; i++) {
		pthread_join(threads[i], NULL);
	}
	int sum = 0;
	for (int i = 0; i < 5; i++) {
		sum += result[i];
	}
	return sum == NUM_THREADS_SECOND_EXAMPLE;
}
#pragma endregion Example2

int main()
{
	double min_time = 10.0;
#pragma region Example1
	srand(time(NULL));
	const int row = 100;
	const int column = 1000000;
	matrix = (int**)malloc(row * sizeof(int*));
	for (int i = 0; i < row; i++)
		matrix[i] = (int*)malloc(column * sizeof(int));
	fill_matrix(matrix, row, column);
	min_time = 10.0;
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
		free(matrix[i]);
	free(matrix);
#pragma endregion Example1

#pragma region Example2
	min_time = 10.0;
	bool res;
	for (int i = 0; i < 100; i++) {
		clock_t begin = clock();
		res = is_sudoku_valid();
		clock_t end = clock();
		double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
		if (time_spent < min_time) {
			min_time = time_spent;
		}
	}
	printf("Is valid: %s\n", res ? "true" : "false");
	printf("The elapsed time is %f milliseconds\n", min_time * 1000);
#pragma endregion Example2
}
