// Mengasumsikan `a` dan `b` adalah bilangan desimal titik mengambang IEEE 754 single precision (32 bit)
#include <stdio.h>
#include <math.h>
#include <stdint.h>

float add_floats_bitwise(float a, float b) {
  // Ekstraksi bit representasi IEEE 754
  unsigned int a_bits = *((unsigned int*)&a);
  unsigned int b_bits = *((unsigned int*)&b);

  // Memisahkan sign bit
  unsigned int a_sign = a_bits >> 31;
  unsigned int b_sign = b_bits >> 31;

  // Menggabungkan eksponen
  unsigned int exp_bits = (a_bits & 0x7F800000) | (b_bits & 0x7F800000);

  // Menambahkan mantissa
  unsigned int mant_a = a_bits & 0x007FFFFF;
  unsigned int mant_b = b_bits & 0x007FFFFF;
  unsigned int mant_sum = mant_a + mant_b;

  // Meminjam dari eksponen jika perlu
  if (mant_sum >= 0x00800000) {
    mant_sum >>= 1;
    exp_bits++;
  }

  // Menggabungkan sign bit
  unsigned int result_bits = exp_bits | (a_sign << 31) | (mant_sum & 0x007FFFFF);

  // Mengembalikan hasil sebagai bilangan desimal titik mengambang
  float result = *((float*)&result_bits);

  return result;
}

int main(){
    printf("%f\n", add_floats_bitwise(0.2, 0.5));

    // printf("Sum of %.2f and %.2f is %.2f\n", a.f, b.f, result.f);
    printf("83.234f + 47.001f = %f, expected = %f\n", add_floats_bitwise(83.234f, 47.001f), 83.234f + 47.001f);
    printf("-120.5f + 32.789f = %f, expected = %f\n", add_floats_bitwise(-120.5f, 32.789f), -120.5f + 32.789f);
    printf("0.001f + -0.001f = %f, expected = %f\n", add_floats_bitwise(0.001f, -0.001f), 0.001f + -0.001f);
    printf("999.999f + 0.001f = %f, expected = %f\n", add_floats_bitwise(999.999f, 0.001f), 999.999f + 0.001f);
    printf("-83.234f + -47.001f = %f, expected = %f\n", add_floats_bitwise(-83.234f, -47.001f), -83.234f + -47.001f);
    printf("NAN + 47.001f = %f, expected = %f\n", add_floats_bitwise(NAN, 47.001f), NAN + 47.001f);
    printf("INFINITY + -120.5f = %f, expected = %f\n", add_floats_bitwise(INFINITY, -120.5f), INFINITY + -120.5f);
    printf("-INFINITY + INFINITY = %f, expected = %f\n", add_floats_bitwise(-INFINITY, INFINITY), -INFINITY + INFINITY);
    printf("0.0f + -0.0f = %f, expected = %f\n", add_floats_bitwise(0.0f, -0.0f), 0.0f + -0.0f);
    printf("1.2345f + 2.3456f = %f, expected = %f\n", add_floats_bitwise(1.2345f, 2.3456f), 1.2345f + 2.3456f);
    printf("-1.2345f + -2.3456f = %f, expected = %f\n", add_floats_bitwise(-1.2345f, -2.3456f), -1.2345f + -2.3456f);
    printf("12345.6789f + 98765.4321f = %f, expected = %f\n", add_floats_bitwise(12345.6789f, 98765.4321f), 12345.6789f + 98765.4321f);
    printf("-12345.6789f + 98765.4321f = %f, expected = %f\n", add_floats_bitwise(-12345.6789f, 98765.4321f), -12345.6789f + 98765.4321f);
    printf("0.0001f + 0.0002f = %f, expected = %f\n", add_floats_bitwise(0.0001f, 0.0002f), 0.0001f + 0.0002f);
    printf("3.14159f + -3.14159f = %f, expected = %f\n", add_floats_bitwise(3.14159f, -3.14159f), 3.14159f + -3.14159f);
    return 0;
}