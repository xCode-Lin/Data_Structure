#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#define MAX 99999

char input[MAX];
int k = 0;

void skipSpaces() {
    while (input[k] && isspace((char)input[k])) k++;
}

char peek() {
    skipSpaces();
    return input[k];
}

char getCharSkip() {
    char c = peek();
    if (c) k++;
    return c;
}

typedef struct Node {
    char data;
    struct Node* left, * right;
} Node;

Node* makeNode(char ch) {
    Node* n = (Node*)malloc(sizeof(Node));
    n->data = ch;
    n->left = n->right = NULL;
    return n;
}


Node* parseTree(int* error) {
    if (peek() != '(') { *error = 1; return NULL; }
    getCharSkip();

    if (peek() == ')') { getCharSkip(); return NULL; }

    char c = peek();
    if (!isupper((char)c)) { *error = 1; return NULL; }
    char label = getCharSkip();
    Node* node = makeNode(label);

    int aCount = 0;
    while (1) {
        skipSpaces();
        char p = peek();
        if (p == '(') {
            Node* a = parseTree(error);
            if (*error == 1) return NULL;
            aCount++;
            if (aCount == 1) node->left = a;
            else if (aCount == 2) node->right = a;
            if (aCount > 2 && *error != 1) *error = 2;
        }
        else if (isupper((unsigned char)p)) {
            char ch = getCharSkip();
            Node* a = makeNode(ch);
            aCount++;
            if (aCount == 1) node->left = a;
            else if (aCount == 2) node->right = a;
            if (aCount > 2 && *error != 1) *error = 2;
        }
        else {
            break;
        }
    }

    skipSpaces();
    if (peek() != ')') { *error = 1; return node; }
    getCharSkip();
    return node;
}

int main() {
    if (!fgets(input, MAX, stdin)) {
        printf("ERROR\n");
        return 0;
    }

    k = 0;
    int error = 0;
    Node* root = parseTree(&error);


    skipSpaces();
    if (error == 1 || root == NULL) {
        printf("ERROR\n");
        return 0;
    }
    if (peek() != '\0' && peek() != '\n') {
        printf("ERROR\n");
        return 0;
    }

    if (error == 2) printf("FALSE\n");
    else printf("TRUE\n");

    return 0;
}
