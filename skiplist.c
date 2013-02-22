#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define object int

#define MAX_LEVEL 18

typedef struct _node {
	int key;
	object *obj;
	struct _node *forward[1];
} node;

typedef struct _skiplist {
	int level;
	struct _node *head;
} skiplist;

static node *create_node(int level, int key, object *obj)
{
	node *nd = (node *)malloc(sizeof(node) + level * sizeof(node *));
	nd->obj = obj;
	nd->key = key;

	return nd;
}

skiplist *create_skiplist(void)
{
	int i;
	skiplist *sl = (skiplist *)malloc(sizeof(skiplist));
	sl->head = create_node(MAX_LEVEL, 0, NULL);

	for (i = 0; i < MAX_LEVEL; i++) {
		sl->head->forward[i] = NULL;
	}

	sl->level = 1;

	return sl;
}

static void free_node(node *nd)
{
	free(nd);
}

void free_skiplist(skiplist *sl)
{

}

static int random_level()
{
	int level = 1;
	while ((rand() & 0xFFFF) < (0.5 * 0xFFFF)) {
		level += 1;
	}
	return (level < MAX_LEVEL) ? level : MAX_LEVEL;

}

void insert(skiplist *sl, int key, object *obj)
{
	node  *update[MAX_LEVEL];
	node  *p, *q, *nd;

	int i, j, level;

	nd = sl->head;

	for (i = sl->level - 1; i >= 0; i--) {
		while (nd->forward[i] != NULL && nd->forward[i]->key < key)
			nd = nd->forward[i];
		update[i] = nd;
	}

	level = random_level();
	if (level > sl->level) {
		for (i = sl->level; i < level; i++) {
			update[i] = sl->head;
		}
		sl->level = level;
	}
	
	nd = create_node(level, key, obj);
	for (i = 0; i < level; i++) {
		nd->forward[i] = update[i]->forward[i];
		update[i]->forward[i] = nd;
	}

}	

static void delete_node(skiplist *sl, node *nd, node **update)
{
	int i;
	for (i = 0; sl->head->forward[i] != NULL; i++) {
		if (update[i]->forward[i] == nd) {
			update[i]->forward[i] = nd->forward[i];
		}
	}

	for (; i >= 0; i--) {
		if (sl->head->forward[i] == NULL) {
			sl->level--;
		}
	}
}

void delete(skiplist *sl, int key)
{
	node *update[MAX_LEVEL], *nd;
	int i;

	nd = sl->head;
	for (i = sl->level - 1; i >= 0; i--) {
		while (nd->forward[i] && nd->forward[i]->key < key) {
			nd = nd->forward[i];
		}
		update[i] = nd;
	}

	if (nd && nd->key == key) {
		delete_node(sl, nd, update);
		free_node(nd);
	}
}
























