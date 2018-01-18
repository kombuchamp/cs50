/*
 *
 *	Having fun with binary trees
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct node
{
	int value;
	struct node* left;
	struct node* right;
}
node;

node* build_node(int value)
{
	node* new_node = malloc(sizeof(node));
	
	if(new_node == NULL)
		{
			printf("Couldn't allocate memory!\n");
			exit(1);
		}
	new_node->value = value;
	new_node->left = NULL;
	new_node->right = NULL;
	
	return new_node;
}


// Inserting value into tree. Seems to work
bool insert(int value, node* tree)
{
	node* new_node = build_node(value);
	
	if(new_node == NULL)
		return false;
	
	new_node->value = value;
	new_node->left = NULL;
	new_node->right = NULL;
	
	node* crawler = tree;
	
	if (tree == NULL)
		return false;
	
	
	while(true)
	{
		if(crawler->value >= value )
			if(crawler->left == NULL)
				{
					crawler->left = new_node;
					return true;
				}
			else
				crawler = crawler->left;
		else
			if(crawler->right == NULL)
				{
					crawler->right = new_node;
					return true;
				}
			else
				crawler = crawler->right;
	}
	
}

// Staff recursive solution utilizing passing by refference
// its also tail recursive
bool insert_recursive(int value, node** tree)
{
	if(tree == NULL) // if pointer to a pointer(sic!) to a tree == null
		return false;
	if(*tree == NULL)
	{
		*tree = build_node(value);
		if (*tree == NULL)
			return false;
		return true;
	}
	return insert_recursive(value, value < (**tree).value ? &(**tree).left : &(**tree).right);
}


// First time I came up with recursive solution MYSELF (yeeeey)
bool contains(int value, node* tree)
{
	if(tree == NULL)
		return false;
	if(tree->value < value)
		return contains(value, tree->right);
	
	else if(tree->value > value)
		return contains(value, tree->left);
	
	
	else if(tree->value == value)
		return true;	 
}

// Rob Bowden's messed up oneliner (no idea why would you do this, but looks smart
bool contains_oneliner(int value, node* tree)
{
	return tree != NULL && (
	 value == tree->value || contains_oneliner(value, value < tree->value ? tree->left : tree->right));
}


// "Contains" function for any binary tree (not only sorted)
bool contains_generic_recursive(int value, node* tree)
{
	if (tree == NULL)
		return false;
	if (tree->value == value)
		return true;
	return contains_generic_recursive(value, tree->left) || contains_generic_recursive(value, tree->right);
}

// struct to keep track of subtrees we skip over in iterative solution
// (a linked list per se)
struct list
{
	node* n;
	struct list* next;
};

// Iterative version. Kind of sophisticateds
// LARGE memory leak here, need to free linked list
bool contains_generic_iterative(int value, node* tree)
{
	node* current = tree;
	
	struct list* list = malloc(sizeof(struct list));
	list->n = tree;
	list->next = NULL;
	
	while (list != NULL)
	{
		current = list->n;
		list = list->next;
	
		if (current->value == value)
			return true;
		if(current->right != NULL)
		{
			struct list* new_list = malloc(sizeof(struct list));
			new_list->n = current->right;
			new_list->next = list;
			list = new_list;
		}
		if (current->left != NULL)
		{
			struct list* new_list = malloc(sizeof(struct list));
			new_list->n = current->left;
			new_list->next = list;
			list = new_list;
		}

	}
}


int main()
{
	node* node7 = build_node(7);
	node* node6 = build_node(6);
	node* node3 = build_node(3);
	node* node9 = build_node(9);
	
	node7->left = node6;
	node7->right = node9;
	node6->left = node3;
	
	node* root = node7;
	
	if( contains(3, root) )
		printf("\"Contains\" function seems to work\n");
		
	insert(77, root);
	
	
	if( contains(77, root) )
		printf("Insertion works fine\n");
		
	insert_recursive( 1, &root);
	
	if( contains(1, root) )
		printf("Recursive insertion works fine\n");
		
	
	
	return 0;
}
