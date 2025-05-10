#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "outname.h"

char * computeOutputFileName(const char * inputName) {
    // Calculate the length of the new string (inputName + ".counts" + null terminator)
    size_t inputLength = strlen(inputName);
    size_t outputLength = inputLength + strlen(".counts") + 1;

    // Allocate memory for the new string
    char * outputName = malloc(outputLength * sizeof(*outputName));
    if (outputName == NULL) {
        perror("Failed to allocate memory for output file name");
        return NULL;
    }

    // Copy the input name and append ".counts"
    strcpy(outputName, inputName);
    strcat(outputName, ".counts");

    return outputName;
}
