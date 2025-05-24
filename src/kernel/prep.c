#include "../../include/setter.h"

static int _series_equal(const series_t* a, const series_t* b) {
    if (!a || !b) return 0;
    if (a->series_count != b->series_count) return 0;
    for (int i = 0; i < a->series_count; ++i) {
        if (!FLOAT_EQUAL(a->series[i], b->series[i])) return 0;
    }
    return 1;
}

int remove_duplicate_series(table_t* table) {
    if (!table || !table->series || table->series_count == 0) return 0;
    int unique_count = 0;
    for (int i = 0; i < table->series_count; ++i) {
        int is_duplicate = 0;
        for (int j = 0; j < unique_count; ++j) {
            if (_series_equal(table->series[i], table->series[j])) {
                is_duplicate = 1;
                break;
            }
        }

        if (!is_duplicate && table->series[i]->series_count > 1) table->series[unique_count++] = table->series[i];
        else {
            free(table->series[i]->series);
            free(table->series[i]);
        }
    }

    table->series_count = unique_count;
    return 1;
}

static int _double_cmp(const void* a, const void* b) {
    double da = *(const double*)a;
    double db = *(const double*)b;
    if (da < db) return -1;
    if (da > db) return 1;
    return 0;
}

int clean_and_sort_series_values(const table_t* table) {
    if (!table) return 0;
    for (int i = 0; i < table->series_count; ++i) {
        series_t* s = table->series[i];
        if (!s || s->series_count <= 1) continue;
        qsort(s->series, s->series_count, sizeof(double), _double_cmp);

        int new_count = 1;
        for (int j = 1; j < s->series_count; ++j) {
            if (!FLOAT_EQUAL(s->series[j], s->series[new_count - 1])) {
                s->series[new_count++] = s->series[j];
            }
        }

        s->series_count = new_count;
    }

    return 1;
}

int filter_series_by_range(table_t* table, double min, double max) {
    if (!table || table->series_count == 0) return 0;

    int new_count = 0;
    for (int i = 0; i < table->series_count; ++i) {
        series_t* s = table->series[i];
        int out_of_range = 0;
        for (int j = 0; j < s->series_count; ++j) {
            if (s->series[j] < min || s->series[j] > max) {
                out_of_range = 1;
                break;
            }
        }

        if (!out_of_range) table->series[new_count++] = s;
        else {
            free(s->series);
            free(s);
        }
    }

    table->series_count = new_count;
    return 1;
}
