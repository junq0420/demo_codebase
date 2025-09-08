#include <stdio.h>
#include <stdint.h>

typedef union {
	uint32_t u;
	float f;
} float_bits_t;

int main(void) {
	float_bits_t x;
	x.f = 50.0f;

	float tf = 50.0f;
	uint8_t *ptr_f = (uint8_t *)&tf;

	printf("x: 0x%08X\n", x.u);
	printf("y: 0x%02X%02X%02X%02X\n", *(ptr_f+3), *(ptr_f+2), *(ptr_f+1), *(ptr_f));
	
	return 0;
}
