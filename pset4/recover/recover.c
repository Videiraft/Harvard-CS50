
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int foo();

// Variables to create the JPEG files names
char filename[8];
int numJPG = 0;

// Create some malloc memory of size 512Bytes
unsigned char *buffer;


//pointer to file
FILE *file;

//Pointer to new image;
FILE *img;

int main(int argc, char *argv[]) {
    
    // Ensure proper use
    if (argc != 2) {
        fprintf(stderr, "Usage: ./recover file\n");
        return 1;
    }
    
    // Open raw memory card
    file = fopen(argv[1], "r");
    
    // Create some malloc memory of size 512Bytes
    buffer = malloc(sizeof(char) * 512);
    
    img = NULL;
    
    foo();
    
    fclose(file);
    free(buffer);
    fclose(img);

    return 0;
}

    int foo() {
        // read the file - 512 blocks of 1 byte each time
        fread(buffer, 1, 512, file);
        
        // Test if it's the begining of a new JPEG
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0) {
            
            // If it's not the first .jpg close the previous one
            if (img != NULL) {
                fclose(img);
                numJPG++;
            }
            // Create new .jpg file
            sprintf(filename, "%03i.jpg", numJPG);
            img = fopen(filename, "w");
            fwrite(buffer, 1, 512, img);
            foo();
        }
        
        // Test if it's the end of file - EOF
        if( feof(file) ) {
            return 0;
        }
        
        // Is it the first .jpg?
        if (img == NULL) {
            foo();
        } else {
            fwrite(buffer, 1, 512, img);
            foo();
        }
    
        return 0;

    }