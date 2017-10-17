/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include "dictionary.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct node {
    bool is_word;
    struct node *children[27];
}
node;

//pointer to the root of the trie
node *root;

//number of words in the dictionary
int wordCount = 0; 

//declare function to recursively free memory space
void recuUnl(node *current);




    
/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    // start at the root
    node *curNode = root;
    
    int cIndex;
    int i = 0;
    while(word[i] != 0) {
        // set the correct index number to the word[i] letter/character
        if (word[i]=='\'') {
            cIndex = 0;
        } else {
            if(word[i] >=65 && word[i] <= 90) {
                cIndex = word[i] - 64;
            } else {
                cIndex = word[i] - 96;
            }
        }
        
        // check if the corresponding element in children is NULL - check if the letters checked so far make up no word
        if(curNode->children[cIndex] == NULL) {
            return false;
        // or just move to the next children/letter    
        } else {
            curNode = curNode->children[cIndex];
        }
        // add 1 to counter i so it is possible to check the next letter/character of the word
        i++;
    }
    
    //check if the letters create a valid word
    if(curNode->is_word == true) {
        return true;
    } else {
        return false;
    }
}





/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // try to open file
    FILE *fd = fopen(dictionary, "r");
    if (fd == NULL)
    {
        printf("Could not open %s.\n", dictionary);
        return false;
    }
    
    // calloc some memory to create a root node to insert the dictionary words into the trie
    root = calloc(1, sizeof(node));
    if(root == NULL) {
        printf("Could not open %s.\n", dictionary);
        return false;
    }
    
    // variable to save the words that fscanf() parses
    char word[45];
    // variable to determine the index for each letter/character when choosing the children in the nodes
    int cIndex;
    // pointer to the current node during the loading of each word (character)
    node* actualNode = root;
    
    while (fscanf(fd, "%s", word) != EOF) {
        for (int i = 0; i < strlen(word); i++) {
            // gets the right index for children[27] to each letter
            if (word[i]=='\'') {
                cIndex = 0;
            } else {
                cIndex = word[i] - 96;
            }
            
            // creates a new node in the right index of children - if the index is still NULL - and goes there
            if (actualNode->children[cIndex] == NULL) {
                // calloc a new node and have the current children point to it
                actualNode->children[cIndex] = calloc(1, sizeof(node));
                // go to the new node
                actualNode = actualNode->children[cIndex];
            } else {
                // go to the next node
                actualNode = actualNode->children[cIndex];
            }
            
        }
        // at the end of the word loading it sets the actual word to possible (true)
        actualNode->is_word = true;
        wordCount ++;

        // at the end of each word resets the actualNode to the root, so it is possible to add a new word
        actualNode = root;

    }
    
    fclose(fd);
    return true;
}





/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return wordCount;
}





/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    node *ptr = root;
    if (ptr == NULL) {
        return false;
    }
    
    recuUnl(ptr);
    
    return true;
}





void recuUnl(node *current) {
    if(!current) {
        return;
    }
    
    for(int i = 0; i < 27; i++) {
        if(current->children[i] != NULL) {
            recuUnl(current->children[i]);
        }
    }
    free(current);
    return;
}