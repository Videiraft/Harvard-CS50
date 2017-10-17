#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int vigenereEncrypt(string p, string k);

int main(int argc, string argv[])
{
    //check if there are two arguments
    if (argc == 2) {
        string key = argv[1];
        //check if the arguments aren't letters
        for (int i=0; i<strlen(key); i++) {
            if(!isalpha(key[i])) {
                printf("Error. The command-line argument cannot contain any non-alphabetical character.\n");
                return 1;
            }
        }
        printf("plaintext: ");
        string plainTxt = get_string();
        
        printf("ciphertext: ");
        vigenereEncrypt(plainTxt, key);
        return 0;
    }
    
    printf("Error. The program accepts a single command-line argument.\n");
    return 1;
}

int vigenereEncrypt(string p, string k)
{
    // Defining keyPostion to manually loop through the key until the end of the plain text
    int keyPosition = 0;
        //Loop through the plain text to encrypt one character at a time
        for(int j=0; j<strlen(p); j++) {
            int cipherChar = p[j];
            
            if (keyPosition >= strlen(k)){
                keyPosition = 0;
            }

            if (isalpha(p[j])) {
                if (k[keyPosition]<='Z') {
                    cipherChar += (k[keyPosition]-'A');
                    if (cipherChar > 'Z') {
                        cipherChar -= 26;
                    }
                } else {
                    cipherChar += (k[keyPosition]-'a');
                    if (cipherChar > 'z') {
                        cipherChar -= 26;
                    }
                }
                printf("%c", (char) cipherChar);
                keyPosition++;
            } else {
                printf("%c", p[j]);
            }
        }
        printf("\n");
        return 0;
}