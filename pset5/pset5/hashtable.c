/*************************************************************************
 * sll.c
 *
 * Implements a hashtable functionality 
 *
 * Author: kombuchamp (Belousov Konstantin)
 ************************************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"
#include "hashtable.h"

#define HASHTABLE_SIZE 4 // Number of groups in hash table
#define A_G 0
#define H_N 1
#define O_U 2
#define V_Z 3



// Number of words in hashtable.
int words_count = 0;


/**
 * Returns true if a node in the list contains the value [string] and false
 * otherwise.
 */
bool contains(char* string, node* first)
{
    node* crawler = first;
    
    while(crawler != NULL)
    {
        if(strcmp(crawler->word, string) == 0)
            {
            	return true;
            }
        crawler = crawler->next;
    }
    
    return false;
}

/**
 * Puts a new node containing [string] at the front (head) of the list.
 */
void prepend(char* string, node** first) // Need to pass "first" by ref
{
    node* new_node = malloc(sizeof(node));
    
    strcpy(new_node->word, string);
    new_node->next = *first;
    
    *first = new_node;
}

/**
 * Puts a new node containing [string] at the end (tail) of the list.
 */
void append(char* string, node** first)
{
    node* crawler = *first;
    
    if(crawler == NULL)
        {
            prepend(string, &crawler);
            *first = crawler;
            return;
        }
    
    node* new_node = malloc(sizeof(node));
    
    new_node->next = NULL;
    strcpy(new_node->word, string);
    
    
    while(crawler->next != NULL)
        crawler = crawler->next;
    
    crawler->next = new_node;
    
}


/**
 * Distributes words by groups of first letters ("buckets" of hashtable).
 * [bucket] is an output variable for the first node of corresponding group.
 */
void distribute_on_load(char* word)
{
	if('a' <= word[0] && word[0] <= 'g')
    	append(word, &hashtable[A_G]);
    else
    if('h' <= word[0] && word[0] <= 'n')
    	append(word, &hashtable[H_N]);
    else
    if('o' <= word[0] && word[0] <= 'u')
    	append(word, &hashtable[O_U]);
    else
    if('v' <= word[0] && word[0] <= 'z')
    	append(word, &hashtable[V_Z]);
}

void distribute_on_check(char* word, node** bucket)
{
	if('a' <= word[0] && word[0] <= 'g')
    	*bucket = hashtable[A_G];
    else
    if('h' <= word[0] && word[0] <= 'n')
    	*bucket = hashtable[H_N];
    else
    if('o' <= word[0] && word[0] <= 'u')
    	*bucket = hashtable[O_U];
    else
    if('v' <= word[0] && word[0] <= 'z')
    	*bucket = hashtable[V_Z];
}

