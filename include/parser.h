#ifndef PARSER_H_
#define PARSER_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define GRADATIONS  "--grads"
#define END_EGRADS  "--end-grads"
#define SERIES_MAX  "--max"
#define SERIES_MIN  "--min"

#define UNDEFINED_BOUND 0xDEADBEEF

typedef struct {
    char** gradations;
    int drags_count;
    double min;
    double max;
} params_t;

params_t* parse_input(char* argv[], int argc);
int free_params(params_t* params);

#endif