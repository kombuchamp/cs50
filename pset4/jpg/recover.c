/****************************************************************************
 * recover.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 ***************************************************************************/
 
 
 #include <cs50.h>
 #include <stdio.h>
 #include <stdlib.h>
 
 #define BUFFER_SIZE 512
 
 int main(void)
 {
    //open memory card image
    
    FILE* input = fopen("card.raw", "r");
    if(input == NULL)
        return 1;
        
    // create buffer
    unsigned char buffer[BUFFER_SIZE];
    
    // filename counter
    int filecount = 0;
    
    FILE* picture = NULL;
    
    // flag for finding JPEG
    int isFound = 0;
    
    while (fread(buffer, BUFFER_SIZE, 1, input) == 1)
        {
            if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xe0) == 0xe0)
            {
                if(isFound == 1)
                {
                    fclose(picture);
                }
                else
                {
                    isFound = 1;
                }   
             
                char filename[8];
                sprintf(filename, "%03d.jpg", filecount);
             
                picture = fopen(filename, "a");
                filecount++;
            }
            
            if(isFound == 1)
            {
                fwrite(&buffer, BUFFER_SIZE, 1, picture);
            }
            
        }
        
        fclose(input);
        fclose(picture);
        
        return 0;
    
    
 }
