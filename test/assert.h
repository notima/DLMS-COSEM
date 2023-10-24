#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void assert_int(const char* name, int expect, int real) {
    if(expect == real)
        return;

    printf("%s is incorrect (expected %d (#%x), got %d (#%x))\n", name, expect, expect, real, real);
    exit(1);
}

void assert_str(const char* name, char* expect, char* real) {
    if(strcmp(expect, real) == 0)
        return;

    printf("%s is incorrect (expected %s, got %s)\n", name, expect, real);
    exit(1);
}

void assert_double(const char* name, double expect, double real) {
    if(expect == real)
        return;

    printf("%s is incorrect (expected %f, got %f)\n", name, expect, real);
    exit(1);
}