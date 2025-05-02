#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define SIZE 9

int sudoku[SIZE][SIZE] = {
    {6,2,4,5,3,9,1,8,7},
    {5,1,9,7,2,8,6,3,4},
    {8,3,7,6,1,4,2,9,5},
    {1,4,3,8,6,5,7,2,9},
    {9,5,8,2,4,7,3,6,1},
    {7,6,2,3,9,1,4,5,8},
    {3,7,1,9,5,6,8,4,2},
    {4,9,6,1,8,2,5,7,3},
    {2,8,5,4,7,3,9,1,6}
};

int results[27]; 

typedef struct {
    int row;
    int col;
    int index; 
} parameters;

void* check_row(void* arg) {
    parameters* p = (parameters*) arg;
    int seen[10] = {0};
    for (int i = 0; i < SIZE; i++) {
        int val = sudoku[p->row][i];
        if (val < 1 || val > 9 || seen[val]) {
            pthread_exit(NULL);
        }
        seen[val] = 1;
    }
    results[p->index] = 1;
    pthread_exit(NULL);
}

void* check_col(void* arg) {
    parameters* p = (parameters*) arg;
    int seen[10] = {0};
    for (int i = 0; i < SIZE; i++) {
        int val = sudoku[i][p->col];
        if (val < 1 || val > 9 || seen[val]) {
            pthread_exit(NULL);
        }
        seen[val] = 1;
    }
    results[p->index] = 1;
    pthread_exit(NULL);
}

void* check_box(void* arg) {
    parameters* p = (parameters*) arg;
    int seen[10] = {0};
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int val = sudoku[p->row + i][p->col + j];
            if (val < 1 || val > 9 || seen[val]) {
                pthread_exit(NULL);
            }
            seen[val] = 1;
        }
    }
    results[p->index] = 1;
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[27];
    int thread_count = 0;

                // Checking rows
    for (int i = 0; i < SIZE; i++) {
        parameters* data = (parameters*) malloc(sizeof(parameters));
        data->row = i;
        data->col = 0;
        data->index = i;
        pthread_create(&threads[thread_count++], NULL, check_row, data);
    }

                // Checking columns
    for (int i = 0; i < SIZE; i++) {
        parameters* data = (parameters*) malloc(sizeof(parameters));
        data->row = 0;
        data->col = i;
        data->index = 9 + i;
        pthread_create(&threads[thread_count++], NULL, check_col, data);
    }

                // Checking 3x3 boxes
    int box_index = 18;
    for (int i = 0; i < SIZE; i += 3) {
        for (int j = 0; j < SIZE; j += 3) {
            parameters* data = (parameters*) malloc(sizeof(parameters));
            data->row = i;
            data->col = j;
            data->index = box_index++;
            pthread_create(&threads[thread_count++], NULL, check_box, data);
        }
    }

                // Join threads
    for (int i = 0; i < 27; i++) {
        pthread_join(threads[i], NULL);
    }

    
    for (int i = 0; i < 27; i++) {
        if (results[i] != 1) {
            printf("Sudoku solution is invalid.\n");
            return 0;
        }
    }

    printf("Sudoku solution is valid.\n");
    return 0;
}
