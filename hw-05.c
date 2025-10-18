#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 1000
#define MAX_VALUE 10000

void generate_random_unique(int arr[], int size, int min, int max) {
    int range = max - min + 1;
    if (range < size) {
        printf("범위가 너무 작습니다. (중복 없는 %d개의 난수를 만들 수 없음)\n", size);
        exit(1);
    }

    int* used = (int*)calloc(range, sizeof(int));
    if (used == NULL) {
        printf("메모리 할당 실패\n");
        exit(1);
    }

    int count = 0;
    while (count < size) {
        int num = rand() % range + min;
        if (!used[num - min]) {
            used[num - min] = 1;
            arr[count++] = num;
        }
    }

    free(used);
}

void generate_sorted_ascending(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = i;
    }
}

void generate_sorted_descending(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = size - 1 - i;
    }
}

void generate_custom_pattern(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = i * (i % 2 + 2);
    }
}

double test_array_search(int data[], int randoms[], int size) {
    long long total_comparisons = 0;

    for (int i = 0; i < size; i++) {
        int target = randoms[i];
        int comparisons = 0;
        int found = 0;

        for (int j = 0; j < size; j++) {
            comparisons++;
            if (data[j] == target) {
                found = 1;
                break;
            }
        }

        total_comparisons += comparisons;
    }

    return (double)total_comparisons / size;
}

typedef struct Node {
    int key;
    struct Node* left;
    struct Node* right;
} Node;

Node* createNode(int key) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    newNode->key = key;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

Node* insertBST(Node* root, int key) {
    if (root == NULL) return createNode(key);
    if (key < root->key)
        root->left = insertBST(root->left, key);
    else if (key > root->key)
        root->right = insertBST(root->right, key);
    return root;
}

int searchBST(Node* root, int key) {
    int comparisons = 0;
    Node* current = root;

    while (current != NULL) {
        comparisons++;
        if (key == current->key)
            break;
        else if (key < current->key)
            current = current->left;
        else
            current = current->right;
    }

    return comparisons;
}

void freeBST(Node* root) {
    if (root == NULL) return;
    freeBST(root->left);
    freeBST(root->right);
    free(root);
}

double test_bst_search(int data[], int randoms[], int size) {
    Node* root = NULL;

    for (int i = 0; i < size; i++) {
        root = insertBST(root, data[i]);
    }

    long long total_comparisons = 0;
    for (int i = 0; i < size; i++) {
        total_comparisons += searchBST(root, randoms[i]);
    }

    freeBST(root);

    return (double)total_comparisons / size;
}

typedef struct AVLNode {
    int key;
    struct AVLNode* left;
    struct AVLNode* right;
    int height;
} AVLNode;

int height(AVLNode* n) {
    return n ? n->height : 0;
}

int maxValue(int a, int b) {
    return (a > b) ? a : b;
}

AVLNode* createAVLNode(int key) {
    AVLNode* node = (AVLNode*)malloc(sizeof(AVLNode));
    if (node == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    node->key = key;
    node->left = node->right = NULL;
    node->height = 1;
    return node;
}

AVLNode* rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = maxValue(height(y->left), height(y->right)) + 1;
    x->height = maxValue(height(x->left), height(x->right)) + 1;

    return x;
}

AVLNode* leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = maxValue(height(x->left), height(x->right)) + 1;
    y->height = maxValue(height(y->left), height(y->right)) + 1;

    return y;
}

int getBalance(AVLNode* n) {
    return n ? height(n->left) - height(n->right) : 0;
}

AVLNode* insertAVL(AVLNode* node, int key) {
    if (node == NULL)
        return createAVLNode(key);

    if (key < node->key)
        node->left = insertAVL(node->left, key);
    else if (key > node->key)
        node->right = insertAVL(node->right, key);
    else
        return node;

    node->height = 1 + maxValue(height(node->left), height(node->right));

    int balance = getBalance(node);

    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

int searchAVL(AVLNode* root, int key) {
    int comparisons = 0;
    AVLNode* current = root;

    while (current != NULL) {
        comparisons++;
        if (key == current->key)
            break;
        else if (key < current->key)
            current = current->left;
        else
            current = current->right;
    }

    return comparisons;
}

void freeAVL(AVLNode* root) {
    if (root == NULL) return;
    freeAVL(root->left);
    freeAVL(root->right);
    free(root);
}

double test_avl_search(int data[], int randoms[], int size) {
    AVLNode* root = NULL;

    for (int i = 0; i < size; i++) {
        root = insertAVL(root, data[i]);
    }

    long long total_comparisons = 0;
    for (int i = 0; i < size; i++) {
        total_comparisons += searchAVL(root, randoms[i]);
    }

    freeAVL(root);

    return (double)total_comparisons / size;
}

void print_search_results(int data[], int randoms[], int size, int data_num) {
    double avg_array = test_array_search(data, randoms, size);
    double avg_bst = test_bst_search(data, randoms, size);
    double avg_avl = test_avl_search(data, randoms, size);

    printf("Array: 데이터 (%d)에서 평균 %.2f회 탐색\n", data_num, avg_array);
    printf("BST:   데이터 (%d)에서 평균 %.2f회 탐색\n", data_num, avg_bst);
    printf("AVL:   데이터 (%d)에서 평균 %.2f회 탐색\n\n", data_num, avg_avl);
}

int main() {
    srand(time(NULL));

    int data1[SIZE], data2[SIZE], data3[SIZE], data4[SIZE];
    int randoms[SIZE];

    generate_random_unique(data1, SIZE, 0, 9999);
    generate_sorted_ascending(data2, SIZE);
    generate_sorted_descending(data3, SIZE);
    generate_custom_pattern(data4, SIZE);

    generate_random_unique(randoms, SIZE, 0, MAX_VALUE);

    print_search_results(data1, randoms, SIZE, 1);
    print_search_results(data2, randoms, SIZE, 2);
    print_search_results(data3, randoms, SIZE, 3);
    print_search_results(data4, randoms, SIZE, 4);

    return 0;
}
