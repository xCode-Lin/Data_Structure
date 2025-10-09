#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define SIZE 100
#define MAX_VALUE 1000

typedef struct Node {
    int data;
    struct Node* left;
    struct Node* right;
} Node;

Node* createNode(int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    newNode->data = value;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

Node* insertNode(Node* root, int value) {
    if (root == NULL) {
        return createNode(value);
    }
    if (value < root->data) {
        root->left = insertNode(root->left, value);
    }
    else if (value > root->data) {
        root->right = insertNode(root->right, value);
    }
    return root;
}

void freeBST(Node* root) {
    if (!root) return;
    freeBST(root->left);
    freeBST(root->right);
    free(root);
}

int searchBST(Node* root, int target) {
    int count = 0;
    while (root != NULL) {
        count++;
        printf("  노드 방문 #%d → %d\n", count, root->data);

        if (target == root->data) {
            printf("  → 찾음!\n");
            return 1;
        }
        else if (target < root->data)
            root = root->left;
        else
            root = root->right;
    }
    printf("  → 트리에 없음\n");
    return 0;
}

int linearSearch(int arr[], int size, int target) {
    int count = 0;
    for (int i = 0; i < size; i++) {
        count++;
        printf("  인덱스 %d → 값 %d\n", i, arr[i]);
        if (arr[i] == target) {
            printf("  → 찾음!\n");
            return 1;
        }
    }
    return 0;
}

int main() {
    int arr[SIZE];
    Node* root = NULL;
    int i;

    srand(time(NULL));

    for (i = 0; i < SIZE; i++) {
        arr[i] = rand() % (MAX_VALUE + 1);
    }

    for (i = 0; i < SIZE; i++) {
        root = insertNode(root, arr[i]);
    }

    printf("BST construction complete with 100 random numbers.\n");

    for (i = 0; i < SIZE; i++) {
        printf("%d ", arr[i]);
    }

    int target = arr[rand() % SIZE];
    printf("\n\n찾을 숫자: %d\n\n", target);

    LARGE_INTEGER freq, start, end;
    double linearTime, bstTime;
    QueryPerformanceFrequency(&freq);

    printf("===== 선형 탐색 과정 =====\n");
    QueryPerformanceCounter(&start);
    int found1 = linearSearch(arr, SIZE, target);
    QueryPerformanceCounter(&end);
    linearTime = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart;

    printf("\n===== BST 탐색 과정 =====\n");
    QueryPerformanceCounter(&start);
    int found2 = searchBST(root, target);
    QueryPerformanceCounter(&end);
    bstTime = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart;

    printf("\n===== 탐색 결과 =====\n");
    printf("선형 탐색: %s (탐색 시간: %.9f초)\n", found1 ? "성공" : "실패", linearTime);
    printf("BST 탐색: %s (탐색 시간: %.9f초)\n", found2 ? "성공" : "실패", bstTime);

    freeBST(root);

    return 0;
}
