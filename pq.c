/*
 * In this file, you will write the structures and functions needed to
 * implement a priority queue.  Feel free to implement any helper functions
 * you need in this file to implement a priority queue.  Make sure to add your
 * name and @oregonstate.edu email address below:
 *
 * Name: Kaedin Hanoano
 * Email: hanohank@oregonstate.edu
 */

#include <stdlib.h>
#include <assert.h>

#include "pq.h"
#include "dynarray.h"

/*
 * Structure to represent a node in the priority queue.
 * Each node stores a pointer to a value and its associated priority.
 */
struct pq_node
{
	void *value;
	int priority;
};

/*
 * This is the structure that represents a priority queue.
 * In this implementation, a priority queue is backed by a dynamic array
 * that stores pointers to pq_node structures arranged in a minimizing binary heap.
 */
struct pq
{
	struct dynarray *heap;
};

/*
 * Function: bubble_up
 * Description: Restores the heap property by moving the element at the given index upward
 *              until its parent's priority is less than or equal to its own.
 * Parameters:
 *   - pq: pointer to the priority queue.
 *   - index: starting index of the element to bubble up.
 * Returns: None.
 */
static void bubble_up(struct pq *pq, int index)
{
	while (index > 0)
	{
		int parent = (index - 1) / 2;
		struct pq_node *current = dynarray_get(pq->heap, index);
		struct pq_node *parentNode = dynarray_get(pq->heap, parent);
		if (parentNode->priority <= current->priority)
		{
			break;
		}
		/* Swap current node with parent node */
		dynarray_set(pq->heap, index, parentNode);
		dynarray_set(pq->heap, parent, current);
		index = parent;
	}
}

/*
 * Function: bubble_down
 * Description: Restores the heap property by moving the element at the given index downward
 *              until both children have priorities greater than or equal to it.
 * Parameters:
 *   - pq: pointer to the priority queue.
 *   - index: starting index of the element to bubble down.
 * Returns: None.
 */
static void bubble_down(struct pq *pq, int index)
{
	int size = dynarray_size(pq->heap);
	while (1)
	{
		int left = 2 * index + 1;
		int right = 2 * index + 2;
		int smallest = index;

		if (left < size)
		{
			struct pq_node *leftNode = dynarray_get(pq->heap, left);
			struct pq_node *smallestNode = dynarray_get(pq->heap, smallest);
			if (leftNode->priority < smallestNode->priority)
			{
				smallest = left;
			}
		}

		if (right < size)
		{
			struct pq_node *rightNode = dynarray_get(pq->heap, right);
			struct pq_node *smallestNode = dynarray_get(pq->heap, smallest);
			if (rightNode->priority < smallestNode->priority)
			{
				smallest = right;
			}
		}

		if (smallest == index)
		{
			break;
		}

		/* Swap the current node with the smallest child */
		struct pq_node *nodeIndex = dynarray_get(pq->heap, index);
		struct pq_node *nodeSmallest = dynarray_get(pq->heap, smallest);
		dynarray_set(pq->heap, index, nodeSmallest);
		dynarray_set(pq->heap, smallest, nodeIndex);
		index = smallest;
	}
}

/*
 * This function should allocate and initialize an empty priority queue and
 * return a pointer to it.
 */
struct pq *pq_create()
{
	struct pq *new_pq = malloc(sizeof(struct pq));
	assert(new_pq);
	new_pq->heap = dynarray_create();
	return new_pq;
}

/*
 * This function should free the memory allocated to a given priority queue.
 * Note that this function SHOULD NOT free the individual elements stored in
 * the priority queue.  That is the responsibility of the caller.
 *
 * Params:
 *   pq - the priority queue to be destroyed.  May not be NULL.
 */
void pq_free(struct pq *pq)
{
	assert(pq);
	int size = dynarray_size(pq->heap);
	for (int i = 0; i < size; i++)
	{
		struct pq_node *node = dynarray_get(pq->heap, i);
		free(node);
	}
	dynarray_free(pq->heap);
	free(pq);
}

/*
 * This function should return 1 if the specified priority queue is empty and
 * 0 otherwise.
 *
 * Params:
 *   pq - the priority queue whose emptiness is to be checked.  May not be
 *     NULL.
 *
 * Return:
 *   Should return 1 if pq is empty and 0 otherwise.
 */
int pq_isempty(struct pq *pq)
{
	assert(pq);
	return dynarray_size(pq->heap) == 0;
}

/*
 * This function should insert a given element into a priority queue with a
 * specified priority value.  Note that in this implementation, LOWER priority
 * values are assigned to elements with HIGHER priority.  In other words, the
 * element in the priority queue with the LOWEST priority value should be the
 * FIRST one returned.
 *
 * Params:
 *   pq - the priority queue into which to insert an element.  May not be
 *     NULL.
 *   value - the value to be inserted into pq.
 *   priority - the priority value to be assigned to the newly-inserted
 *     element.  Note that in this implementation, LOWER priority values
 *     should correspond to elements with HIGHER priority.  In other words,
 *     the element in the priority queue with the LOWEST priority value should
 *     be the FIRST one returned.
 */
void pq_insert(struct pq *pq, void *value, int priority)
{
	assert(pq);
	struct pq_node *node = malloc(sizeof(struct pq_node));
	assert(node);
	node->value = value;
	node->priority = priority;
	dynarray_insert(pq->heap, node);
	bubble_up(pq, dynarray_size(pq->heap) - 1);
}

/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void *pq_first(struct pq *pq)
{
	assert(pq);
	assert(dynarray_size(pq->heap) > 0);
	struct pq_node *node = dynarray_get(pq->heap, 0);
	return node->value;
}

/*
 * This function should return the priority value of the first item in a
 * priority queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a priority value.  May not be
 *     NULL or empty.
 *
 * Return:
 *   Should return the priority value of the first item in pq, i.e. the item
 *   with LOWEST priority value.
 */
int pq_first_priority(struct pq *pq)
{
	assert(pq);
	assert(dynarray_size(pq->heap) > 0);
	struct pq_node *node = dynarray_get(pq->heap, 0);
	return node->priority;
}

/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value, and then remove that item
 * from the queue.
 *
 * Params:
 *   pq - the priority queue from which to remove a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void *pq_remove_first(struct pq *pq)
{
	assert(pq);
	int size = dynarray_size(pq->heap);
	assert(size > 0);

	struct pq_node *firstNode = dynarray_get(pq->heap, 0);
	void *value = firstNode->value;

	/* Move the last element to the top and remove the last element */
	struct pq_node *lastNode = dynarray_get(pq->heap, size - 1);
	dynarray_set(pq->heap, 0, lastNode);
	dynarray_remove(pq->heap, size - 1);

	/* Free the removed node */
	free(firstNode);

	/* Restore heap property if there are remaining elements */
	if (dynarray_size(pq->heap) > 0)
	{
		bubble_down(pq, 0);
	}

	return value;
}
