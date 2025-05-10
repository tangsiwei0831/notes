#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kv.h"


kvarray_t * readKVs(const char * fname) {
  FILE *f = fopen(fname, "r");
  if (f == NULL) {
    perror("Could not open file");
    return NULL;
  }

  kvarray_t *kvArray = malloc(sizeof(*kvArray));
  if (kvArray == NULL) {
    perror("Could not allocate memory for kvarray_t");
    fclose(f);
    return NULL;
  }
  kvArray->pairs = NULL;
  kvArray->size = 0;

  char *line = NULL;
  size_t len = 0;
  while (getline(&line, &len, f) != -1) {
    char *equalsSign = strchr(line, '=');
    if (equalsSign == NULL) {
      fprintf(stderr, "Malformed line: %s", line);
      continue;
    }

    *equalsSign = '\0'; // Split the line into key and value
    char *key = strdup(line);
    char *value = strdup(equalsSign + 1);

    // Remove the newline character from the value
    char *newline = strchr(value, '\n');
    if (newline != NULL) {
      *newline = '\0';
    }

    kvpair_t *pair = malloc(sizeof(*pair));
    if (pair == NULL) {
      perror("Could not allocate memory for kvpair_t");
      free(key);
      free(value);
      continue;
    }
    pair->key = key;
    pair->value = value;

    kvArray->pairs = realloc(kvArray->pairs, (kvArray->size + 1) * sizeof(*kvArray->pairs));
    if (kvArray->pairs == NULL) {
      perror("Could not reallocate memory for pairs array");
      free(pair->key);
      free(pair->value);
      free(pair);
      break;
    }

    kvArray->pairs[kvArray->size] = pair;
    kvArray->size++;
  }

  free(line);
  fclose(f);
  return kvArray;
}

void freeKVs(kvarray_t * pairs) {
  if (pairs == NULL) {
    return; // If the input is NULL, there's nothing to free
  }

  // Free each key-value pair
  for (int i = 0; i < pairs->size; i++) {
    free(pairs->pairs[i]->key);   // Free the key string
    free(pairs->pairs[i]->value); // Free the value string
    free(pairs->pairs[i]);        // Free the kvpair_t structure
  }

  // Free the array of pointers to kvpair_t
  free(pairs->pairs);

  // Free the kvarray_t structure itself
  free(pairs);
}

void printKVs(kvarray_t * pairs) {
  if (pairs == NULL) {
    return; // If the input is NULL, there's nothing to print
  }

  for (int i = 0; i < pairs->size; i++) {
    printf("key = '%s' value = '%s'\n", pairs->pairs[i]->key, pairs->pairs[i]->value);
  }
}

char * lookupValue(kvarray_t * pairs, const char * key) {
  if (pairs == NULL || key == NULL) {
    return NULL; // If the input array or key is NULL, return NULL
  }

  // Iterate through the array of key-value pairs
  for (int i = 0; i < pairs->size; i++) {
    if (strcmp(pairs->pairs[i]->key, key) == 0) {
      return pairs->pairs[i]->value; // Return the value if the key matches
    }
  }

  return NULL; // Return NULL if no matching key is found
}
