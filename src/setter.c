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

typedef struct {
    double gradation;
    table_t* table;
} grad_group_t;

static table_t** _separate_table_by_grad(const table_t* t) {
    if (!t || !t->series_count) return NULL;

    int group_count = 0;
    int group_capacity = 8;
    grad_group_t* groups = (grad_group_t*)malloc(sizeof(grad_group_t) * group_capacity);
    if (!groups) {
        return NULL;
    }

    for (int i = 0; i < t->series_count; ++i) {
        series_t* s = _create_series(t->series[i]->series_count, t->series[i]->gradation);
        memcpy(s->series, t->series[i]->series, sizeof(double) * t->series[i]->series_count);

        for (int j = 0; j < group_count; ++j) {
            if (groups[j].gradation == s->gradation) {
                _add_series_to_table(groups[j].table, s);
                goto found_entry;
            }
        }

        if (group_count >= group_capacity) {
            group_capacity *= 2;
            groups = realloc(groups, group_capacity * sizeof(grad_group_t));
        }

        table_t* new_table      = (table_t*)malloc(sizeof(table_t));
        new_table->series       = (series_t**)malloc(sizeof(series_t*));
        new_table->series[0]    = s;
        new_table->series_count = 1;

        groups[group_count].gradation = s->gradation;
        groups[group_count].table     = new_table;
        group_count++;

found_entry:
    }

    table_t** result = malloc(sizeof(table_t*) * (group_count + 1));
    if (result) {
        for (int i = 0; i < group_count; ++i) result[i] = groups[i].table;
        result[group_count] = NULL;
    }

    free(groups);
    return result;
}

table_t* generate_sets(const table_t* possible_series) {
    table_t* table = (table_t*)malloc(sizeof(table_t));
    if (!table) return NULL;

    table_t** separated_grads = _separate_table_by_grad(possible_series);
    if (!separated_grads) {
        free(table);
        return NULL;
    }

    int grad_table_count = 0;
    int total_combinations = 1;
    for (int i = 0; separated_grads[i]; i++) {
        total_combinations *= separated_grads[i]->series_count;
        grad_table_count++;
    }

    table_t* result = malloc(sizeof(table_t));
    if (!result) {
        free(table);
        for (int i = 0; separated_grads[i]; i++) free_table(separated_grads[i]);
        free(separated_grads);
        return NULL;
    }

    result->series_count = total_combinations;
    result->series = malloc(sizeof(series_t*) * total_combinations);

    int* indices = calloc(grad_table_count, sizeof(int));
    for (int combo = 0; combo < total_combinations; ++combo) {
        int total_values = 0;
        for (int i = 0; i < grad_table_count; ++i) {
            total_values += separated_grads[i]->series[indices[i]]->series_count;
        }

        series_t* new_series = _create_series(total_values, separated_grads[0]->series[indices[0]]->gradation);
        if (new_series) {
            int offset = 0;
            for (int i = 0; i < grad_table_count; ++i) {
                series_t* src = separated_grads[i]->series[indices[i]];
                memcpy(new_series->series + offset, src->series, sizeof(double) * src->series_count);
                offset += src->series_count;
            }

            result->series[combo] = new_series;
        }

        for (int i = grad_table_count - 1; i >= 0; --i) {
            indices[i]++;
            if (indices[i] < separated_grads[i]->series_count) break;
            indices[i] = 0;
        }
    }

    for (int i = 0; separated_grads[i]; i++) free_table(separated_grads[i]);
    free(separated_grads);
    free(indices);

    return result;
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
