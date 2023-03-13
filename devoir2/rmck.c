#include "headers/rmck.h"

typedef struct node {
    struct node *next;
    int val;
} node;

typedef struct {
    node *start;
    node *end;
    int n;
} Queue;



Queue *create_queue() {
    Queue *q = malloc(sizeof(Queue));
    q->n = 0;
    q->start = q->end = NULL;
    return q;
}

void delete_queue(Queue *q) {
    node *current = q->start;
    for (int i = 0; i < q->n; i++) {
        node *prev = current;
        current = current->next;
        free(prev);
    }
    free(q);
}

void push_queue(Queue *q, int val) {
    q->n++;
    if (q->start == NULL) {
        q->start = q->end = malloc(sizeof(node));
        q->start->next = NULL;
        q->start->val = val;
        return;
    }
    q->end->next = malloc(sizeof(node));
    q->end = q->end->next;
    q->end->val = val;
    q->end->next = NULL;
}

int pop_queue(Queue *q) {
    q->n--;
    int val = q->start->val;
    node *prev = q->start;
    q->start = q->start->next;
    free(prev);
    return val;
}

vector **g;

int compare_adjency_list(const void * g1, const void * g2) {
    int idx1 = *((int*) g1); int idx2 = *((int*) g2);
    return g[idx1]->n - g[idx2]->n;
}

int compare_int(const void *int1, const void *int2) {
    int n1 = *((const int*) int1); int n2 = *((const int*) int2);    
    return n1 - n2;
}

void rmck(int *perm, Triplet *triplets, int n_triplets, int n_nodes) {
    int max_coords = 2 * n_nodes;
    
    // Create adjency lists
    g = malloc(max_coords * sizeof(vector));
    for (int i = 0; i < max_coords; i++) {
        g[i] = create_vector();
    }

    for (int i = 0; i < n_triplets; i++) {
        push_vector(g[triplets[i].i], triplets[i].j);
    }

    int *sortedDegrees = malloc(max_coords * sizeof(int));
    for (int i = 0; i < max_coords; i++) {
        qsort(g[i]->array, g[i]->n, sizeof(int), compare_int);
        sortedDegrees[i] = i;
    }
    qsort(sortedDegrees, max_coords, sizeof(int), compare_adjency_list);

    char *visited = malloc(max_coords);
    memset(visited, 0, max_coords);

    int RIdx = max_coords - 1;

    for (int i = 0; i < max_coords; i++) {
        int x = sortedDegrees[i];
        if (visited[x]) {
            continue;
        }
        
        Queue *q = create_queue();
        push_queue(q, x);
        while (q->n) {
            int x = pop_queue(q);
            if (visited[x]) {
                continue;
            }
            perm[RIdx--] = x;
            visited[x] = 1;
            for (int i = 0; i < g[x]->n; i++) {
                if (visited[get_vector(g[x], i)]) continue;
                push_queue(q, get_vector(g[x], i));
            }
        }
        delete_queue(q);
    }

    for (int i = 0; i < max_coords; i++) {
        delete_vector(g[i]);
    }
    free(sortedDegrees);
    free(g);
    free(visited);
}