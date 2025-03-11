/*
 * In this file, you'll implement Dijkstra's algorithm to find the least
 * expensive paths in the graph defined in `airports.dat`.  Don't forget to
 * include your name and @oregonstate.edu email address below.
 *
 * Name: Kaedin Hanohano
 * Email: hanohank@oregonstate.edu
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

#include "pq.h"

/*
 * Structure representing an edge in the graph.
 * Each edge stores a destination vertex, a cost, and a pointer to the next edge.
 */
struct edge
{
	int dest;
	int cost;
	struct edge *next;
};

/*
 * Function: add_edge
 * Description: Adds an edge from source vertex to destination vertex with the given cost
 *              into the graph's adjacency list. The new edge is inserted at the beginning.
 * Parameters:
 *   - graph: the graph represented as an array of edge-list pointers.
 *   - src: the source vertex.
 *   - dest: the destination vertex.
 *   - cost: the weight/cost of the edge.
 * Returns: None.
 */
void add_edge(struct edge **graph, int src, int dest, int cost)
{
	struct edge *new_edge = malloc(sizeof(struct edge));
	assert(new_edge);
	new_edge->dest = dest;
	new_edge->cost = cost;
	new_edge->next = graph[src];
	graph[src] = new_edge;
}

/*
 * Function: read_graph
 * Description: Reads the graph from the given file and builds the adjacency list.
 *              Assumes the file pointer is positioned after the first two integers.
 *              For each edge read, adds edges in both directions (undirected graph).
 * Parameters:
 *   - file: pointer to the opened file containing the graph data.
 *   - n_nodes: number of nodes in the graph.
 *   - n_edges: number of edges in the graph.
 * Returns: Pointer to the graph (an array of pointers to edge lists).
 */
struct edge **read_graph(FILE *file, int n_nodes, int n_edges)
{
	int i, src, dest, cost;
	struct edge **graph = malloc(n_nodes * sizeof(struct edge *));
	assert(graph);
	for (i = 0; i < n_nodes; i++)
	{
		graph[i] = NULL;
	}
	for (i = 0; i < n_edges; i++)
	{
		fscanf(file, " %d %d %d ", &src, &dest, &cost);
		/* For undirected graph, add edge in both directions */
		add_edge(graph, src, dest, cost);
		add_edge(graph, dest, src, cost);
	}
	return graph;
}

/*
 * Function: free_graph
 * Description: Frees the memory allocated for the graph, including all edge nodes.
 * Parameters:
 *   - graph: the graph represented as an array of edge-list pointers.
 *   - n_nodes: the number of nodes in the graph.
 * Returns: None.
 */
void free_graph(struct edge **graph, int n_nodes)
{
	int i;
	struct edge *curr, *temp;
	for (i = 0; i < n_nodes; i++)
	{
		curr = graph[i];
		while (curr)
		{
			temp = curr;
			curr = curr->next;
			free(temp);
		}
	}
	free(graph);
}

/*
 * Function: dijkstra
 * Description: Implements Dijkstra's algorithm to compute the least-cost paths
 *              from the starting vertex to all other vertices in the graph.
 * Parameters:
 *   - graph: the graph represented as an array of edge-list pointers.
 *   - n_nodes: number of nodes in the graph.
 *   - start: starting vertex for Dijkstra's algorithm.
 *   - dist: array to store the computed shortest distances.
 *   - prev: array to store the predecessor of each vertex in the shortest path.
 * Returns: None.
 */
void dijkstra(struct edge **graph, int n_nodes, int start, int *dist, int *prev)
{
	int i;
	/* Initialize distance and previous arrays */
	for (i = 0; i < n_nodes; i++)
	{
		dist[i] = INT_MAX;
		prev[i] = -1;
	}
	dist[start] = 0;

	/* Create an array to mark visited vertices */
	int *visited = malloc(n_nodes * sizeof(int));
	assert(visited);
	for (i = 0; i < n_nodes; i++)
	{
		visited[i] = 0;
	}

	/* Create a priority queue to select the next vertex with the smallest distance */
	struct pq *pq = pq_create();
	/* Insert the start vertex into the PQ */
	int *start_ptr = malloc(sizeof(int));
	assert(start_ptr);
	*start_ptr = start;
	pq_insert(pq, start_ptr, dist[start]);

	while (!pq_isempty(pq))
	{
		/* Remove the vertex with the smallest tentative distance */
		int *u_ptr = (int *)pq_remove_first(pq);
		int u = *u_ptr;
		free(u_ptr);
		/* If already visited, skip */
		if (visited[u])
		{
			continue;
		}
		visited[u] = 1;

		/* Relaxation step for all adjacent edges */
		struct edge *e = graph[u];
		while (e)
		{
			int v = e->dest;
			if (!visited[v] && dist[u] != INT_MAX && dist[u] + e->cost < dist[v])
			{
				dist[v] = dist[u] + e->cost;
				prev[v] = u;
				int *v_ptr = malloc(sizeof(int));
				assert(v_ptr);
				*v_ptr = v;
				pq_insert(pq, v_ptr, dist[v]);
			}
			e = e->next;
		}
	}

	free(visited);
	pq_free(pq);
}

/*
 * Function: print_path
 * Description: Recursively prints the path from the starting vertex to vertex v.
 * Parameters:
 *   - start: the starting vertex.
 *   - v: the destination vertex whose path is to be printed.
 *   - prev: array containing the predecessor for each vertex.
 * Returns: None.
 */
void print_path(int start, int v, int *prev)
{
	if (v == start)
	{
		printf("%d", start);
	}
	else if (prev[v] == -1)
	{
		printf("No path");
	}
	else
	{
		print_path(start, prev[v], prev);
		printf(" -> %d", v);
	}
}

/*
 * Function: main
 * Description: Main driver for the Dijkstra's algorithm application.
 *              Reads the graph from the file, prompts the user for a starting node,
 *              computes the shortest paths using Dijkstra's algorithm, and prints the results.
 * Parameters:
 *   - argc: argument count.
 *   - argv: argument vector.
 * Returns:
 *   - 0 on successful execution.
 */
int main(int argc, char const *argv[])
{
	/*
	 * Open file and read the first two int: num of nodes, num of edges
	 */
	int n_nodes, n_edges;
	FILE *file = fopen("airports.dat", "r");
	fscanf(file, " %d %d ", &n_nodes, &n_edges);

	/* Read the graph from file */
	struct edge **graph = read_graph(file, n_nodes, n_edges);

	/* Prompt user for starting node */
	int start;
	printf("Input starting node (0 - %d): ", n_nodes - 1);
	scanf("%d", &start);

	/* Allocate arrays for distances and predecessors */
	int *dist = malloc(n_nodes * sizeof(int));
	int *prev = malloc(n_nodes * sizeof(int));
	assert(dist && prev);

	/* Run Dijkstra's algorithm */
	dijkstra(graph, n_nodes, start, dist, prev);

	/* Print the shortest paths */
	printf("Shortest paths from node #%d:\n", start);
	int i;
	for (i = 0; i < n_nodes; i++)
	{
		if (i == start)
		{
			printf("(Starting node, cost: 0)\n");
		}
		else if (dist[i] == INT_MAX)
		{
			printf("%d -> (No path)\n", start);
		}
		else
		{
			print_path(start, i, prev);
			printf(" (cost: %d)\n", dist[i]);
		}
	}

	/* Free allocated memory */
	free(dist);
	free(prev);
	free_graph(graph, n_nodes);

	fclose(file);

	return 0;
}
