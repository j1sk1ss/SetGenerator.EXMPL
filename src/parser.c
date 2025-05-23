#include "../include/parser.h"

params_t* parse_input(char* argv[], int argc) {
    int grads_start = 0;
    int grads_count = 0;
    double min = INT_MIN, max = INT_MAX;
    for (int i = 0; i < argc; i++) {
        if (grads_start) grads_count++;
        if (!strcmp(argv[i], GRADATIONS)) grads_start = 1;
        else if (!strcmp(argv[i], END_EGRADS)) grads_start = 0;
        else if (!strcmp(argv[i], SERIES_MAX)) {
            if (i + 1 < argc) max = atof(argv[i + 1]);
        }
        else if (!strcmp(argv[i], SERIES_MIN)) {
            if (i + 1 < argc) min = atof(argv[i + 1]);
        }
    }

    params_t* params = (params_t*)malloc(sizeof(params_t));
    if (!params) return NULL;
    params->save_path = NULL;
    params->max = max;
    params->min = min;
    params->drags_count = grads_count;
    params->gradations = (char**)malloc(sizeof(char*) * grads_count);
    if (!params->gradations) {
        free(params);
        return NULL;
    }

    int index = 0;
    grads_start = 0;
    for (int i = 0; i < argc; i++) {
        if (!strcmp(argv[i], GRADATIONS)) grads_start = 1;
        else if (!strcmp(argv[i], END_EGRADS)) grads_start = 0;
        else if (!strcmp(argv[i], SAVE_PATH)) {
            if (i + 1 < argc) params->save_path = argv[i + 1];
        }

        if (grads_start) {
            params->gradations[index++] = argv[i];
        }
    }

    return params;
}

int free_params(params_t* params) {
    if (!params) return 0;
    free(params->gradations);
    free(params);
    return 1;
}
