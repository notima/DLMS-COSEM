#include "test.h"

int main() {
    int pos = 18;
    size_t size = step_over(test_data2 + pos);
    assert_int("Size of array", 560, size);

    pos += 2;

    size = step_over(test_data2 + pos);
    assert_int("Size of struct", 24, size);

    pos += size;

    size = step_over(test_data2 + pos);
    assert_int("Size of struct", 21, size);
}