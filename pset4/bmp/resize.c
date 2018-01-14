/****************************************************************************
 * resize.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Must resize bmp files
 ***************************************************************************/
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: resize n infile outfile\n");
        return 1;
    }
    
    int n;
    sscanf(argv[1], "%d", &n);
    
    if (n < 0 || n > 100)
        return 100;
    
    
    
    // remember filenames
    char* infile = argv[2];
    char* outfile = argv[3];

    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    
    // Arrangements required for resizing headers
    
    // Headers of an old image:
    int old_biWidth = bi.biWidth;
    int old_biHeight = bi.biHeight;
    int old_Padding = (4 - (old_biWidth * sizeof(RGBTRIPLE)) % 4) % 4; // in bytes
    

    
    // changing width and height in headers
    
    bi.biWidth = old_biWidth * n; // width in pixels
    bi.biHeight = old_biHeight * n; // height in pixels
    
    // Calculating new padding
    int new_Padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // changing size and sizeimage in headers
    bi.biSizeImage = ((bi.biWidth * sizeof(RGBTRIPLE)) + new_Padding) * abs(bi.biHeight) ; // size in bytes of the image
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER); // size in bytes of the bitmap file

    
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    
    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(old_biHeight); i < biHeight; i++)
    {
		for (int k_row = 0; k_row < n; k_row++)
		{
			// iterate over pixels in scanline
			for (int j = 0; j < old_biWidth; j++)
			{
            
				for (int k_col = 0; k_col < n; k_col++)
            	{
					// temporary storage
					RGBTRIPLE triple;

					// read RGB triple from infile
					fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
            
					// write RGB triple to outfile
					
					fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
				
					// move file pointer back one pixel if not at the last one
					if (k_col != (n-1))
						fseek(inptr, -sizeof(RGBTRIPLE), SEEK_CUR);
				}
            
			}
		// skip over padding, if any
			fseek(inptr, old_Padding, SEEK_CUR);    
        
		// then add it back (to demonstrate how)
		for (int l = 0; l < new_Padding; l++)
			fputc(0x00, outptr);    

		
		
		// move file pointer back to the beginning of the row if row is not last
		if (k_row != (n-1))
			fseek(inptr, (-sizeof(RGBTRIPLE) * old_biWidth) - old_Padding, SEEK_CUR);
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}
