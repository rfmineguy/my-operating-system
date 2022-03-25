#include <stdint.h>
#include <stddef.h>
#include "../../intf/math.h"

int pow(int num, int n) {
    int ret = 1;
    for (int i = n; i > 0; i--) {
        ret *= num;
    }
    return ret;
}

int get_digit(int num, int n) {
    int r, t1, t2;
    t1 = pow(10, n + 1);
    r = num % t1;
    t2 = pow(10, n);
    r = r / t2;
    return r;
}

int count_digits(int num) {
    int count = 0;
    while (num != 0) {
        num /= 10;
        count++;
    }
    return count;
}
