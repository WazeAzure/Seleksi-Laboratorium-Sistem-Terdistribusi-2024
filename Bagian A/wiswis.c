#include <stdio.h>
#include <stdint.h>
#include <math.h>

int a(int x, int y){
	while (y != 0) {
		uint32_t carry = x & y;
		x = x ^ y;
		y = carry << 1;
	}
	return x;
}

uint32_t adduint32(uint32_t x, uint32_t y){
	while (y != 0) {
		uint32_t carry = x & y;
		x = x ^ y;
		y = carry << 1;
	}
	return x;
}

uint32_t subuint32(uint32_t x, uint32_t y){
	return adduint32(adduint32(x, ~y), 1);
}

float b(float x, float y){
	// AKU UDAH BUAT YANG UBAH FLOAT JADI INTEGER DI PRAK ARSIKOM, PLEASE JUST LET ME USE POINTERS
	uint32_t ux = *(uint32_t*)&x;
	uint32_t uy = *(uint32_t*)&y;

	uint32_t signx = ux >> 31;
	uint32_t exponentx = (ux >> 23) & 0xFF;
	uint32_t mantissax = (ux & 0x7FFFFF) | 0x800000;

	uint32_t signy = uy >> 31;
	uint32_t exponenty = (uy >> 23) & 0xFF;
	uint32_t mantissay = (uy & 0x7FFFFF) | 0x800000;

	if((exponentx == 0xFF && mantissax != 0) || (exponenty == 0xFF && mantissay != 0)){
		uint32_t nan = 0x7FC00000;
		return *(float*)&nan;
	}

	if(exponentx == 0xFF && mantissax == 0){
		if(exponenty == 0xFF && mantissay == 0 && signx != signy){
			uint32_t nan = 0x7FC00000;
			return *(float*)&nan;
		}
		return x;
	}

	if(exponenty == 0xFF && mantissay == 0){
		return y;
	}

	if(exponentx == 0){
		mantissax = mantissax & 0x7FFFFF;
	}

	if(exponenty == 0){
		mantissay = mantissay & 0x7FFFFF;
	}

	if(exponentx > exponenty){
		uint32_t exponent_diff = subuint32(exponentx, exponenty);
		mantissay = mantissay >> exponent_diff;
		exponenty = adduint32(exponenty, exponent_diff);
	}
	else if(exponenty > exponentx){
		uint32_t exponent_diff = subuint32(exponenty, exponentx);
		mantissax = mantissax >> exponent_diff;
		exponentx = adduint32(exponentx, exponent_diff);
	}

	uint32_t mantissa, exponent, sign;
	if(signx == signy){
		mantissa = adduint32(mantissax, mantissay);
		sign = signx;
	}
	else{
		if(mantissax > mantissay){
			mantissa = subuint32(mantissax, mantissay);
			sign = signx;
		}
		else{
			mantissa = subuint32(mantissay, mantissax);
			sign = signy;
		}
	}

	exponent = exponentx;
	while(mantissa >= (1 << 24)){
		mantissa = mantissa >> 1;
		exponent = adduint32(exponent, 1);
	}
	while((mantissa < (1 << 23)) && exponent > 0){
		mantissa = mantissa << 1;
		exponent = subuint32(exponent, 1);
	}

	mantissa = mantissa & 0x7FFFFF;

	if(exponent < 1){
		exponent = 0;
		mantissa = mantissa >> 1;
	}

	if(exponent >= 255){
		exponent = 255;
		mantissa = 0;
	}

	uint32_t result = (sign << 31) | ((exponent & 0xFF) << 23) | (mantissa & 0x7FFFFF);

	if(result == 0x80000000){
		result = 0;
	}

	float result_float = *(float*)&result;

	return result_float;
}

int main(void){
	printf("83.234f + 47.001f = %f, expected = %f\n", b(83.234f, 47.001f), 83.234f + 47.001f);
	printf("-120.5f + 32.789f = %f, expected = %f\n", b(-120.5f, 32.789f), -120.5f + 32.789f);
	printf("0.001f + -0.001f = %f, expected = %f\n", b(0.001f, -0.001f), 0.001f + -0.001f);
	printf("999.999f + 0.001f = %f, expected = %f\n", b(999.999f, 0.001f), 999.999f + 0.001f);
	printf("-83.234f + -47.001f = %f, expected = %f\n", b(-83.234f, -47.001f), -83.234f + -47.001f);
	printf("NAN + 47.001f = %f, expected = %f\n", b(NAN, 47.001f), NAN + 47.001f);
	printf("INFINITY + -120.5f = %f, expected = %f\n", b(INFINITY, -120.5f), INFINITY + -120.5f);
	printf("-INFINITY + INFINITY = %f, expected = %f\n", b(-INFINITY, INFINITY), -INFINITY + INFINITY);
	printf("0.0f + -0.0f = %f, expected = %f\n", b(0.0f, -0.0f), 0.0f + -0.0f);
	printf("1.2345f + 2.3456f = %f, expected = %f\n", b(1.2345f, 2.3456f), 1.2345f + 2.3456f);
	printf("-1.2345f + -2.3456f = %f, expected = %f\n", b(-1.2345f, -2.3456f), -1.2345f + -2.3456f);
	printf("12345.6789f + 98765.4321f = %f, expected = %f\n", b(12345.6789f, 98765.4321f), 12345.6789f + 98765.4321f);
	printf("-12345.6789f + 98765.4321f = %f, expected = %f\n", b(-12345.6789f, 98765.4321f), -12345.6789f + 98765.4321f);
	printf("0.0001f + 0.0002f = %f, expected = %f\n", b(0.0001f, 0.0002f), 0.0001f + 0.0002f);
	printf("3.14159f + -3.14159f = %f, expected = %f\n", b(3.14159f, -3.14159f), 3.14159f + -3.14159f);
}