#ifndef SETTER_H_
#define SETTER_H_

#include "parser.h"
#include "graphics.h"

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

int print_table(const table_t* table, const char* table_name, int highlight);
int save_table(FILE* fp, const table_t* table);

table_t* generate_series(const series_t** src, int src_count);
table_t* generate_sets(const table_t* possible_series);
int free_table(table_t* table);

int clean_and_sort_series_values(const table_t* table);
int remove_duplicate_series(table_t* table);
int filter_series_by_range(table_t* table, double min, double max);

#endif