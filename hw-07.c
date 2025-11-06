#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N_VERTICES 10
#define N_EDGES    20
#define INF 1000000000

void init_graph(int adj[N_VERTICES][N_VERTICES]) {
    for (int i = 0; i < N_VERTICES; i++) {
        for (int j = 0; j < N_VERTICES; j++) {
            if (i == j) adj[i][j] = 0;
            else adj[i][j] = INF;
        }
    }
}

void add_random_edges(int adj[N_VERTICES][N_VERTICES]) {
    int count = 0;
    while (count < N_EDGES) {
        int u = rand() % N_VERTICES;
        int v = rand() % N_VERTICES;

        if (u == v || adj[u][v] != INF)
            continue;

        int weight = (rand() % 20) + 1;
        adj[u][v] = weight;
        adj[v][u] = weight;
        count++;
    }

    printf("랜덤 간선 %d개가 추가되었습니다.\n", N_EDGES);
}

void floyd_warshall(int adj[N_VERTICES][N_VERTICES],
    int dist[N_VERTICES][N_VERTICES],
    int next[N_VERTICES][N_VERTICES]) {
    for (int i = 0; i < N_VERTICES; i++) {
        for (int j = 0; j < N_VERTICES; j++) {
            dist[i][j] = adj[i][j];
            if (adj[i][j] != INF && i != j)
                next[i][j] = j;
            else
                next[i][j] = -1;
        }
    }

    for (int k = 0; k < N_VERTICES; k++) {
        for (int i = 0; i < N_VERTICES; i++) {
            for (int j = 0; j < N_VERTICES; j++) {
                if (dist[i][k] == INF || dist[k][j] == INF)
                    continue;

                if (dist[i][j] > dist[i][k] + dist[k][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    next[i][j] = next[i][k];
                }
            }
        }
    }
}

void print_path(int u, int v, int next[N_VERTICES][N_VERTICES]) {
    if (next[u][v] == -1) {
        printf(" (경로 없음)");
        return;
    }

    printf("%d", u);
    while (u != v) {
        u = next[u][v];
        printf(" -> %d", u);
    }
}

void print_all_pairs_paths(int dist[N_VERTICES][N_VERTICES],
    int next[N_VERTICES][N_VERTICES]) {
    printf("\n===== 모든 노드 쌍의 최단경로 =====\n");
    for (int i = 0; i < N_VERTICES; i++) {
        for (int j = i + 1; j < N_VERTICES; j++) {
            printf("\n[%d → %d]\n", i, j);
            if (dist[i][j] == INF) {
                printf("경로 없음\n");
                continue;
            }
            printf("최단 거리: %d\n", dist[i][j]);
            printf("경로: ");
            print_path(i, j, next);
            printf("\n");
        }
    }
}

int main(void) {
    int adj[N_VERTICES][N_VERTICES];
    int dist[N_VERTICES][N_VERTICES];
    int next[N_VERTICES][N_VERTICES];

    srand((unsigned)time(NULL));

    init_graph(adj);
    add_random_edges(adj);
    floyd_warshall(adj, dist, next);
    print_all_pairs_paths(dist, next);

    return 0;
}
