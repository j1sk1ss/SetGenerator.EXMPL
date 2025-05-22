#ifndef SETTER_H_
#define SETTER_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#define EPSILON 1e-9
#define FLOAT_EQUAL(a, b) (fabs(a - b) < EPSILON)

typedef struct {
    double gradation;
    double* series;
    int series_count;
} series_t;

typedef struct {
    series_t** series;
    int series_count;
} table_t;

int print_table(const table_t* table);
table_t* generate_series(const series_t** src, int src_count);
int free_table(table_t* table);

#endif