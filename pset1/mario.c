#include <stdio.h>
#include <cs50.h>

int main(void) 
{
    int height;
    do {
        printf("Height: ");
        height = get_int();
    } while (height > 23 || height < 0);
    
    for (int i = 1; i <= height; i++) //which row
    {
        for (int j = height -i; j > 0; j--) //spaces-left
        {
            printf(" ");
        }
        for (int h = 1; h <= i; h++) // blocks # -left
        {
            printf("#");
        }
        printf("  "); // spaces center
        for (int h = 1; h <= i; h++) // blocks # -right
        {
            (h == i) ? printf("#\n") : printf("#");
        }
    }
}