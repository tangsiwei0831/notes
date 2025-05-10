#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void encrypt(FILE *f, int key, FILE *outfile)
{
  char *line = NULL; // Initialize to NULL for getline
  size_t sz = 0;     // Initialize size to 0
  while (getline(&line, &sz, f) >= 0)
  {
    char *ptr = line;
    while (*ptr != '\0')
    {
      int c = *ptr;
      if (isalpha(c))
      {
        c = tolower(c);
        c -= 'a';
        c += key;
        c %= 26;
        c += 'a';
      }
      *ptr = c;
      ptr++;
    }
    fprintf(outfile, "%s", line);
  }
  free(line); // Free the allocated memory for line
  // No need to free sz, as it's not dynamically allocated
  // The getline function takes care of the memory management for line
  // and sz, so we only need to free line.
  // sz is just a size_t variable, not dynamically allocated.
  // The getline function will automatically allocate memory for line
  // and manage its size, so we don't need to worry about sz.
}

int main(int argc, char **argv)
{
  if (argc != 3)
  {
    fprintf(stderr, "Usage: encrypt key inputFileName\n");
    return EXIT_FAILURE;
  }
  int key = atoi(argv[1]);
  if (key == 0)
  {
    fprintf(stderr, "Invalid key (%s): must be a non-zero integer\n", argv[1]);
    return EXIT_FAILURE;
  }
  FILE *f = fopen(argv[2], "r");
  if (f == NULL)
  {
    perror("Could not open file");
    return EXIT_FAILURE;
  }
  // Allocate memory for output file name (+5 for ".enc\0")
  char *outFileName = malloc((strlen(argv[2]) + 5) * sizeof(*outFileName));
  strcpy(outFileName, argv[2]);
  strcat(outFileName, ".enc");
  FILE *outFile = fopen(outFileName, "w");
  encrypt(f, key, outFile);
  if (fclose(outFile) != 0)
  {
    perror("Failed to close the input file!");
    return EXIT_FAILURE;
  }
  if (fclose(f) != 0)
  {
    perror("Failed to close the input file!");
    return EXIT_FAILURE;
  }
  free(outFileName); // Free the allocated memory
  return EXIT_SUCCESS;
}
