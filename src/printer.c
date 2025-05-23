#include "../include/setter.h"

int print_table(const table_t* table, const char* table_name) {
    if (!table) {
        fprintf(stderr, "Table is NULL.\n");
        return 0;
    }

    if (table->series_count < 1) {
        fprintf(stdout, "Table is empty.\n");
        return 1;
    }

    fprintf(stdout, "+------------------------------------------------------+\n");
    fprintf(stdout, "\t%s\t\n", table_name);
    fprintf(stdout, "+---------+------------+-------------------------------+\n");
    fprintf(stdout, "| Series# | Gradation  |            Values             |\n");
    fprintf(stdout, "+---------+------------+-------------------------------+\n");

    for (int i = 0; i < table->series_count; ++i) {
        series_t* s = table->series[i];
        if (!s || s->series_count < 1) continue;

        fprintf(stdout, "| %7d | %10.4f |", i, s->gradation);
        for (int j = 0; j < s->series_count; ++j) {
            if (j > 0) fprintf(stdout, " ");
            fprintf(stdout, "%.3f", s->series[j]);
        }

        fprintf(stdout, "\n");
    }

    fprintf(stdout, "+---------+------------+-------------------------------+\n");
    return 1;
}

int save_table(FILE* fp, const table_t* table) {
    if (!table) return 0;
    if (table->series_count < 1) return 0;
    
    for (int i = 0; i < table->series_count; ++i) {
        series_t* s = table->series[i];
        if (!s || s->series_count < 1) continue;

        fprintf(fp, "Variant: %7d -----> ", i);
        for (int j = 0; j < s->series_count; ++j) {
            if (j > 0) fprintf(fp, " ");
            fprintf(fp, "%.3f", s->series[j]);
        }

        fprintf(fp, "\n");
    }

    return 1;
}
