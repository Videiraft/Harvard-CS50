#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>


int main(void)
{
    string name = get_string();
    
    for (int i = 0, slen = strlen(name); i < slen; i++) {
        if (name[i] != 32) {
            printf("%c", toupper(name[i]));
            do {
                i++;
            } while (name[i] != 32 && i < slen);
        }    
    }
    printf("\n");
}