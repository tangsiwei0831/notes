#include <stdio.h>
#include <stdlib.h>

void rotate(char matrix[10][10]){
    char temp[10][10];
    int i, j;

    // Rotate the matrix 90 degrees clockwise
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            temp[j][9 - i] = matrix[i][j];
        }
    }

    // Copy the rotated matrix back to the original matrix
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            matrix[i][j] = temp[i][j];
        }
    }
}