/* 
 * Author: Zhixin Liu
 * UO ID: 951452405
 * Duck ID: zhixinl
 * Authorship Statement:
 * This is my own work.
*/
#include "tldlist.h"
#include "date.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct tldnode {
	char *name;
	TLDNode *left;
	TLDNode *right;
	long height;
	long count;
};

struct tldlist {
	TLDNode *node;
	Date *begin;
	Date *end;
	long size;
	long count;	
};

struct tlditerator {
	long t_next;
	long next;
	long size;
	TLDNode **traversal;
};

static long Find_Max(long a, long b) {
	return ((a > b) ? a : b);
}

static long Find_Hight(TLDNode *tld) {
	if(tld == NULL) 
		return 0;
	else 
		return (tld->height);
}

static TLDNode *LL_rotation(TLDNode *tree) {
	
	TLDNode *sub = tree->left;

    tree->left = sub->right;
    sub->right = tree;

    tree->height = Find_Max(Find_Hight(tree->left), Find_Hight(tree->right)) + 1;
    sub->height = Find_Max(tree->height, Find_Hight(sub->left)) + 1;

    return sub;
}

static TLDNode *RR_rotation(TLDNode *tree) {

	TLDNode *sub = tree->right;

    tree->right = sub->left;
    sub->left = tree;

    tree->height = Find_Max(Find_Hight(tree->left), Find_Hight(tree->right)) + 1;
    sub->height = Find_Max( tree->height, Find_Hight(sub->right)) + 1;

    return sub;
}

static TLDNode *LR_rotation(TLDNode *tree) {
	
	TLDNode *sub;

	tree->left = RR_rotation(tree->left);
	sub = LL_rotation(tree);

	return sub;
}

static TLDNode *RL_rotation(TLDNode *tree) {
	
	TLDNode *sub;

	tree->right = LL_rotation(tree->right);
	sub = RR_rotation(tree);

	return sub;
}

static TLDNode *node_create(char *hostname) {
	
	TLDNode *new_node;
	new_node = (TLDNode *) malloc (sizeof(TLDNode));
	
	if( new_node != NULL) {
		new_node->left = NULL;
		new_node->right = NULL;
		new_node->name = hostname;
		new_node->count = 1L;
		new_node->height = 0L;

		return new_node;
	}
	else 
		return NULL;
}

static TLDNode *node_add(TLDNode *c_node, char *hostname, TLDList *tld) {
	/* root case */
	if(c_node == NULL) {
		(tld->size)++;
		c_node = node_create(hostname);
		if(c_node == NULL) {
			free(hostname);
			return NULL;
		}
  	}

	/* left tree case */
	else if(strcmp(hostname, c_node->name) < 0) {
		c_node->left = node_add(c_node->left, hostname, tld);
		if(Find_Hight(c_node->left) - Find_Hight(c_node->right) > 1) {
			if(strcmp(hostname, c_node->left->name) < 0)
				c_node = LL_rotation(c_node);
			else
				c_node = LR_rotation(c_node);
		}
	}

	/* right tree case */
	else if(strcmp(hostname, c_node->name) > 0) {
		c_node->right = node_add(c_node->right, hostname, tld);
		if(Find_Hight(c_node->right) - Find_Hight(c_node->left) > 1) {
			if(strcmp(hostname, c_node->right->name) > 0)
				c_node = RR_rotation(c_node);
			else
				c_node = RL_rotation(c_node);
		}
	}
	/* repeated node case */
  	else {
  		(c_node->count)++;
  		free(hostname);
  		hostname = NULL;
  	}


	/* recalculate the height of node tree */
	c_node->height = Find_Max(Find_Hight(c_node->left), Find_Hight(c_node-> right)) + 1;
	return c_node;	
}

static void node_destroy(TLDNode *node) {
	if(node != NULL) {
		if(node->left != NULL)
        	node_destroy(node->left);
    	if(node->right != NULL)
        	node_destroy(node->right);
        if(node->name != NULL) {
        	free(node->name);
        }
        free(node);
        node = NULL;
	}
}

static void preorder_traversal(TLDIterator *iter, TLDNode *node_tree) {
	if(node_tree != NULL) {
		TLDNode **c_traversal = iter->traversal;

		c_traversal[iter->t_next] = node_tree;
		(iter->t_next)++;
		preorder_traversal(iter, node_tree->left);
		preorder_traversal(iter, node_tree->right);
	}
}

static char *split_url(char *hostname) {
	//printf("%s\n", hostname);
	const char delimit[2] = ".";
	int len = strlen(hostname);
	char tem[len];
	strcpy(tem, hostname); 
	char *token = NULL;
	char *last = NULL;

	token = strtok(tem, delimit);
	while(token != NULL) {
		last = (char *) malloc (strlen(token) +1);
		if(last == NULL)
			return NULL;

    	strcpy(last, token);
    	token = strtok(NULL, delimit);
    	if(token != NULL) {
    		free(last);
    		last = NULL;
    	}
	}
	return last;
}
/*
 * tldlist_create generates a list structure for storing counts against
 * top level domains (TLDs)
 *
 * creates a TLDList that is constrained to the `begin' and `end' Date's
 * returns a pointer to the list if successful, NULL if not
 */
TLDList *tldlist_create(Date *begin, Date *end) {
	
	TLDList *new_list;
	new_list = (TLDList *) malloc (sizeof(TLDList));
	
	if(new_list != NULL) {
		new_list->begin = date_duplicate(begin);
		new_list->end = date_duplicate(end);
		new_list->count = 0L;
		new_list->size = 0L;
		new_list->node = NULL;

		return new_list;
	}
	else 
		return NULL;
}

/*
 * tldlist_destroy destroys the list structure in `tld'
 *
 * all heap allocated storage associated with the list is returned to the heap
 */
void tldlist_destroy(TLDList *tld) {
	if(tld != NULL) {
		if(tld->node != NULL)
			node_destroy(tld->node);	
					
		date_destroy(tld->begin);
		date_destroy(tld->end);
		free(tld);
		tld = NULL;
	}
}

/*
 * tldlist_add adds the TLD contained in `hostname' to the tldlist if
 * `d' falls in the begin and end dates associated with the list;
 * returns 1 if the entry was counted, 0 if not
 */
int tldlist_add(TLDList *tld, char *hostname, Date *d) {

  	/* the date fall in date window */
  	if((date_compare(d, tld->begin) >= 0) && 
  		(date_compare(d, tld->end) <= 0)) {

  		/* convert hostname to lowercase */
		char *p = hostname;
		while(*p) {
	     	*p = tolower(*p);
	      	p++;
	  	}
  		/* get the top level domain */
  		char *top_host = split_url(hostname);
  		/* add process */
  		tld->node = node_add(tld->node, top_host, tld);
  	    (tld->count)++;

  		return 1;
  	}
  	else 
  		return 0;
}

/*
 * tldlist_count returns the number of successful tldlist_add() calls since
 * the creation of the TLDList
 */
long tldlist_count(TLDList *tld) {
	return (tld->count);
}

/*
 * tldlist_iter_create creates an iterator over the TLDList; returns a pointer
 * to the iterator if successful, NULL if not
 */
TLDIterator *tldlist_iter_create(TLDList *tld) {
	
	TLDIterator *iter = (TLDIterator *) malloc (sizeof(TLDIterator));
	
	if(iter != NULL) {

		TLDNode **temporary = NULL;
		long size = tld->size;
		/* create the traversal */
		//printf("%ld\n", size);
		temporary = (TLDNode **) malloc (size * sizeof(TLDNode *));
		if(temporary == NULL) {
			free(iter);
			iter = NULL;
		}
		else {
			iter->next = 0L;
			iter->t_next = 0L;
			iter->size = size;
			iter->traversal = temporary;
			preorder_traversal(iter, tld->node);
		}
	}
	return iter;
}

/*
 * tldlist_iter_next returns the next element in the list; returns a pointer
 * to the TLDNode if successful, NULL if no more elements to return
 */
TLDNode *tldlist_iter_next(TLDIterator *iter) {
	
	TLDNode *next_node = NULL;
	
	if ((iter->next) < (iter->size)) {
		next_node = iter->traversal[iter->next++];
		return next_node;
	} 
	else 
		return NULL;
}

/*
 * tldlist_iter_destroy destroys the iterator specified by `iter'
 */
void tldlist_iter_destroy(TLDIterator *iter) {
	if(iter != NULL) {
		if(iter->traversal != NULL) 
			free(iter->traversal);
		free(iter);
		iter = NULL;
	}
}

/*
 * tldnode_tldname returns the tld associated with the TLDNode
 */
char *tldnode_tldname(TLDNode *node) {
	return (node->name);
}

/*
 * tldnode_count returns the number of times that a log entry for the
 * corresponding tld was added to the list
 */
long tldnode_count(TLDNode *node) {
	return (node->count);
}
