/****************************************************************************
 * puff.c
 *
 * Computer Science 50
 * Problem Set 6
 *
 * Decodes files that was coded by Huffman's method.
 ***************************************************************************/

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "forest.h"
#include "huffile.h"
#include "tree.h"

// first and last symbols to dump
#define FIRST 0
#define LAST '~'


int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 3)
    {
        printf("Usage: %s [input] [output file name]\n", argv[0]);
        return 1;
    }

    // open input
    Huffile* input = hfopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open %s for reading.\n", argv[1]);
        return 1;
    }

    // read in header
    Huffeader header;
    if (hread(&header, input) == false)
    {
        hfclose(input);
        printf("Could not read header.\n");
        return 1;
    }

    // check for magic number
    if (header.magic != MAGIC)
    {
        hfclose(input);
        printf("File was not huffed.\n");
        return 1;
    }

    // check checksum
    int checksum = header.checksum;
    for (int i = 0; i < SYMBOLS; i++)
    {
        checksum -= header.frequencies[i];
    }
    if (checksum != 0)
    {
        hfclose(input);
        printf("File was not huffed.\n");
        return 1;
    }

	/*
	 * Creating a forest
	 *
	 */
	 
	 // Create a forest and a tree
	 Forest* forest = mkforest();
	 Tree* tree;
	 
	 // Creating tree counter. Contains number of trees in the forest
	 int count = 0;
	 
	 // Creating single-noded trees for characters in input document
	 for(int index = FIRST; index <= LAST; index++)
	 {
	 	// ignore symbols with 0 frequencies
	 	if(header.frequencies[index] == 0)
	 		continue;
	 	
	 	tree = mktree();
	 	tree->symbol = (char)index;
	 	tree->frequency = header.frequencies[index];
	 	
	 	if(!plant(forest, tree))
	 		{
	 			printf("Couldn't plant a tree\n");
	 			hfclose(input);
	 			rmforest(forest);
	 			rmtree(tree);
	 			return 1;
	 		}
	 	count++; // increase count of trees
	 }
	 
	// Building a Huffman tree
	Tree* left_tree;
	Tree* right_tree;
	Tree* bigger_tree;
	
	while (count > 1)
	{
		// pick two trees with least frequencies from the forest and decrease its tree counter
		left_tree = pick(forest); count--;
		right_tree = pick(forest); count--;
		
		// create tree with previously picked trees as children
		bigger_tree = mktree();
		
		if(left_tree == NULL || right_tree == NULL || bigger_tree == NULL)
			{
				printf("Couldn't pick from the forest (current tree count: %d", count);
				hfclose(input);
	 			rmforest(forest);
	 			rmtree(tree);
	 			rmtree(left_tree);
	 			rmtree(right_tree);
	 			rmtree(bigger_tree);
				return 1;
			}
		
		bigger_tree->frequency = left_tree->frequency + right_tree->frequency;
		bigger_tree->left = left_tree;
		bigger_tree->right = right_tree;
		
		if( !plant(forest, bigger_tree) )
		{
			printf("Couldn't plant a tree");
			hfclose(input);
	 		rmforest(forest);
	 		rmtree(tree);
	 		rmtree(left_tree);
	 		rmtree(right_tree);
	 		rmtree(bigger_tree);
			return -1;
		}
		count++; // increse tree count
	}
	
	
    // read bits from input and decipher them
    int bit;
    Tree* crawler = forest->first->tree;
    
    // output file
    FILE* fp = fopen(argv[2], "w");
    
    // parse code
    while ((bit = bread(input)) != EOF)
    {   
        if(bit)
        	crawler = crawler->right;
        else
        	crawler = crawler->left;
        
        // if symbol is found - output and get back to the starting point
        if(crawler->left == NULL && crawler->right == NULL)
        {
        	printf("%c", crawler->symbol);
        	fputc( (int)crawler->symbol, fp);
        	crawler = bigger_tree;
        }
        
        	
    }
    

 	// exit operations
    hfclose(input);
    fclose(fp);
    
    rmtree(tree);
    rmtree(left_tree);
    rmtree(right_tree);
    rmtree(bigger_tree);
    rmtree(crawler);
    
    rmforest(forest);
    
    
    return 0;
}
