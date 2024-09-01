#include <stdio.h>

int compute(int a, int b) {
    int result1 = ((a << 3) + 25 + ((a << 3) + 25 >> 31)) >> 1;
    int result2 = result1 * b;
    int result3 = result1 + b;
    int result4 = (result3 << (result2 & 3)) | result3;
    int final_result = result4 ^ result2;
    printf("%d %d\n", result3, final_result);
    return final_result;
}

int main() {
    float res1 = (float)compute(69, 42);
    float res2 = (float)compute(42, 69);
    double final_res = (double)(res1 + res2);
    printf("%f %f\n", (double)res1, (double)res2);
    return 0;
}
