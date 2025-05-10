#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int descrypt(FILE *f) {  
    int arr[26] = {0};
    int c;

    // Count the frequency of each letter
    while ((c = fgetc(f)) != EOF) {
        if (isalpha(c)) {
            c = tolower(c);
            c -= 'a';
            arr[c]++;
        }
    }

    // Find the most frequent letter
    int max = 0;
    int j = 0;
    for (int i = 0; i < 26; i++) {
        if (arr[i] > max) {
            max = arr[i];
            j = i;
        }
    }

    // Calculate the key based on the most frequent letter
    int k = 0;
    if (j >= ('e' - 'a')) {
        k = j - ('e' - 'a');
    } else {
        k = (26 - ('e' - 'a')) + j;
    }

    return k;
}

int main(int argc,char** argv){
    if (argc != 2 ){
      fprintf(stderr,"not enough arguments\n");
      return EXIT_FAILURE;
    }
   
   FILE * f= fopen(argv[1] ,"r");

   if (f == NULL) {
      perror("Could not open file");
      return EXIT_FAILURE;      
   }
   
   int key= descrypt(f);
   printf("%d\n",key);
   
   if (fclose(f) != 0) {
      perror("Failed to close the input file!");
      return EXIT_FAILURE;
   }
   
   return EXIT_SUCCESS;
   
}