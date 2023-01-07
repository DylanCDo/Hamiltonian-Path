#include "graph.h"
#include "path.h"
#include "stack.h"
#include "vertices.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/****************************************************/
// Filename: tsp.c
// Created: Dylan Do
/****************************************************/

bool verbose_printing
    = false; //This variable is responsible for determining if all hamiltonian paths should be printed.
uint32_t recursion
    = 0; //This variable is responsible for keeping count of how many times dfs does a recursion.
uint32_t shortest_length = 0;

void help(); //Declaration for the help function.

void dfs(Graph *G, uint32_t v, Path *curr, Path *shortest, char *cities[],
    FILE *outfile); //Declaration for the dfs function.

//THe main is responsible for running the command line options, file opening, memory allocation, and dfs.
int main(int argc, char **argv) {

    int opt = 0;
    uint32_t vertices = 0; //This variable specifies the number of vertices in a graph.
    uint32_t v_one = 0; //This number specifies one of the two vertices for the adjacency matrix.
    uint32_t v_two = 0; //This number specifies one of the two vertices for the adhacency matrix.
    uint32_t v_three
        = 0; //This number specifies the weight of the adjency matrix coordinate (v_0ne, v_two).
    bool graph_undirected = false; //Determines whether the graph is directed or undirected.
    FILE *infile = stdin; //The infile responsble for being the input file. Set to stdin by default.
    FILE *outfile
        = stdout; //The outfile is responsible for being the output file. Set to stdout by default.

    //This while loop is responsible for parsing through the command-lines given by a user.
    while ((opt = getopt(argc, argv, "hvui:o:")) != -1) {

        //This if statement is responsible for printing out the help statement if the user inputs an unknown command-line.
        if (opt == '?') {
            help();
            return -1;
        }

        //This are all the cases.
        switch (opt) {
        case 'h': help(); return -1;
        case 'v': verbose_printing = true; break;
        case 'u': graph_undirected = true; break;
        case 'i': infile = fopen(optarg, "r"); break;
        case 'o': outfile = fopen(optarg, "w"); break;
        }
    }

    //This section of code is responsible for scanning the number from the first line of the .graph files annd assigning it to vertices.
    fscanf(infile, "%" SCNu32 " ", &vertices);
    if (vertices
        > VERTICES) { //Checks if vertices is greater than VERTICES. If so, sets vertices to VERTICES>
        vertices = VERTICES;
    }

    //This section of code is responsible for allocating memory to the names array and inputing the location names from the infile.
    char buf[1024];
    char **names = calloc(vertices, sizeof(char *));
    for (uint32_t i = 0; i < vertices; i++) {
        names[i] = fgets(buf, 1024, infile);
        buf[strlen(buf) - 1] = '\0';
        char *dupe = strdup(buf);
        names[i] = dupe;
    }

    //Creates a graph that is vertices long.
    Graph *locations = graph_create(vertices, graph_undirected);

    //This while loop is responsible for finding the vertices and weight from the infile and adding it to the graph.
    while (NULL != fgets(buf, 1024, infile)) {
        fscanf(infile, "%" SCNu32 " %" SCNu32 " %" SCNu32, &v_one, &v_two, &v_three);
        graph_add_edge(locations, v_one, v_two, v_three);
        shortest_length += v_three;
    }

    fclose(infile);

    //Creates a path and a place holder path for the shortest apth.
    Path *curr = path_create();
    Path *shortest = path_create();

    //Runs the algorithm that finds the shortest Hamiltonian path.
    dfs(locations, START_VERTEX, curr, shortest, names, outfile);

    //Prints out the shortest Hamiltonian path and total recursive calls.
    path_print(shortest, outfile, names);
    fprintf(outfile, "Total recursive calls: %" PRIu32 "\n", recursion);

    //Freeing memory
    path_delete(&curr);
    path_delete(&shortest);
    graph_delete(&locations);
    for (uint32_t i = 0; i < vertices; i++) {
        free(names[i]);
    }
    free(names);
    return 0;
}

//This function prints out the help statement if the user specifies it in
//command line or if the user inputs a non-existing command-line case.
void help() {
    printf("SYNOPSIS\n");
    printf("  Traveling Salesman Problem using DFS.\n");
    printf("\n");
    printf("USAGE\n");
    printf("  ./tsp [-u] [-v] [-h] [-i infile] [-o outfile]\n");
    printf("\n");
    printf("OPTIONS\n");
    printf("  -u             Use undirected graph.\n");
    printf("  -v             Enable verbose printing.\n");
    printf("  -h             Program usage and help.\n");
    printf("  -i infile      Input containing graph (default: stdin)\n");
    printf("  -o outfile     Output of computed path (default: stdout)\n");
}

/****************************************************/
// Function: DFS
// Created: Dylan Do
// Psuedo Code: Darrel Long
// Summary: This section of code is responsible for
// finding the hamiltonian paths given a adjanceny graph
// with locations and vertices. The output will be sent
// to the variable outfie.
/****************************************************/

void dfs(Graph *G, uint32_t v, Path *curr, Path *shortest, char *cities[], FILE *outfile) {
    path_push_vertex(curr, v, G);
    graph_mark_visited(G, v);
    for (uint32_t i = 0; i < graph_vertices(G); i++) {
        if (graph_has_edge(G, v, i) && !graph_visited(G, i)
            && path_length(curr) < shortest_length) {
            recursion += 1;
            dfs(G, i, curr, shortest, cities, outfile);
        }
    }
    if (graph_has_edge(G, v, START_VERTEX) && path_vertices(curr) == graph_vertices(G)) {
        path_push_vertex(curr, START_VERTEX, G);
        if (path_length(shortest) > path_length(curr) || path_length(shortest) == 0) {
            shortest_length = path_length(curr);
            path_copy(shortest, curr);
        }

        if (verbose_printing) {
            path_print(curr, outfile, cities);
        }
        path_pop_vertex(curr, &v, G);
    }
    path_pop_vertex(curr, &v, G);
    graph_mark_unvisited(G, v);
}
