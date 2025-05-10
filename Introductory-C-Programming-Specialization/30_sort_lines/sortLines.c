#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//This function is used to figure out the ordering
//of the strings in qsort.  You do not need
//to modify it.
int stringOrder(const void * vp1, const void * vp2) {
  const char * const * p1 = vp1;
  const char * const * p2 = vp2;
  return strcmp(*p1, *p2);
}
//This function will sort and print data (whose length is count).
void sortData(char ** data, size_t count) {
  qsort(data, count, sizeof(char *), stringOrder);
}

void processInput(FILE *input) {
  char **lines = NULL;
  size_t numLines = 0;
  char *line = NULL;
  size_t len = 0;

  // Read lines using getline
  while (getline(&line, &len, input) != -1) {
    lines = realloc(lines, (numLines + 1) * sizeof(*lines));
    if (lines == NULL) {
      perror("Failed to allocate memory");
      free(line);
      exit(EXIT_FAILURE);
    }
    lines[numLines] = line;
    line = NULL; // Reset line so getline allocates a new buffer
    numLines++;
  }
  free(line);

  // Sort the lines
  sortData(lines, numLines);

  // Print the sorted lines
  for (size_t i = 0; i < numLines; i++) {
    printf("%s", lines[i]);
    free(lines[i]); // Free each line after printing
  }
  free(lines); // Free the array of pointers
}

int main(int argc, char ** argv) {
  if (argc == 1) {
    // No arguments, read from standard input
    processInput(stdin);
  } else {
    // Process each file provided as an argument
    for (int i = 1; i < argc; i++) {
      FILE *file = fopen(argv[i], "r");
      if (file == NULL) {
        perror(argv[i]);
        return EXIT_FAILURE;
      }
      processInput(file);
      fclose(file);
    }
  }
  return EXIT_SUCCESS;
}
