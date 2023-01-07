#include "path.h"

#include "graph.h"
#include "stack.h"
#include "vertices.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

//Struct path provided by the assignment pdf.
struct Path {
    Stack *vertices;
    uint32_t length;
};

//This function is resonsible for allocating the memory for a path. Modeled closely like the stack create function.
Path *path_create(void) {
    Path *p = (Path *) malloc(sizeof(Path));
    if (p) {
        p->length = 0;
        p->vertices = stack_create(VERTICES);
    }
    return p;
}

//This function is responsible for freeing the memory.
void path_delete(Path **p) {
    stack_delete(&(*p)->vertices);
    free(*p);
    *p = NULL;
}

//This function pushes a vertex on to path by pushing a vertices onto stack and adding to the length by the weight between to vertices if the stack isn't empty.
bool path_push_vertex(Path *p, uint32_t v, Graph *G) {
    uint32_t top = START_VERTEX;
    if (!stack_full(p->vertices)) {
        stack_peek(p->vertices, &top);
        if (!stack_empty(p->vertices)) {
            p->length += graph_edge_weight(G, top, v);
        }
        stack_push(p->vertices, v);
        return true;
    }
    return false;
}

//This function pops a vertex from path by poping a vertices from stack and storing it into a pointer. This function is the inverse of its push counterpart.
bool path_pop_vertex(Path *p, uint32_t *v, Graph *G) {
    uint32_t top = START_VERTEX;
    if (!stack_empty(p->vertices)) {
        stack_pop(p->vertices, v);
        stack_peek(p->vertices, &top);
        if (!stack_empty(p->vertices)) {
            p->length -= graph_edge_weight(G, top, (*v));
        }
        return true;
    }
    return false;
}

//This returns the number of vertices within the stack.
uint32_t path_vertices(Path *p) {
    return stack_size(p->vertices);
}

//This returns the total length of the path.
uint32_t path_length(Path *p) {
    return p->length;
}

//This copies the length and stacks from one path to another.
void path_copy(Path *dst, Path *src) {
    dst->length = src->length;
    stack_copy(dst->vertices, src->vertices);
}

//This is the function that is used in tsp.c to print out the path and path length for the dfs function and whenever it is called in main.
void path_print(Path *p, FILE *outfile, char *cities[]) {
    fprintf(outfile, "Path length: %" PRIu32 "\n", path_length(p));
    fprintf(outfile, "Path: ");
    stack_print(p->vertices, outfile, cities);
}
