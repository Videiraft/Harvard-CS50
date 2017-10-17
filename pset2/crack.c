#define _XOPEN_SOURCE
#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

//Cracking a password with a given hash of up to 4 letters, case sensitive, and with a salt of "50".
const char * salt = "50";
string inputHash;
int testCmp(string testStr);
int genPsw(string hash);
bool done = false;
    
int main(int argc, string argv[])
{
    //check if there is just one argument - the password
    if (argc != 2) {
        printf("Usage: ./crack hash\n");
        return 1;
    }
    inputHash = argv[1];
    
    genPsw(inputHash);
    return 0;
}

//Function to generate all the possible passwords with 4 letters, case sensitive
int genPsw(string hash) {
    char test1[2], test2[3], test3[4], test4[5];
    test1[1] = '\0';
    test2[2] = '\0';
    test3[3] = '\0';
    test4[4] = '\0';
    string possChar = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    for (int i=0; i<strlen(possChar); i++) {
        test1[0] = test2[0] = test3[0] = test4[0] = possChar[i];
        testCmp(test1);
        if (done == true) {
        break;
        }
        
        for (int j=0; j<strlen(possChar); j++) {
            test2[1] = test3[1] = test4[1] = possChar[j];
            testCmp(test2);
            
            for (int h=0; h<strlen(possChar); h++) {
                test3[2] = test4[2] = possChar[h];
                testCmp(test3);
                
                for (int k=0; k<strlen(possChar); k++) {
                    test4[3] = possChar[k];
                    testCmp(test4);
                }
            }
        }
    }
    return 0;
}

//Function to compare the given hash with the resulted hash from the possible password generated in genPsw()
int testCmp(string testStr) 
{
    int result = strcmp(inputHash, crypt(testStr,salt));
        if (result == 0) {
            printf("%s\n", testStr);
            done = true;
            return 0;
        }
    return 1;
}