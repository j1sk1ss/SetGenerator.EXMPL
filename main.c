#include "include/setter.h"
#include "include/graphics.h"

static series_t _default_series[] = {
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

static series_t** _load_base_series(FILE* fp) {
    int series_count = 0;
    if (fread(&series_count, sizeof(int), 1, fp) != sizeof(int)) {
        fprintf(stderr, "fread() error during series count read!\n");
    }

    series_t** answer = (series_t**)malloc(sizeof(series_t*) * (series_count + 1));
    if (!answer) return NULL;
    memset(answer, 0, sizeof(series_t*) * series_count);
    
    for (int i = 0; i < series_count; i++) {
        series_t* series = (series_t*)malloc(sizeof(series_t));
        if (!series) goto series_cleanup;

        double gradation = 0;
        int series_size  = 0;
        fread(&gradation, sizeof(double), 1, fp);
        fread(&series_size, sizeof(int), 1, fp);

        series->series = (double*)malloc(sizeof(double) * series_size);
        fread(series->series, sizeof(double), series_size, fp);
        answer[i] = series;
    }

    answer[series_count] = (series_t*)malloc(sizeof(series_t));
    answer[series_count]->series = NULL;
    return answer;
series_cleanup:
    for (int i = 0; i < series_count; i++) {
        if (answer[i]) free(answer[i]);
    }

    free(answer);
    return NULL;
}

static series_t** __convert_static_series_to_ptr_array__(series_t* source_array, size_t* out_count) {
    if (!source_array || !out_count)
        return NULL;

    size_t count = 0;
    while (source_array[count].series_count != -1)
        count++;

    series_t** result = malloc(sizeof(series_t*) * (count + 1));
    if (!result) return NULL;

    for (size_t i = 0; i < count; ++i)
        result[i] = &source_array[i];

    result[count] = NULL;
    *out_count = count;
    return result;
}


/*
Input data example:
0.001 0.005 0.01 ...
*/
int main(int argc, char* argv[]) {
    clear_screen();
    point_t screen_sizes;
    get_console_size(&screen_sizes);
    
    print_center("WELCOME!", 0);
    print_center("PRESS <ENTER> BUTTON TO CONTINUE...", 1);

    int main_series_size = 0;
    series_t** main_series = NULL;

    wait_key('\n');
    clear_screen();

    print_center("INIT SETTINGS:", -1);
    print_center("[1] USE DEFAULT LAYOUT", 0);
    print_center("[2] USE FILE LAYOUT", 1);

    while (1) {
        char option = wait_any_key();
        switch (option) {
            case '1': 
                main_series = __convert_static_series_to_ptr_array__(_default_series, (size_t*)&main_series_size);
                goto main_prog;
            break;
            case '2':
                gotoxy(0, screen_sizes.y - 1);
                fprintfl(stdout, "PATH (max. 128): ");
                char path[128] = { 0 };
                fgets(path, sizeof(path), stdin);

                FILE* fp = fopen(path, "rb");
                if (fp) {
                    main_series = _load_base_series(fp);
                    fclose(fp);
                }
                else {
                    fprintfl(stderr, "fopen() error! File nfound!");
                    exit(EXIT_FAILURE);
                }

                if (!main_series) {
                    fprintfl(stderr, "_load_base_series() error!");
                    exit(EXIT_FAILURE);
                }
                goto main_prog;
            break;
            default: continue;
        }
    }

main_prog:
    table_t* source_data = (table_t*)malloc(sizeof(table_t));
    if (!source_data) return EXIT_FAILURE;
    source_data->series         = main_series;
    source_data->series_count   = main_series_size;

    int line = 0;
    clear_screen();
    print_center("USED GRADS AND SOURCE. PRESS <ENTER> TO CONTINUE\n", -(screen_sizes.y / 2));
    print_table(source_data, "AVAILABLE GRADATIONS", line);
    print_center("[w] - Up, [s] - Down, [space] - Select gradation\n", (screen_sizes.y / 2) - 1);
    print_center("[[] - Set min, []] - Set max\n", (screen_sizes.y / 2));

    params_t* params = (params_t*)malloc(sizeof(params_t));
    if (!params) return EXIT_FAILURE;
    params->gradations = NULL;
    params->min = INT_MIN;
    params->max = INT_MAX;

    char option = 0;
    while ((option = wait_any_key()) != '\n') {
        clear_screen();
        if (option == 'w')      line = MAX(line - 1, 0);
        else if (option == 's') line = MIN(line + 1, main_series_size);
        else if (option == ' ') {
            int presented = 0;
            for (int i = 0; i < params->grads_count; i++) {
                if (params->gradations[i] == main_series[line]->gradation) {
                    presented = 1;
                    break;
                }
            }

            if (!presented) {
                params->gradations = (double*)realloc(params->gradations, sizeof(double) * (params->grads_count + 1));
                params->gradations[params->grads_count++] = main_series[line]->gradation;
            }
        }
        else if (option == '[') {
            gotoxy(0, screen_sizes.y - 1);
            fprintfl(stdout, "Min value: ");
            char val[128] = { 0 };
            fgets(val, sizeof(val), stdin);
            params->min = atof(val);
        }
        else if (option == ']') {
            gotoxy(0, screen_sizes.y - 1);
            fprintfl(stdout, "Max value: ");
            char val[128] = { 0 };
            fgets(val, sizeof(val), stdin);
            params->max = atof(val);
        }

        print_center("USED GRADS AND SOURCE. PRESS <ENTER> TO CONTINUE\n", -(screen_sizes.y / 2));
        print_table(source_data, "AVAILABLE GRADATIONS", line);
        print_center("[w] - Up, [s] - Down, [space] - Select gradation\n", (screen_sizes.y / 2) - 1);
        print_center("[[] - Set min, []] - Set max\n", (screen_sizes.y / 2));
        for (int i = 0; i < params->grads_count; i++) fprintfl(stdout, "%.3f\t", params->gradations[i]);
    }

    free(source_data);

    series_t** considering_series = (series_t**)malloc(sizeof(series_t*) * (params->grads_count));
    if (!considering_series) goto free_cons_ser;

    int series_index = 0;
    for (int i = 0; i < params->grads_count; i++) {
        for (int j = 0; main_series[j]->series; j++) {
            if (FLOAT_EQUAL(main_series[j]->gradation, params->gradations[i])) {
                series_t* curr = (series_t*)malloc(sizeof(series_t)); 
                if (!curr) {
                    fprintfl(stderr, "malloc() error!");
                    goto free_cons_ser;
                }

                memcpy(curr, main_series[j], sizeof(series_t));
                considering_series[series_index++] = curr;
                break;
            }
        }
    }

    table_t* answer = generate_series((const series_t**)considering_series, series_index);
    if (!answer) goto free_cons_ser;

    clear_screen();
    print_center("GENERATED SERIES. PRESS <ENTER> TO CONTINUE.\n", -(screen_sizes.y / 2));
    clean_and_sort_series_values(answer);
    remove_duplicate_series(answer);
    print_table(answer, "GENERATED SERIES", -1);
    wait_key('\n');
    clear_screen();
    
    table_t* sets = generate_sets(answer);
    if (!sets) {
        free_table(answer);
        goto free_cons_ser;
    }

    clean_and_sort_series_values(sets);
    remove_duplicate_series(sets);
    filter_series_by_range(sets, params->min, params->max);
    
    clear_screen();
    print_center("POSSIBLE SETS. PRESS <ENTER> TO EXIT AND SAVE. <q> TO EXIT.\n", -(screen_sizes.y / 2));
    print_table(sets, "GENERATED SETS", -1);
    
    while (1) {
        char exit_option = wait_any_key();
        if (exit_option == '\n') {
            gotoxy(0, screen_sizes.y - 1);
            fprintfl(stdout, "Save path (default: tmp_answer.txt): ");
            char save_path[128] = { 0 };
            sprintf(save_path, "tmp_answer.txt");

            fgets(save_path, sizeof(save_path), stdin);
            FILE* fp = fopen(save_path, "w");
            if (fp) {
                save_table(fp, sets);
                fclose(fp);
            }
        }
        else if (exit_option == 'q') { }
        else continue;

        free_table(sets);
        free_table(answer);
        break;
    }

free_cons_ser:
    if (main_series) {
        for (int i = 0; i < main_series_size; i++) {
            if (main_series[i]) free(main_series[i]);
        }

        free(main_series);
    }

    if (considering_series) {
        for (int i = 0; i < argc - 1; i++) {
            if (considering_series[i]) free(considering_series[i]);
        }

        free(considering_series);
    }

    free_params(params);
    return EXIT_SUCCESS;
}