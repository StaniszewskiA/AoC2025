#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helpers/input_helper.h"

char** read_input_file(const char *filename, int *out_count) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening input file");
        *out_count = 0;
        return NULL;
    }
    char **lines = NULL;
    int count = 0, capacity = 0;
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file)) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        if (count == capacity) {
            capacity = capacity ? capacity * 2 : 128;
            lines = realloc(lines, capacity * sizeof(char *));
        }
        lines[count] = malloc(len + 1);
        strcpy(lines[count], buffer);
        count++;
    }
    fclose(file);
    *out_count = count;
    return lines;
}
