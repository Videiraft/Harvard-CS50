/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>
#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // binary search - O(log n) searching algorithm
    int left = 0;
    int right = n -1;
    int middle;

    while (right >= left) {
        middle = ((right - left) /2) + left;
        if (values[middle] == value) {
            return true;
        } else if (values[middle] > value) {
            right = middle - 1;
        } else if (values[middle] < value) {
            left = middle + 1;
        }
    }
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // Counting sort - O(n^2) sorting algorithm
    int countArr[65536] = {0};
    int i;
    for (i = 0; i < n; i++) {
        countArr[values[i]] += 1;
    }
    
    int j;
    int h = 0;
    for (j = 0; j < 65536; j++) {
        while (countArr[j] != 0) {
            values[h] = j;
            countArr[j] -= 1;
            h++;
        }
    }
}
