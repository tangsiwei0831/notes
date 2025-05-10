#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "counts.h"

counts_t * createCounts(void) {
  // Allocate memory for the counts_t structure
  counts_t * c = malloc(sizeof(*c));
  if (c == NULL) {
    perror("Failed to allocate memory for counts_t");
    exit(EXIT_FAILURE);
  }

  // Initialize the fields
  c->array = NULL;       // No counts yet, so the array is NULL
  c->size = 0;           // No elements in the array
  c->unknown_count = 0;  // No unknown names counted yet

  return c;
}

void addCount(counts_t * c, const char * name) {
  if (name == NULL) {
    // Increment the unknown count if the name is NULL
    c->unknown_count++;
    return;
  }

  // Check if the name already exists in the array
  for (size_t i = 0; i < c->size; i++) {
    if (strcmp(c->array[i]->name, name) == 0) {
      // Increment the count for the existing name
      c->array[i]->count++;
      return;
    }
  }

  // If the name is not found, add a new entry
  // Allocate memory for a new one_count_t
  one_count_t * new_count = malloc(sizeof(*new_count));
  if (new_count == NULL) {
    perror("Failed to allocate memory for one_count_t");
    exit(EXIT_FAILURE);
  }
  new_count->name = strdup(name);  // Duplicate the name string
  new_count->count = 1;

  // Resize the array to hold the new entry
  c->array = realloc(c->array, (c->size + 1) * sizeof(*c->array));
  if (c->array == NULL) {
    perror("Failed to resize array");
    exit(EXIT_FAILURE);
  }

  // Add the new entry to the array
  c->array[c->size] = new_count;
  c->size++;
}

void printCounts(counts_t * c, FILE * outFile) {
  // Print each name and its count
  for (size_t i = 0; i < c->size; i++) {
    fprintf(outFile, "%s: %d\n", c->array[i]->name, c->array[i]->count);
  }

  // Print the unknown count if it is greater than 0
  if (c->unknown_count > 0) {
    fprintf(outFile, "<unknown> : %d\n", c->unknown_count);
  }
}

void freeCounts(counts_t * c) {
  // Free each one_count_t in the array
  for (size_t i = 0; i < c->size; i++) {
    free((char *)c->array[i]->name);  // Free the duplicated name string
    free(c->array[i]);               // Free the one_count_t structure
  }

  // Free the array of pointers
  free(c->array);

  // Free the counts_t structure itself
  free(c);
}
