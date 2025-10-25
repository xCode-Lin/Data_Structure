#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <stddef.h>

#define NUM_VERTICES 100
#define SPARSE_M 100
#define DENSE_M 4000

typedef struct {
    int src;
    int dest;
} Edge;

typedef struct {
    double build_time;
    size_t memory_bytes;
    long insert_delete_comparisons;
    long connectivity_comparisons;
    long adjacency_print_comparisons;
} TestResult;

typedef struct GraphMatrix {
    int n;
    int** matrix;
} GraphMatrix;

typedef struct GraphList {
    int n;
    struct Node** adj;
} GraphList;

typedef struct Node {
    int vertex;
    struct Node* next;
} Node;

GraphMatrix* gm_create(int n) {
    GraphMatrix* g = (GraphMatrix*)malloc(sizeof(GraphMatrix));
    if (!g) {
        perror("malloc GraphMatrix");
        return NULL;
    }

    g->n = n;
    g->matrix = (int**)malloc(n * sizeof(int*));
    if (!g->matrix) {
        perror("malloc matrix rows");
        free(g);
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        g->matrix[i] = (int*)calloc(n, sizeof(int));
        if (!g->matrix[i]) {
            perror("calloc matrix[i]");
            for (int j = 0; j < i; j++)
                free(g->matrix[j]);
            free(g->matrix);
            free(g);
            return NULL;
        }
    }

    return g;
}

size_t gm_memory_bytes(GraphMatrix* g) {
    if (!g) return 0;
    size_t bytes = sizeof(GraphMatrix);
    bytes += sizeof(int*) * g->n;
    bytes += sizeof(int) * g->n * g->n;
    return bytes;
}

void gm_free(GraphMatrix* g) {
    if (!g) return;

    for (int i = 0; i < g->n; i++) {
        free(g->matrix[i]);
    }

    free(g->matrix);
    free(g);
}

void gm_build_from_edges(GraphMatrix* g, Edge* edges, int m) {
    if (!g || !edges) return;

    for (int i = 0; i < m; i++) {
        int u = edges[i].src;
        int v = edges[i].dest;

        if (u >= 0 && u < g->n && v >= 0 && v < g->n) {
            g->matrix[u][v] = 1;
            g->matrix[v][u] = 1;
        }
    }
}

void gm_run_benchmarks(GraphMatrix* g, Edge* edges, int m, TestResult* res) {
    if (!g || !res) return;

    res->memory_bytes = gm_memory_bytes(g);

    long insert_delete_comp = 0;
    long connectivity_comp = 0;
    long adjacency_print_comp = 0;

    for (int i = 0; i < m; i++) {
        int u = edges[i].src;
        int v = edges[i].dest;

        if (u >= 0 && v >= 0 && u < g->n && v < g->n) {
            insert_delete_comp++;
            if (g->matrix[u][v] == 0) {
                g->matrix[u][v] = 1;
                g->matrix[v][u] = 1;
            }
            else {
                g->matrix[u][v] = 0;
                g->matrix[v][u] = 0;
            }
        }
    }

    for (int i = 0; i < m; i++) {
        int u = rand() % g->n;
        int v = rand() % g->n;
        connectivity_comp++;
        volatile int connected = g->matrix[u][v];
        (void)connected;
    }

    for (int u = 0; u < g->n; u++) {
        for (int v = 0; v < g->n; v++) {
            adjacency_print_comp++;
            if (g->matrix[u][v]) {
                volatile int neighbor = v;
                (void)neighbor;
            }
        }
    }

    res->insert_delete_comparisons = insert_delete_comp;
    res->connectivity_comparisons = connectivity_comp;
    res->adjacency_print_comparisons = adjacency_print_comp;
}

GraphList* gl_create(int n) {
    GraphList* g = (GraphList*)malloc(sizeof(GraphList));
    if (!g) {
        perror("malloc GraphList");
        return NULL;
    }

    g->n = n;
    g->adj = (Node**)malloc(n * sizeof(Node*));
    if (!g->adj) {
        perror("malloc adj list");
        free(g);
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        g->adj[i] = NULL;
    }

    return g;
}

size_t gl_memory_bytes(GraphList* g) {
    if (!g) return 0;
    size_t total = sizeof(GraphList);
    total += sizeof(Node*) * g->n;
    for (int i = 0; i < g->n; i++) {
        Node* curr = g->adj[i];
        while (curr) {
            total += sizeof(Node);
            curr = curr->next;
        }
    }
    return total;
}

void gl_free(GraphList* g) {
    if (!g) return;

    for (int i = 0; i < g->n; i++) {
        Node* curr = g->adj[i];
        while (curr) {
            Node* next = curr->next;
            free(curr);
            curr = next;
        }
    }
    
    free(g->adj);
    free(g);
}

void gl_build_from_edges(GraphList* g, Edge* edges, int m) {
    if (!g || !edges) return;

    for (int i = 0; i < m; i++) {
        int u = edges[i].src;
        int v = edges[i].dest;

        if (u >= 0 && v >= 0 && u < g->n && v < g->n) {
            Node* newNode1 = (Node*)malloc(sizeof(Node));
            newNode1->vertex = v;
            newNode1->next = g->adj[u];
            g->adj[u] = newNode1;

            if (u != v) {
                Node* newNode2 = malloc(sizeof(Node));
                newNode2->vertex = u;
                newNode2->next = g->adj[v];
                g->adj[v] = newNode2;
            }
        }
    }
}

void gl_run_benchmarks(GraphList* g, Edge* edges, int m, TestResult* res) {
    clock_t start, end;

    start = clock();
    gl_build_from_edges(g, edges, m);
    end = clock();

    res->build_time = (double)(end - start) / CLOCKS_PER_SEC;
    res->memory_bytes = gl_memory_bytes(g);
}

static Edge* generate_random_unique_edges(int n, int m) {
    if (m > n * (n - 1) / 2) return NULL;
    bool* seen = calloc(n * n, sizeof(bool));
    if (!seen) { perror("calloc"); return NULL; }

    Edge* edges = malloc(sizeof(Edge) * m);
    if (!edges) { perror("malloc"); free(seen); return NULL; }

    int cnt = 0;
    while (cnt < m) {
        int u = rand() % n;
        int v = rand() % n;
        if (u == v) continue;

        int a = u < v ? u : v;
        int b = u < v ? v : u;
        if (seen[a * n + b]) continue;
        seen[a * n + b] = true;
        edges[cnt].src = a;
        edges[cnt].dest = b;
        cnt++;
    }

    free(seen);
    return edges;
}

int main(void) {
    srand((unsigned)time(NULL));

    const int n = NUM_VERTICES;

    Edge* sparse_edges = generate_random_unique_edges(n, SPARSE_M);
    if (!sparse_edges) {
        fprintf(stderr, "Failed to generate sparse edges\n");
        return 1;
    }
    Edge* dense_edges = generate_random_unique_edges(n, DENSE_M);
    if (!dense_edges) {
        fprintf(stderr, "Failed to generate dense edges\n");
        free(sparse_edges);
        return 1;
    }

    TestResult res;

    {
        GraphMatrix* gm = gm_create(n);
        if (!gm) { fprintf(stderr, "gm_create failed\n"); goto cleanup; }
        gm_build_from_edges(gm, sparse_edges, SPARSE_M);
        gm_run_benchmarks(gm, sparse_edges, SPARSE_M, &res);

        printf("케이스 1:\n");
        printf("메모리 %zu Bytes\n", res.memory_bytes);
        printf("간선 삽입/삭제 비교 %ld번\n", res.insert_delete_comparisons);
        printf("두 정점의 연결 확인 비교 %ld번\n", res.connectivity_comparisons);
        printf("한 노드의 인접 노드 출력 비교 %ld번\n\n", res.adjacency_print_comparisons);

        gm_free(gm);
    }

    {
        GraphList* gl = gl_create(n);
        if (!gl) { fprintf(stderr, "gl_create failed\n"); goto cleanup; }
        gl_build_from_edges(gl, sparse_edges, SPARSE_M);
        gl_run_benchmarks(gl, sparse_edges, SPARSE_M, &res);

        printf("케이스 2:\n");
        printf("메모리 %zu Bytes\n", res.memory_bytes);
        printf("간선 삽입/삭제 비교 %ld번\n", res.insert_delete_comparisons);
        printf("두 정점의 연결 확인 비교 %ld번\n", res.connectivity_comparisons);
        printf("한 노드의 인접 노드 출력 비교 %ld번\n\n", res.adjacency_print_comparisons);

        gl_free(gl);
    }

    {
        GraphMatrix* gm = gm_create(n);
        if (!gm) { fprintf(stderr, "gm_create failed\n"); goto cleanup; }
        gm_build_from_edges(gm, dense_edges, DENSE_M);
        gm_run_benchmarks(gm, dense_edges, DENSE_M, &res);

        printf("케이스 3:\n");
        printf("메모리 %zu Bytes\n", res.memory_bytes);
        printf("간선 삽입/삭제 비교 %ld번\n", res.insert_delete_comparisons);
        printf("두 정점의 연결 확인 비교 %ld번\n", res.connectivity_comparisons);
        printf("한 노드의 인접 노드 출력 비교 %ld번\n\n", res.adjacency_print_comparisons);

        gm_free(gm);
    }

    {
        GraphList* gl = gl_create(n);
        if (!gl) { fprintf(stderr, "gl_create failed\n"); goto cleanup; }
        gl_build_from_edges(gl, dense_edges, DENSE_M);
        gl_run_benchmarks(gl, dense_edges, DENSE_M, &res);

        printf("케이스 4:\n");
        printf("메모리 %zu Bytes\n", res.memory_bytes);
        printf("간선 삽입/삭제 비교 %ld번\n", res.insert_delete_comparisons);
        printf("두 정점의 연결 확인 비교 %ld번\n", res.connectivity_comparisons);
        printf("한 노드의 인접 노드 출력 비교 %ld번\n\n", res.adjacency_print_comparisons);

        gl_free(gl);
    }

cleanup:
    free(sparse_edges);
    free(dense_edges);
    return 0;
}
