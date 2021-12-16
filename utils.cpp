#include "utils.hpp"

int fastPowAcc(int a, int n, int acc) {
    if (n == 1)
        return acc;
    if (n % 2 == 0)
        return fastPowAcc(a, n / 2, acc * acc);
    else
        return fastPowAcc(a, n / 2, acc * acc * a);
}

int fastPow(int a, int n) {
    if (n == 0)
        return 1;
    return fastPowAcc(a, n, a);
}
