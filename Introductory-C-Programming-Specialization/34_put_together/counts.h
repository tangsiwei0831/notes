#ifndef __COUNTS_H__
#define __COUNTS_H__

struct _one_count_t {
  const char * name;  // The string being counted
  int count;          // Number of times the string has been seen
};

typedef struct _one_count_t one_count_t;

struct _counts_t {
  one_count_t ** array;  // Array of pointers to one_count_t
  size_t size;           // Number of elements in the array
  int unknown_count;     // Count of unknown names
};

typedef struct _counts_t counts_t;

counts_t * createCounts(void);
void addCount(counts_t * c, const char * name);
void printCounts(counts_t * c, FILE * outFile);

void freeCounts(counts_t * c);

#endif
