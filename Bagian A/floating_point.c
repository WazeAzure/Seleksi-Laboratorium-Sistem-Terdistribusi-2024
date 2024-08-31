#include <stdio.h>
#include <math.h>
#include <stdint.h>

typedef union {
    float f;
    uint32_t u;
} FloatUnion;

uint32_t extractSign(uint32_t f) {
    return (f >> 31) & 0x1;
}

uint32_t extractExponent(uint32_t f) {
    return (f >> 23) & 0xFF;
}

uint32_t extractMantissa(uint32_t f) {
    return f & 0x7FFFFF;
}

uint32_t add(uint32_t x, uint32_t y){
    while(y != 0) {
        uint32_t carry = x & y;
        x = x ^ y;
        y = carry << 1;
    }
    return x;
}

uint32_t sub(uint32_t x, uint32_t y){
    return add(add(x, ~y), 1);
}

uint32_t combineFloat(uint32_t sign, uint32_t exponent, uint32_t mantissa) {
    return (sign << 31) | (exponent << 23) | mantissa;
}

float addFloats(float a, float b) {
    FloatUnion x, y;
    x.f = a;
    y.f = b;
    // Ekstraksi komponen dari floating point x
    uint32_t signX = extractSign(x.u);
    uint32_t exponentX = extractExponent(x.u);
    uint32_t mantissaX = extractMantissa(x.u) | 0x800000;  // Add implicit leading 1 for normalized values

    // Ekstraksi komponen dari floating point y
    uint32_t signY = extractSign(y.u);
    uint32_t exponentY = extractExponent(y.u);
    uint32_t mantissaY = extractMantissa(y.u) | 0x800000;  // Add implicit leading 1 for normalized values

    // special values
    if((exponentX == 0xFF && mantissaX != 0) || (exponentY == 0xFF && mantissaY != 0)){
		uint32_t nan = 0x7FC00000;
		return *(float*)&nan;
	}

	if(exponentX == 0xFF && mantissaX == 0){
		if(exponentY == 0xFF && mantissaY == 0 && signX != signY){
			uint32_t nan = 0x7FC00000;
			return *(float*)&nan;
		}
		return x.f;
	}

	if(exponentY == 0xFF && mantissaY == 0){
		return y.f;
	}

    // Normalisasi eksponen
    if (exponentX > exponentY) {
        int shift = sub(exponentX, exponentY);
        mantissaY >>= shift;
        exponentY = exponentX;
    } else {
        int shift = sub(exponentY, exponentX);
        mantissaX >>= shift;
        exponentX = exponentY;
    }

    // Penjumlahan mantissa dengan mempertimbangkan tanda
    uint32_t mantissaResult;
    uint32_t resultSign;
    if (signX == signY) {
        mantissaResult = add(mantissaX, mantissaY);
        resultSign = signX;
    } else {
        if (mantissaX > mantissaY) {
            mantissaResult = sub(mantissaX, mantissaY);
            resultSign = signX;
        } else {
            mantissaResult = sub(mantissaY, mantissaX);
            resultSign = signY;
        }
    }

    // Normalisasi hasil mantissa
    if (mantissaResult & 0x1000000) { // Check if bit 24 is set
        mantissaResult >>= 1;
        exponentX = add(exponentX, 1);
    }

    // Buang leading 1
    mantissaResult &= 0x7FFFFF;

    // Gabungkan kembali hasilnya
    FloatUnion result;
    result.u = combineFloat(resultSign, exponentX, mantissaResult);
    return result.f;
}

int main() {
    FloatUnion a, b, result;

    printf("Sum of %.2f and %.2f is %.2f\n", a.f, b.f, result.f);
    printf("83.234f + 47.001f = %f, expected = %f\n", addFloats(83.234f, 47.001f), 83.234f + 47.001f);
    printf("-120.5f + 32.789f = %f, expected = %f\n", addFloats(-120.5f, 32.789f), -120.5f + 32.789f);
    printf("0.001f + -0.001f = %f, expected = %f\n", addFloats(0.001f, -0.001f), 0.001f + -0.001f);
    printf("999.999f + 0.001f = %f, expected = %f\n", addFloats(999.999f, 0.001f), 999.999f + 0.001f);
    printf("-83.234f + -47.001f = %f, expected = %f\n", addFloats(-83.234f, -47.001f), -83.234f + -47.001f);
    printf("NAN + 47.001f = %f, expected = %f\n", addFloats(NAN, 47.001f), NAN + 47.001f);
    printf("INFINITY + -120.5f = %f, expected = %f\n", addFloats(INFINITY, -120.5f), INFINITY + -120.5f);
    printf("-INFINITY + INFINITY = %f, expected = %f\n", addFloats(-INFINITY, INFINITY), -INFINITY + INFINITY);
    printf("0.0f + -0.0f = %f, expected = %f\n", addFloats(0.0f, -0.0f), 0.0f + -0.0f);
    printf("1.2345f + 2.3456f = %f, expected = %f\n", addFloats(1.2345f, 2.3456f), 1.2345f + 2.3456f);
    printf("-1.2345f + -2.3456f = %f, expected = %f\n", addFloats(-1.2345f, -2.3456f), -1.2345f + -2.3456f);
    printf("12345.6789f + 98765.4321f = %f, expected = %f\n", addFloats(12345.6789f, 98765.4321f), 12345.6789f + 98765.4321f);
    printf("-12345.6789f + 98765.4321f = %f, expected = %f\n", addFloats(-12345.6789f, 98765.4321f), -12345.6789f + 98765.4321f);
    printf("0.0001f + 0.0002f = %f, expected = %f\n", addFloats(0.0001f, 0.0002f), 0.0001f + 0.0002f);
    printf("3.14159f + -3.14159f = %f, expected = %f\n", addFloats(3.14159f, -3.14159f), 3.14159f + -3.14159f);
    return 0;
}
