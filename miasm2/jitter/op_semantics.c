#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <math.h>
#include "op_semantics.h"

const uint8_t parity_table[256] = {
    CC_P, 0, 0, CC_P, 0, CC_P, CC_P, 0,
    0, CC_P, CC_P, 0, CC_P, 0, 0, CC_P,
    0, CC_P, CC_P, 0, CC_P, 0, 0, CC_P,
    CC_P, 0, 0, CC_P, 0, CC_P, CC_P, 0,
    0, CC_P, CC_P, 0, CC_P, 0, 0, CC_P,
    CC_P, 0, 0, CC_P, 0, CC_P, CC_P, 0,
    CC_P, 0, 0, CC_P, 0, CC_P, CC_P, 0,
    0, CC_P, CC_P, 0, CC_P, 0, 0, CC_P,
    0, CC_P, CC_P, 0, CC_P, 0, 0, CC_P,
    CC_P, 0, 0, CC_P, 0, CC_P, CC_P, 0,
    CC_P, 0, 0, CC_P, 0, CC_P, CC_P, 0,
    0, CC_P, CC_P, 0, CC_P, 0, 0, CC_P,
    CC_P, 0, 0, CC_P, 0, CC_P, CC_P, 0,
    0, CC_P, CC_P, 0, CC_P, 0, 0, CC_P,
    0, CC_P, CC_P, 0, CC_P, 0, 0, CC_P,
    CC_P, 0, 0, CC_P, 0, CC_P, CC_P, 0,
    0, CC_P, CC_P, 0, CC_P, 0, 0, CC_P,
    CC_P, 0, 0, CC_P, 0, CC_P, CC_P, 0,
    CC_P, 0, 0, CC_P, 0, CC_P, CC_P, 0,
    0, CC_P, CC_P, 0, CC_P, 0, 0, CC_P,
    CC_P, 0, 0, CC_P, 0, CC_P, CC_P, 0,
    0, CC_P, CC_P, 0, CC_P, 0, 0, CC_P,
    0, CC_P, CC_P, 0, CC_P, 0, 0, CC_P,
    CC_P, 0, 0, CC_P, 0, CC_P, CC_P, 0,
    CC_P, 0, 0, CC_P, 0, CC_P, CC_P, 0,
    0, CC_P, CC_P, 0, CC_P, 0, 0, CC_P,
    0, CC_P, CC_P, 0, CC_P, 0, 0, CC_P,
    CC_P, 0, 0, CC_P, 0, CC_P, CC_P, 0,
    0, CC_P, CC_P, 0, CC_P, 0, 0, CC_P,
    CC_P, 0, 0, CC_P, 0, CC_P, CC_P, 0,
    CC_P, 0, 0, CC_P, 0, CC_P, CC_P, 0,
    0, CC_P, CC_P, 0, CC_P, 0, 0, CC_P,
};

uint16_t bcdadd_16(uint16_t a, uint16_t b)
{
	int carry = 0;
	int i,j = 0;
	uint16_t res = 0;
	int nib_a, nib_b;
	for (i = 0; i < 16; i += 4) {
		nib_a = (a  >> i) & (0xF);
		nib_b = (b >> i) & (0xF);

		j = (carry + nib_a + nib_b);
		if (j >= 10) {
			carry = 1;
			j -= 10;
			j &=0xf;
		}
		else {
			carry = 0;
		}
		res += j << i;
	}
	return res;
}

uint16_t bcdadd_cf_16(uint16_t a, uint16_t b)
{
	int carry = 0;
	int i,j = 0;
	int nib_a, nib_b;
	for (i = 0; i < 16; i += 4) {
		nib_a = (a >> i) & (0xF);
		nib_b = (b >> i) & (0xF);

		j = (carry + nib_a + nib_b);
		if (j >= 10) {
			carry = 1;
			j -= 10;
			j &=0xf;
		}
		else {
			carry = 0;
		}
	}
	return carry;
}

unsigned int mul_lo_op(unsigned int size, unsigned int a, unsigned int b)
{
	unsigned int mask;

	switch (size) {
		case 8: mask = 0xff; break;
		case 16: mask = 0xffff; break;
		case 32: mask = 0xffffffff; break;
		default: fprintf(stderr, "inv size in mul %d\n", size); exit(EXIT_FAILURE);
	}

	a &= mask;
	b &= mask;
	return ((int64_t)a * (int64_t) b) & mask;
}

unsigned int mul_hi_op(unsigned int size, unsigned int a, unsigned int b)
{
	uint64_t res = 0;
	unsigned int mask;

	switch (size) {
		case 8: mask = 0xff; break;
		case 16: mask = 0xffff; break;
		case 32: mask = 0xffffffff; break;
		default: fprintf(stderr, "inv size in mul %d\n", size); exit(EXIT_FAILURE);
	}

	a &= mask;
	b &= mask;
	res = ((uint64_t)a * (uint64_t)b);
	return (res >> 32) & mask;
}


unsigned int imul_lo_op_08(char a, char b)
{
	return a*b;
}

unsigned int imul_lo_op_16(short a, short b)
{
	return a*b;
}

unsigned int imul_lo_op_32(int a, int b)
{
	return a*b;
}

int imul_hi_op_08(char a, char b)
{
	int64_t res = 0;
	res = a*b;
	return res>>8;
}

int imul_hi_op_16(short a, short b)
{
	int64_t res = 0;
	res = a*b;
	return res>>16;
}

int imul_hi_op_32(int a, int b)
{
	int64_t res = 0;
	res = (int64_t)a*(int64_t)b;
	return res>>32ULL;
}

unsigned int umul16_lo(unsigned short a, unsigned short b)
{
	return (a*b) & 0xffff;
}

unsigned int umul16_hi(unsigned short a, unsigned short b)
{
	uint32_t c;
	c = a*b;
	return (c>>16) & 0xffff;
}

uint64_t rot_left(uint64_t size, uint64_t a, uint64_t b)
{
    uint64_t tmp;

    b = b & 0x3F;
    b %= size;
    switch(size){
	    case 8:
		    tmp = (a << b) | ((a & 0xFF) >> (size - b));
		    return tmp & 0xFF;
	    case 16:
		    tmp = (a << b) | ((a & 0xFFFF) >> (size - b));
		    return tmp & 0xFFFF;
	    case 32:
		    tmp = (a << b) | ((a & 0xFFFFFFFF) >> (size - b));
		    return tmp & 0xFFFFFFFF;
	    case 64:
		    tmp = (a << b) | ((a&0xFFFFFFFFFFFFFFFF) >> (size - b));
		    return tmp & 0xFFFFFFFFFFFFFFFF;

	    /* Support cases for rcl */
	    case 9:
		    tmp = (a << b) | ((a & 0x1FF) >> (size - b));
		    return tmp & 0x1FF;
	    case 17:
		    tmp = (a << b) | ((a & 0x1FFFF) >> (size - b));
		    return tmp & 0x1FFFF;
	    case 33:
		    tmp = (a << b) | ((a & 0x1FFFFFFFF) >> (size - b));
		    return tmp & 0x1FFFFFFFF;
	    /* TODO XXX: support rcl in 64 bit mode */

	    default:
		    fprintf(stderr, "inv size in rotleft %"PRIX64"\n", size);
		    exit(EXIT_FAILURE);
    }
}

uint64_t rot_right(uint64_t size, uint64_t a, uint64_t b)
{
    uint64_t tmp;

    b = b & 0x3F;
    b %= size;
    switch(size){
	    case 8:
		    tmp = ((a & 0xFF) >> b) | (a << (size - b));
		    return tmp & 0xff;
	    case 16:
		    tmp = ((a & 0xFFFF) >> b) | (a << (size - b));
		    return tmp & 0xFFFF;
	    case 32:
		    tmp = ((a & 0xFFFFFFFF) >> b) | (a << (size - b));
		    return tmp & 0xFFFFFFFF;
	    case 64:
		    tmp = ((a & 0xFFFFFFFFFFFFFFFF) >> b) | (a << (size - b));
		    return tmp & 0xFFFFFFFFFFFFFFFF;

	    /* Support cases for rcr */
	    case 9:
		    tmp = ((a & 0x1FF) >> b) | (a << (size - b));
		    return tmp & 0x1FF;
	    case 17:
		    tmp = ((a & 0x1FFFF) >> b) | (a << (size - b));
		    return tmp & 0x1FFFF;
	    case 33:
		    tmp = ((a & 0x1FFFFFFFF) >> b) | (a << (size - b));
		    return tmp & 0x1FFFFFFFF;
	    /* TODO XXX: support rcr in 64 bit mode */

	    default:
		    fprintf(stderr, "inv size in rotright %"PRIX64"\n", size);
		    exit(EXIT_FAILURE);
    }
}

/*
 * Count leading zeros - count the number of zero starting at the most
 * significant bit
 *
 * Example:
 * - cntleadzeros(size=32, src=2): 30
 * - cntleadzeros(size=32, src=0): 32
 */
unsigned int cntleadzeros(uint64_t size, uint64_t src)
{
	int64_t i;

	for (i=(int64_t)size-1; i>=0; i--){
		if (src & (1ull << i))
			return size - (i + 1);
	}
	return size;
}

/*
 * Count trailing zeros - count the number of zero starting at the least
 * significant bit
 *
 * Example:
 * - cnttrailzeros(size=32, src=2): 1
 * - cnttrailzeros(size=32, src=0): 32
 */
unsigned int cnttrailzeros(uint64_t size, uint64_t src)
{
	uint64_t i;
	for (i=0; i<size; i++){
		if (src & (1ull << i))
			return i;
	}
	return size;
}


unsigned int my_imul08(unsigned int a, unsigned int b)
{
	char a08, b08;
	short a16;

	a08 = a&0xFF;
	b08 = b&0xFF;
	a16 = a08*b08;
	return (int)a16;
}



unsigned int x86_cpuid(unsigned int a, unsigned int reg_num)
{
	if (reg_num >3){
		fprintf(stderr, "not implemented x86_cpuid reg %x\n", reg_num);
		exit(EXIT_FAILURE);
	}
	// cases are output: EAX: 0; EBX: 1; ECX: 2; EDX: 3
	if (a == 0){
		switch(reg_num){
		case 0:
			return 0xa;
		// "GenuineIntel"
		case 1:
			return 0x756E6547;
		case 2:
			return 0x6C65746E;
		case 3:
			return 0x49656E69;
		}
	}

	else if (a == 1){
		switch(reg_num){
		case 0:
			// Using a version too high will enable recent
			// instruction set
			return 0x000006FB;
			//return 0x00020652;
		case 1:
			//return 0x02040800;
			return 0x00000800;
		case 2:
			//return 0x0004E3BD;
			return 0x00000209;
		case 3:
			return (/* fpu */ 1 << 0) |
				(/* tsc */ 1 << 4) |
				(/* cx8 */ 1 << 8) |
				(/* cmov */ 1 << 15) |
				(/* mmx */ 1 << 23) |
				(/* sse */ 1 << 25) |
				(/* sse2 */ 1 << 26) |
				(/* ia64 */ 1 << 30);
		}
	}
	// Cache and TLB
	else if (a == 2){
		switch(reg_num){
		case 0:
			return 0x00000000;
		case 1:
			return 0x00000000;
		case 2:
			return 0x00000000;
		case 3:
			return 0x00000000;
		}
	}
	// Intel thread/core and cache topology
	else if (a == 4){
		switch(reg_num){
		case 0:
			return 0x00000000;
		case 1:
			return 0x00000000;
		case 2:
			return 0x00000000;
		case 3:
			return 0x00000000;
		}
	}
	// Extended features
	else if (a == 7){
		switch(reg_num){
		case 0:
			return 0x00000000;
		case 1:
			return (/* fsgsbase */ 1 << 0) | (/* bmi1 */ 1 << 3);
		case 2:
			return 0x00000000;
		case 3:
			return 0x00000000;
		}
	}
	else{
		fprintf(stderr, "WARNING not implemented x86_cpuid index %X!\n", a);
		exit(EXIT_FAILURE);
	}
	return 0;
}

//#define DEBUG_MIASM_DOUBLE

void dump_float(void)
{
	/*
	printf("%e\n", vmmngr.float_st0);
	printf("%e\n", vmmngr.float_st1);
	printf("%e\n", vmmngr.float_st2);
	printf("%e\n", vmmngr.float_st3);
	printf("%e\n", vmmngr.float_st4);
	printf("%e\n", vmmngr.float_st5);
	printf("%e\n", vmmngr.float_st6);
	printf("%e\n", vmmngr.float_st7);
	*/
}

uint32_t fpu_fadd32(uint32_t a, uint32_t b)
{
	float c;
	c = *((float*)&a) + *((float*)&b);
#ifdef DEBUG_MIASM_DOUBLE
	dump_float();
	printf("%e + %e -> %e\n", a, b, c);
#endif
	return *((uint32_t*)&c);
}

uint64_t fpu_fadd64(uint64_t a, uint64_t b)
{
	double c;
	c = *((double*)&a) + *((double*)&b);
#ifdef DEBUG_MIASM_DOUBLE
	dump_float();
	printf("%e + %e -> %e\n", a, b, c);
#endif
	return *((uint64_t*)&c);
}

uint32_t fpu_fsub32(uint32_t a, uint32_t b)
{
	float c;
	c = *((float*)&a) - *((float*)&b);
#ifdef DEBUG_MIASM_DOUBLE
	dump_float();
	printf("%e + %e -> %e\n", a, b, c);
#endif
	return *((uint32_t*)&c);
}

uint64_t fpu_fsub64(uint64_t a, uint64_t b)
{
	double c;
	c = *((double*)&a) - *((double*)&b);
#ifdef DEBUG_MIASM_DOUBLE
	dump_float();
	printf("%e + %e -> %e\n", a, b, c);
#endif
	return *((uint64_t*)&c);
}

uint32_t fpu_fmul32(uint32_t a, uint32_t b)
{
	float c;
	c = *((float*)&a) * *((float*)&b);
#ifdef DEBUG_MIASM_DOUBLE
	dump_float();
	printf("%e * %e -> %e\n", a, b, c);
#endif
	return *((uint32_t*)&c);
}

uint64_t fpu_fmul64(uint64_t a, uint64_t b)
{
	double c;
	c = *((double*)&a) * *((double*)&b);
#ifdef DEBUG_MIASM_DOUBLE
	dump_float();
	printf("%e * %e -> %e\n", a, b, c);
#endif
	return *((uint64_t*)&c);
}

uint32_t fpu_fdiv32(uint32_t a, uint32_t b)
{
	float c;
	c = *((float*)&a) / *((float*)&b);
#ifdef DEBUG_MIASM_DOUBLE
	dump_float();
	printf("%e * %e -> %e\n", a, b, c);
#endif
	return *((uint32_t*)&c);
}

uint64_t fpu_fdiv64(uint64_t a, uint64_t b)
{
	double c;
	c = *((double*)&a) / *((double*)&b);
#ifdef DEBUG_MIASM_DOUBLE
	dump_float();
	printf("%e * %e -> %e\n", a, b, c);
#endif
	return *((uint64_t*)&c);
}

double fpu_ftan(double a)
{
	double b;
	b = tan(a);
#ifdef DEBUG_MIASM_DOUBLE
	dump_float();
	printf("%e tan %e\n", a, b);
#endif
	return b;
}

double fpu_frndint(double a)
{
	int64_t b;
	double c;
	b = (int64_t)a;
	c = (double)b;
#ifdef DEBUG_MIASM_DOUBLE
	dump_float();
	printf("%e double %e\n", a, c);
#endif
	return c;
}

double fpu_fsin(double a)
{
	double b;
	b = sin(a);
#ifdef DEBUG_MIASM_DOUBLE
	dump_float();
	printf("%e sin %e\n", a, b);
#endif
	return b;
}

double fpu_fcos(double a)
{
	double b;
	b = cos(a);
#ifdef DEBUG_MIASM_DOUBLE
	dump_float();
	printf("%e cos %e\n", a, b);
#endif
	return b;
}


double fpu_fscale(double a, double b)
{
	double c;
	c = a * exp2(trunc(b));
#ifdef DEBUG_MIASM_DOUBLE
	dump_float();
	printf("%e *exp2 %e -> %e\n", a, b, c);
#endif
	return c;
}

double fpu_f2xm1(double a)
{
	double b;
	b = exp2(a)-1;
#ifdef DEBUG_MIASM_DOUBLE
	dump_float();
	printf("%e exp2 -1 %e\n", a, b);
#endif
	return b;
}

uint32_t fpu_fsqrt32(uint32_t a)
{
	float b;
	b = sqrtf(*((float*)&a));
#ifdef DEBUG_MIASM_DOUBLE
	dump_float();
	printf("%e sqrt %e\n", a, b);
#endif
	return *((uint32_t*)&b);
}

uint64_t fpu_fsqrt64(uint64_t a)
{
	double b;
	b = sqrt(*((double*)&a));
#ifdef DEBUG_MIASM_DOUBLE
	dump_float();
	printf("%e sqrt %e\n", a, b);
#endif
	return *((uint64_t*)&b);
}

uint64_t fpu_fabs64(uint64_t a)
{
	double b;
	b = abs(*((double*)&a));
#ifdef DEBUG_MIASM_DOUBLE
	dump_float();
	printf("%e abs %e\n", a, b);
#endif
	return *((uint64_t*)&b);
}

uint64_t fpu_fprem64(uint64_t a, uint64_t b)
{
	double c;
	c = fmod(*((double*)&a), *((double*)&b));
#ifdef DEBUG_MIASM_DOUBLE
	dump_float();
	printf("%e %% %e -> %e\n", a, b, c);
#endif
	return *((uint64_t*)&c);
}

double fpu_fchs(double a)
{
	double b;
	b = -a;
#ifdef DEBUG_MIASM_DOUBLE
	dump_float();
	printf(" - %e -> %e\n", a, b);
#endif
	return b;
}

double fpu_fyl2x(double a, double b)
{
	double c;
	c = b * (log(a) / log(2));
#ifdef DEBUG_MIASM_DOUBLE
	dump_float();
	printf("%e * log(%e) -> %e\n", b, a, c);
#endif
	return c;
}

double fpu_fpatan(double a, double b)
{
	double c;
	c = atan2(b, a);
#ifdef DEBUG_MIASM_DOUBLE
	dump_float();
	printf("arctan(%e / %e) -> %e\n", b, a, c);
#endif
	return c;
}

unsigned int fpu_fcom_c0(double a, double b)
{
	if (isnan(a) || isnan(b))
		return 1;
	if (a>=b)
		return 0;
	return 1;
}
unsigned int fpu_fcom_c1(double a, double b)
{
	//XXX
	return 0;
}
unsigned int fpu_fcom_c2(double a, double b)
{
	if (isnan(a) || isnan(b))
		return 1;
	return 0;
}
unsigned int fpu_fcom_c3(double a, double b)
{
	if (isnan(a) || isnan(b))
		return 1;
	if (a==b)
		return 1;
	return 0;
}

uint64_t sint_to_fp_64(int64_t a)
{
	double result = (double) a;
	return *((uint64_t*)&result);
}

uint32_t sint_to_fp_32(int32_t a)
{
	float result = (float) a;
	return *((uint32_t*)&result);
}

int32_t fp32_to_sint32(uint32_t a)
{
	// Enforce nearbyint (IEEE-754 behavior)
	float rounded = *((float*)&a);
	rounded = nearbyintf(rounded);
	return (int32_t) rounded;
}

int64_t fp64_to_sint64(uint64_t a)
{
	// Enforce nearbyint (IEEE-754 behavior)
	double rounded = *((double*)&a);
	rounded = nearbyint(rounded);
	return (int64_t) rounded;
}

int32_t fp64_to_sint32(uint64_t a)
{
	// Enforce nearbyint (IEEE-754 behavior)
	double rounded = *((double*)&a);
	rounded = nearbyint(rounded);
	return (int32_t) rounded;
}

uint32_t fp64_to_fp32(uint64_t a)
{
	float result = (float) *((double*)&a);
	return *((uint32_t*)&result);
}

uint64_t fp32_to_fp64(uint32_t a)
{
	double result = (double) *((float*)&a);
	return *((uint64_t*)&result);
}

uint32_t fpround_towardszero_fp32(uint32_t a)
{
	float rounded = *((float*)&a);
	rounded = truncf(rounded);
	return *((uint32_t*)&rounded);
}

uint64_t fpround_towardszero_fp64(uint64_t a)
{
	double rounded = *((float*)&a);
	rounded = trunc(rounded);
	return *((uint64_t*)&rounded);
}


UDIV(8)
UDIV(16)
UDIV(32)
UDIV(64)

UMOD(8)
UMOD(16)
UMOD(32)
UMOD(64)

IDIV(8)
IDIV(16)
IDIV(32)
IDIV(64)

IMOD(8)
IMOD(16)
IMOD(32)
IMOD(64)
