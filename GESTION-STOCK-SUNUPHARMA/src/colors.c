#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "colors.h"
#include "users.h"
#include "products.h"
#include "categories.h"
#include "sales.h"
#include "rapports.h"

void color_error(const char* msg) {
    printf("\033[1;31m%s\033[0m", msg);
}

void color_success(const char* msg) {
    printf("\033[1;32m%s\033[0m", msg);
}

void color_title(const char* msg) {
    printf("\033[1;34m%s\033[0m", msg);
}

void color_header(const char* msg) {
    printf("\033[1;36m%s\033[0m", msg);
}
