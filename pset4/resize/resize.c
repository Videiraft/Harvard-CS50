/**
 * Resizes 24-bit uncompressed BMPs by a factor of f.
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./copy ratio infile outfile\n");
        return 1;
    }

    // remember filenames and parse the float
    int ratio = atoi(argv[1]);
    if (ratio <= 0) {
        fprintf(stderr, "The ratio is not valid.\nUsage: ./copy ratio infile outfile\n");
        return 1;
    }
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
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
    
    //Save infile's padding, Width, and Height
    int OldPadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int OldWidth = bi.biWidth;
    int OldHeight = bi.biHeight;
    
    //Update outfile's BITMAPINFOHEADER
    bi.biWidth *= ratio;
    bi.biHeight *= ratio;
        // determine newpadding for scanlines
        int NewPadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + NewPadding) * abs(bi.biHeight);
    
    //Update outfile's BITMAPFILEHEADER
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);


    // iterate over infile's scanlines
    for (int i = 0; i < abs(OldHeight); i++) {
        
        // iterate the the number of times needed for the right number of columns
        for (int f = 0; f < ratio; f++) {
            
            // iterate over pixels in scanline
            for (int j = 0; j < OldWidth; j++) {
                
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
            
                // write RGB triple to outfile horizontlly according to ratio (>=1)
                for (int h = 0; h < ratio; h++) {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);    
                }
            }
            
            // Add the new padding to the output file
            for (int k = 0; k < NewPadding; k++) {
                fputc(0x00, outptr);
            }
            
            if (f < ratio - 1) {
               //Backforward to repeat the same row
                fseek(inptr, -(OldWidth*3), SEEK_CUR); 
            }
        }
        // skip over infile's padding, if any
        fseek(inptr, OldPadding, SEEK_CUR);
    }
    
    
    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
