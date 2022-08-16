#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void assert_int(const char* name, int expect, int real) {
    if(expect == real)
        return;

    printf("%s is incorrect (expected %d, got %d)", name, expect, real);
    exit(1);
}

void assert_str(const char* name, char* expect, char* real) {
    if(strcmp(expect, real) == 0)
        return;

    printf("%s is incorrect (expected %s, got %s)", name, expect, real);
    exit(1);
}

void assert_double(const char* name, double expect, double real) {
    if(expect == real)
        return;

    printf("%s is incorrect (expected %f, got %f)", name, expect, real);
    exit(1);
}