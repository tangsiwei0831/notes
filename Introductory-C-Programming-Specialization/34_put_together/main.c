#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kv.h"
#include "counts.h"
#include "outname.h"

counts_t * countFile(const char * filename, kvarray_t * kvPairs) {
  // Open the input file
  FILE * f = fopen(filename, "r");
  if (f == NULL) {
    perror("Could not open input file");
    return NULL;
  }

  // Create a counts_t structure
  counts_t * counts = createCounts();

  // Read each line from the file
  char * line = NULL;
  size_t len = 0;
  while (getline(&line, &len, f) != -1) {
    // Remove the newline character
    char * newline = strchr(line, '\n');
    if (newline != NULL) {
      *newline = '\0';
    }

    // Lookup the value for the key
    char * value = lookupValue(kvPairs, line);

    // Add the value to the counts
    addCount(counts, value);
  }

  // Clean up
  free(line);
  fclose(f);

  return counts;
}

int main(int argc, char ** argv) {
  if (argc < 2) {
    fprintf(stderr,"no enough arguments");
    return EXIT_FAILURE;
  }
  //read the key/value pairs from the file named by argv[1] (call the result kv)
  kvarray_t * kv = readKVs(argv[1]);
  if (kv == NULL) {
    return EXIT_FAILURE;
  }
  // Process each input file
  for (int i = 2; i < argc; i++) {
    // Count the values in the input file
    counts_t * counts = countFile(argv[i], kv);
    if (counts == NULL) {
      freeKVs(kv);
      return EXIT_FAILURE;
    }

    // Compute the output file name
    char * outName = computeOutputFileName(argv[i]);
    if (outName == NULL) {
      freeCounts(counts);
      freeKVs(kv);
      return EXIT_FAILURE;
    }

    // Open the output file
    FILE * outFile = fopen(outName, "w");
    if (outFile == NULL) {
      perror("Could not open output file");
      free(outName);
      freeCounts(counts);
      freeKVs(kv);
      return EXIT_FAILURE;
    }

    // Print the counts to the output file
    printCounts(counts, outFile);

    // Clean up
    fclose(outFile);
    free(outName);
    freeCounts(counts);
  }

  // Free the key/value pairs
  freeKVs(kv);

  return EXIT_SUCCESS;
}
