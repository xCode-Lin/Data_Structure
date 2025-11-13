#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 10000
#define RANGE 1000000
#define TRIALS 100

long long insertion_sort(int arr[], int n) {
    long long comparisons = 0;

    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;

        while (j >= 0) {
            comparisons++;
            if (arr[j] > key) {
                arr[j + 1] = arr[j];
                j--;
            }
            else {
                break;
            }
        }
        arr[j + 1] = key;
    }

    return comparisons;
}

long long shell_sort(int arr[], int n) {
    long long comparisons = 0;

    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j = i;

            while (j >= gap) {
                comparisons++;
                if (arr[j - gap] > temp) {
                    arr[j] = arr[j - gap];
                    j -= gap;
                }
                else {
                    break;
                }
            }
            arr[j] = temp;
        }
    }

    return comparisons;
}

int main(void) {
    int* data = (int*)malloc(sizeof(int) * SIZE);
    int* copy = (int*)malloc(sizeof(int) * SIZE);

    if (data == NULL || copy == NULL) {
        fprintf(stderr, "메모리 할당 실패\n");
        return 1;
    }

    srand((unsigned int)time(NULL));
    long long total_insert_comparisons = 0;
    long long total_shell_comparisons = 0;

    for (int t = 0; t < TRIALS; t++) {
        for (int i = 0; i < SIZE; i++) {
            data[i] = rand() % (RANGE + 1);
        }

        for (int i = 0; i < SIZE; i++) {
            copy[i] = data[i];
        }
        total_insert_comparisons += insertion_sort(copy, SIZE);

        for (int i = 0; i < SIZE; i++) {
            copy[i] = data[i];
        }
        total_shell_comparisons += shell_sort(copy, SIZE);
    }

    double avg_insert = (double)total_insert_comparisons / TRIALS;
    double avg_shell = (double)total_shell_comparisons / TRIALS;

    printf("=== 정렬 비교 결과 (100회 평균) ===\n");
    printf("단순 삽입 정렬 평균 비교 횟수 : %.2f\n", avg_insert);
    printf("셸 정렬 평균 비교 횟수        : %.2f\n", avg_shell);

    free(data);
    free(copy);
    return 0;
}
