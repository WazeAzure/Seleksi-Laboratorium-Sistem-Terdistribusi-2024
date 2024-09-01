#include <stdio.h>

int compute(int a, int b) {
    int temp1 = (a << 3) + 25;
    int temp2 = (temp1 >> 31) + temp1;
    int x = temp2 >> 1;
    int y = x * b;
    int z = b + x;
    int w = ((y & 3) << z) | z;
    int result = w ^ y;

    // Print the intermediate values
    printf("%d %d\n", w, z);

    return result;
}

int main() {
    // First call to compute
    int result1 = compute(69, 42);
    float float_result1 = (float)result1;

    // Second call to compute
    int result2 = compute(42, 69);
    float float_result2 = (float)result2;

    // Add the two floating point results
    float final_float = float_result1 + float_result2;

    // Print the floating point results as double precision
    printf("%f %f\n", (double)float_result1, (double)float_result2);

    return 0;
}

