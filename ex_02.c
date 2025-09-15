#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Node {
    char data;
    struct Node* a;
    struct Node* b;
} Node;

Node* createNode(char data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->a = NULL;
    newNode->b = NULL;
    return newNode;
}

Node* parseTree(char* str, int* index) {
    while (str[*index] == ' ') (*index)++;
    if (str[*index] == '\0' || str[*index] == ')') return NULL;

    if (str[*index] == '(') {
        (*index)++;
        while (str[*index] == ' ') (*index)++;

        if (!isalpha(str[*index])) return NULL;
        Node* root = createNode(str[*index]);
        (*index)++;

        Node* prev = NULL;
        while (str[*index] != ')' && str[*index] != '\0') {
            Node* child = parseTree(str, index);
            if (child) {
                if (!root->a) root->a = child;
                else prev->b = child;
                prev = child;
            }
            while (str[*index] == ' ') (*index)++;
        }

        if (str[*index] == ')') (*index)++;
        return root;
    }

    if (isalpha(str[*index])) {
        Node* node = createNode(str[*index]);
        (*index)++;
        return node;
    }

    return NULL;
}

int getHeight(Node* root) {
    if (!root) return 0;
    int maxChildHeight = -1;
    Node* child = root->a;
    while (child) {
        int h = getHeight(child);
        if (h > maxChildHeight) maxChildHeight = h;
        child = child->b;
    }
    return maxChildHeight + 1;
}

int getNodeCount(Node* root) {
    if (!root) return 0;
    int count = 1;
    Node* child = root->a;
    while (child) {
        count += getNodeCount(child);
        child = child->b;
    }
    return count;
}

int getLeafCount(Node* root) {
    if (!root) return -1;
    if (!root->a) return 0;
    int count = 1;
    Node* child = root->a;
    while (child) {
        count += getLeafCount(child);
        child = child->b;
    }
    return count;
}

int main() {
    char input[1000];
    scanf("%[^\n]", input);

    int index = 0;
    Node* root = parseTree(input, &index);

    if (!root) {
        printf("ERROR\n");
        return 0;
    }

    int height = getHeight(root);
    int nodeCount = getNodeCount(root);
    int leafCount = getLeafCount(root);

    printf("%d, %d, %d\n", height, nodeCount, leafCount);

    return 0;
}
