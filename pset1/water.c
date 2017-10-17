#include <stdio.h>
#include <cs50.h>

int main(void)
{
    printf("Minutes: ");
    int minutes = get_int();
    int bottles_minute = 12;
    
    printf("Bottles: %i\n", minutes * bottles_minute);
}