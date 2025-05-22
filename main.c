#include "include/setter.h"

static series_t _series[] = {
    { .gradation = 0.001, .series = (double[]){ 1, 1.001, 1.002, 1.003, 1.004, 1.005, 1.006, 1.007, 1.008, 1.009, 1.01 }, .series_count = 11 },
    { .gradation = 0.005, .series = (double[]){ 1, 1.005, 1.01 }, .series_count = 3 },
    { .gradation = 0.01, .series = (double[]){ 
        1, 1.01, 1.02, 1.03, 1.04, 1.05, 1.06, 1.07, 1.08, 1.09, 1.1, 1.11, 1.12, 1.13, 1.14, 1.15, 1.16, 1.17, 1.18, 1.19, 1.2, 
        1.21, 1.22, 1.23, 1.24, 1.25, 1.26, 1.27, 1.28, 1.29, 1.3, 1.31, 1.32, 1.33, 1.34, 1.35, 1.36, 1.37, 1.38, 1.39, 1.4, 1.41, 
        1.42, 1.43, 1.44, 1.45, 1.46, 1.47, 1.48, 1.49, 1.5 
    }, .series_count = 51 },
    { .gradation = 0.1, .series = (double[]){ 1, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9, 3 }, .series_count = 21 },
    { .gradation = 0.5, .series = (double[]){ 
        0.5, 1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5, 5, 5.5, 6, 6.5, 7, 7.5, 8, 8.5, 9, 9.5, 10, 10.5, 
        11, 11.5, 12, 12.5, 13, 13.5, 14, 14.5, 15, 15.5, 16, 16.5, 17, 17.5, 18, 18.5, 19, 19.5, 
        20, 20.5, 21, 21.5, 22, 22.5, 23, 23.5, 24, 24.5, 25   
    }, .series_count = 50 },
    { .gradation = 1, .series = (double[]){ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25 }, .series_count = 25 },
    { .gradation = 5, .series = (double[]){ 5, 10, 15, 20, 25, 30 }, .series_count = 6 },
    { .gradation = 10, .series = (double[]){ 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 }, .series_count = 10 },
    { .gradation = 25, .series = (double[]){ 25, 50, 75, 100 }, .series_count = 4 },
    { .gradation = -1, .series = NULL, .series_count = -1 }
};

/*
Input data example:
0.001 0.005 0.01 ...
*/
int main(int argc, char* argv[]) {
    series_t** considering_series = (series_t**)malloc(sizeof(series_t*) * (argc - 1));
    if (!considering_series) {
        fprintf(stderr, "malloc() error!");
        exit(EXIT_FAILURE);
    }

    int series_index = 0;
    for (int i = 1; i < argc; i++) {
        double gradation = atof(argv[i]);
        // fprintf(stdout, "Considered series gradation: %f\n", gradation);

        for (int j = 0; _series[j].series; j++) {
            if (FLOAT_EQUAL(_series[j].gradation, gradation)) {
                series_t* curr = (series_t*)malloc(sizeof(series_t)); 
                if (!curr) {
                    fprintf(stderr, "malloc() error!");
                    goto free_cons_ser;
                }

                memcpy(curr, &_series[j], sizeof(series_t));
                considering_series[series_index++] = curr;
                break;
            }
        }
    }

    table_t* answer = generate_series((const series_t**)considering_series, series_index);
    if (answer) {
        print_table(answer, "             Possible series                        ");
        table_t* sets = generate_sets(answer);
        if (sets) {
            clean_and_sort_series_values(sets);
            remove_duplicate_series(sets);
            print_table(sets, "                Sets                                ");
            free_table(sets);
        }

        free_table(answer);
    }
    else {
        fprintf(stderr, "Table is NULL!");
    }

free_cons_ser:
    for (int i = 0; i < argc - 1; i++) {
        if (considering_series[i]) free(considering_series[i]);
    }

    free(considering_series);
    return EXIT_SUCCESS;
}