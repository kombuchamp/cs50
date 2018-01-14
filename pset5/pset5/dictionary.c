/****************************************************************************
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Implements a dictionary's functionality.
 *
 * Author: kombuchamp (Belousov Konstantin)
 ***************************************************************************/

#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"
#include "hashtable.h"


/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    int num_of_letters = 0;
    char temp[num_of_letters+1];
    
    while(word[num_of_letters] != '\0')
    	num_of_letters++;
    
    // Saving word into temporary string in lowercase
    for(int i = 0; i < num_of_letters; i++)
    	temp[i] = tolower((unsigned char)word[i]);
    
    temp[num_of_letters] = '\0';
    
    // Finding corresponding group for the word
    node* first = NULL;
    distribute_on_check(temp, &first);
    
    // Returning true if the group conatains the word
    return contains(temp, first);
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    // Unload if already loaded
    unload();
    
    // Defines hashtable groups and dictionary wordcount
	hashtable[A_G] = NULL;
	hashtable[H_N] = NULL;
	hashtable[O_U] = NULL;
	hashtable[V_Z] = NULL;
	
	words_count = 0;
    
    // Following code opens file and distributing words by groups
    
    FILE* fptr = fopen(dictionary, "r");
    
    if(fptr == NULL)
    	return false;
        
    char* temp = calloc((LENGTH+1),sizeof(char));
        
    while(fscanf(fptr, "%s", temp) !=EOF)
    {
    	distribute_on_load(temp);
    	words_count++;
    }
    
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return words_count;
}

/**
 * Unloads dictionary from memory. Returns true.
 */
bool unload(void)
{
    for(int i = 0; i < HASHTABLE_SIZE; i++)
    {
    	while (hashtable[i] != NULL)
    	{
        	node* next = hashtable[i]->next;
        	free(hashtable[i]);
        	hashtable[i] = next;
    	}
    }
    
    return true;
}
