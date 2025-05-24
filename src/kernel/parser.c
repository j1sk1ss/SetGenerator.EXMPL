#include "../../include/parser.h"

int free_params(params_t* params) {
    if (!params) return 0;
    free(params->gradations);
    free(params);
    return 1;
}
