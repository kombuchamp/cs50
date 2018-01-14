/*************************************************************************
 * sll.h
 *
 * Declares a hashtable elements and functionality
 *
 * Author: kombuchamp (Belousov Konstantin)
 ************************************************************************/

 
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define HASHTABLE_SIZE 4 // Number of groups
#define A_G 0
#define H_N 1
#define O_U 2
#define V_Z 3


/*
 * Hash table for dictionary
 */

// Node of linked list
typedef struct node
{
	char word[LENGTH+1];
	struct node* next;
}
node;

// "Bucket" of the hash table
node* hashtable[HASHTABLE_SIZE];

// Number of words in hashtable.
extern int words_count;

/**
 * Returns true if a node in the list contains the value [string] and false
 * otherwise.
 */
bool contains(char* string, node* first);

/**
 * Puts a new node containing [string] at the front (head) of the list.
 */
void prepend(char* string, node** first); // Need to pass "first" by ref

/**
 * Puts a new node containing [string] at the end (tail) of the list.
 */
void append(char* string, node** first);

/**
 * Distributes words by groups of first letters ("buckets" of hashtable).
 * [bucket] is an output variable for the first node of corresponding group.
 */
void distribute_on_load(char* word); 
 
void distribute_on_check(char* word, node** bucket);

