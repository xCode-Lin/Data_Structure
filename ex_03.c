#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define MAX_SIZE 100

char tree[MAX_SIZE];
int parentStack[MAX_SIZE];
int isLeftStack[MAX_SIZE];
int parseTop = -1;

void push(int parent, int isLeft) {
    parseTop++;
    parentStack[parseTop] = parent;
    isLeftStack[parseTop] = isLeft;
}

void pop() {
    if (parseTop >= 0) parseTop--;
}

void parseTree(const char* str) {
    for (int i = 0; i < MAX_SIZE; i++)
        tree[i] = '\0';

    int n = strlen(str);
    int nodeIndex = 0;
    int rootSet = 0;

    for (int i = 0; i < n; i++) {
        char c = str[i];
        if (isspace(c)) continue;

        if (c == '(') {
            if (nodeIndex > 0)
                push(nodeIndex, 1);
        }

        else if (c == ')') {
            pop();
        }
        
        else if (isalpha(c)) {
            if (!rootSet) {
                tree[1] = c;
                nodeIndex = 1;
                rootSet = 1;
            }

            else {
                int parent = parentStack[parseTop];
                int isLeft = isLeftStack[parseTop];
                int childIdx = isLeft ? parent * 2 : parent * 2 + 1;

                tree[childIdx] = c;
                nodeIndex = childIdx;

                if (isLeftStack[parseTop] == 1)
                    isLeftStack[parseTop] = 0;
            }
        }
    }
}

void iterativePreorder() {
    int stack[MAX_SIZE];
    int top = -1;

    if (tree[1] == '\0') return;

    stack[++top] = 1;

    while (top != -1) {
        int index = stack[top--];
        if (tree[index] == '\0') continue;

        printf("%c ", tree[index]);

        if (2 * index + 1 < MAX_SIZE && tree[2 * index + 1] != '\0') {
            stack[++top] = 2 * index + 1;
        }
        if (2 * index < MAX_SIZE && tree[2 * index] != '\0') {
            stack[++top] = 2 * index;
        }
    }
}

void iterativeInorder() {
    int stack[MAX_SIZE];
    int top = -1;
    int index = 1;

    while (top != -1 || (index < MAX_SIZE && tree[index] != '\0')) {
        while (index < MAX_SIZE && tree[index] != '\0') {
            stack[++top] = index;
            index = index * 2;
        }

        if (top != -1) {
            index = stack[top--];
            printf("%c ", tree[index]);
            index = index * 2 + 1;
        }
    }
}

void iterativePostorder() {
    int stack[MAX_SIZE];
    int top = -1;
    int index = 1;
    int lastVisited = 0;

    while (top != -1 || (index < MAX_SIZE && tree[index] != '\0')) {
        while (index < MAX_SIZE && tree[index] != '\0') {
            stack[++top] = index;
            index = index * 2;
        }

        int peek = stack[top];
        int right = peek * 2 + 1;

        if (right >= MAX_SIZE || tree[right] == '\0' || lastVisited == right) {
            printf("%c ", tree[peek]);
            lastVisited = peek;
            top--;
        }

        else index = right;
    }
}

int main() {
    char input[500];

    scanf("%[^\n]", input);

    parseTree(input);

    printf("pre-order: ");
    iterativePreorder();

    printf("\nin-order: ");
    iterativeInorder();

    printf("\npost-order: ");
    iterativePostorder();

    printf("\n");

    return 0;
}
