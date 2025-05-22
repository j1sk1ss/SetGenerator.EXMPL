#include "../include/setter.h"

series_t* _create_series(int series_count, double gradation) {
    series_t* series = (series_t*)malloc(sizeof(series_t));
    if (!series) return NULL;

    series->gradation = gradation;
    series->series_count = series_count;
    series->series = (double*)malloc(sizeof(double) * series_count);
    if (!series->series) {
        free(series);
        return NULL;
    }

    return series;
}

static int _add_series_to_table(table_t* table, series_t* new_series) {
    if (!table || !new_series) return 0;

    int new_count = table->series_count + 1;
    series_t** resized = (series_t**)realloc(table->series, sizeof(series_t*) * new_count);
    if (!resized) return 0;
    
    table->series = resized;
    table->series[table->series_count] = new_series;
    table->series_count = new_count;

    return 1;
}

static int _find_possible_ends(const series_t* next, double value) {
    if (!next || !next->series) return 0;
    for (int i = 0; i < next->series_count; i++) {
        if (FLOAT_EQUAL(value + next->gradation, next->series[i])) {
            return 1;
        }
    }

    return 0;
} 

table_t* generate_series(const series_t** src, int src_count) {
    table_t* table = (table_t*)malloc(sizeof(table_t));
    if (!table) return NULL;

    series_t* first_series = _create_series(src[0]->series_count, src[0]->gradation);
    if (!first_series) return NULL;
    memcpy(first_series->series, src[0]->series, sizeof(double) * src[0]->series_count);
    _add_series_to_table(table, first_series);

    for (int i = 1; i < src_count - 1; i++) {
        const series_t* curr = src[i];
        for (int j = 0; j < curr->series_count; j++) {
            if (_find_possible_ends(src[i + 1], curr->series[j])) {
                series_t* possible = _create_series(j + 1, curr->gradation);
                if (!possible) goto cleanup;
                memcpy(possible->series, curr->series, sizeof(double) * (j + 1));
                _add_series_to_table(table, possible);
            }
        }
    }

    series_t* last_series = _create_series(src[src_count - 1]->series_count, src[src_count - 1]->gradation);
    if (!last_series) return NULL;
    memcpy(last_series->series, src[src_count - 1]->series, sizeof(double) * src[src_count - 1]->series_count);
    _add_series_to_table(table, last_series);

    return table;

cleanup:
    free_table(table);
    return NULL;
}

int free_table(table_t* table) {
    if (!table) return 0;

    for (int i = 0; i < table->series_count; ++i) {
        series_t* s = table->series[i];
        if (s) {
            free(s->series);
            free(s);
        }
    }

    free(table->series);
    free(table);
    return 1;
}
