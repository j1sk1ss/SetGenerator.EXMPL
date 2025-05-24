#include "../../include/setter.h"

int print_table(const table_t* table, const char* table_name, int highlight) {
    if (!table) {
        fprintfl(stderr, "Table is NULL.\n");
        return 0;
    }

    if (table->series_count < 1) {
        fprintfl(stdout, "Table is empty.\n");
        return 1;
    }

    fprintfl(stdout, "+------------------------------------------------+\n");
    fprintfl(stdout, "\t%s\t\n", table_name);
    fprintfl(stdout, "+------+---------+-------------------------------+\n");
    fprintfl(stdout, "| Num  | Grad-on |            Values             |\n");
    fprintfl(stdout, "+------+---------+-------------------------------+\n");

    for (int i = 0; i < table->series_count; ++i) {
        series_t* s = table->series[i];
        if (!s || s->series_count < 1) continue;

        if (highlight != i) fprintfl(stdout, "| %4d | %7.3f |", i, s->gradation);
        else fprintfl(stdout, "> %4d | %7.3f |", i, s->gradation);
        for (int j = 0; j < s->series_count; ++j) {
            if (j > 0) fprintfl(stdout, " ");
            fprintfl(stdout, "%.3f", s->series[j]);
        }

        fprintfl(stdout, "\n");
    }

    fprintfl(stdout, "+------+---------+-------------------------------+\n");
    return 1;
}
