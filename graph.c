#include "graph.h"

#include "vertices.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

//This section of code was taken from the assignment pdf.
struct Graph {
    uint32_t vertices; //Number of vertices.
    bool undirected; //Undirected graph?
    bool visited[VERTICES]; //Where have we gone?
    uint32_t matrix[VERTICES][VERTICES]; //Adjacency matrix.
};

//This function is responsible for allocating the memory for a graph. This code was provided by the assignment pdf.
Graph *graph_create(uint32_t vertices, bool undirected) {
    Graph *G = (Graph *) calloc(1, sizeof(Graph));
    G->vertices = vertices;
    G->undirected = undirected;
    return G;
}

//This function is responmsible for freeing up the memory allocated in a graph. This code was provided by the assignment pdf.
void graph_delete(Graph **G) {
    free(*G);
    *G = NULL;
    return;
}

//This function returns the number of vertices in a graph.
uint32_t graph_vertices(Graph *G) {
    return G->vertices;
}

//This function adds an edge between vertices i and j by adding a weight value in an adjancency matrix ad (i, j). Returns true if it is succesful and reutrns false otherwise.
bool graph_add_edge(Graph *G, uint32_t i, uint32_t j, uint32_t k) {
    if (i < G->vertices && j < G->vertices) {
        G->matrix[i][j] = k;
        if (G->undirected) {
            G->matrix[j][i] = k;
        }
        return true;
    }
    return false;
}

//This function checks if the two inputed vertices have an edge by checking if they have a weight in an adjacency matrix at the point (i, j). Returns true if it does and returns false otherwise.
bool graph_has_edge(Graph *G, uint32_t i, uint32_t j) {
    if (i < G->vertices && j < G->vertices && G->matrix[i][j] > 0) {
        return true;
    }
    return false;
}

//This function returns the weight inbetween the vertices i and j if it is within bounds. Returns 0 otherwise.
uint32_t graph_edge_weight(Graph *G, uint32_t i, uint32_t j) {
    if (i < G->vertices && j < G->vertices) {
        return G->matrix[i][j];
    }
    return 0;
}

//This functionr eturns if the vertex is visited.
bool graph_visited(Graph *G, uint32_t v) {
    return G->visited[v];
}

//This manipulates a vertex by marking it as visited if its within bounds.
void graph_mark_visited(Graph *G, uint32_t v) {
    if (v < G->vertices) {
        G->visited[v] = true;
    }
}

//This manipulates a vertex by marking it as unvisited if it within bounds.
void graph_mark_unvisited(Graph *G, uint32_t v) {
    if (v < G->vertices) {
        G->visited[v] = false;
    }
}

