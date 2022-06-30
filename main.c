#include <malloc.h>
#include <memory.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

struct bignum {
	size_t sz;
	uint8_t negate;
	uint8_t isNaN;
	uint64_t* digits;
};


int add(struct bignum* a, struct bignum* b);
int sub(struct bignum* a, struct bignum* b);
int init(struct bignum* num, const char* str);
int equate(struct bignum* dst, const struct bignum* src);
int bfree(struct bignum* number);
int isNull(struct bignum* number);
void print(struct bignum* number);

int init(struct bignum* num, const char* str) {
	char sub[10];
	size_t sz = -1, null_sz = 0;
	memset(num, 0, sizeof(struct bignum));

	while(str[++sz]);
	if (!sz) return -1;

	if (str[0] == '-') {
		null_sz++;
		num->negate = 1;
	}

	if (!memcmp(str + null_sz, "NaN", 3))
	{
		num->isNaN = 1;
		num->digits = NULL;
		return 0;
	}

	while (str[++null_sz] == '0' && sz > null_sz);

	if (null_sz == sz)
	{
		if (!(num->digits = malloc(sizeof(uint64_t)))) return -1;
		num->digits[0] = 0;
		num->sz = 1;
		return 0;
	}

	num->sz = sz / 9 + 1;
	if (!(num->digits = malloc(sizeof(uint64_t) * num->sz))) return -1;
	memset(num->digits, 0, sizeof(uint64_t) * num->sz);


	for (long long i = sz - 1, index = 0; i >= (long long)null_sz; i -= 9, index++)
	{
		size_t start = (i < 9) ? null_sz : (i - 8);
		size_t size = (i < 9) ? (i - null_sz + 1) : 9;
		memcpy(sub, &str[start], size);
		sub[size] = 0;
		num->digits[index] = atoi(sub);
	}

	while(!num->digits[num->sz - 1]) num->sz--;
	return 0;
}

int equate(struct bignum* dst, const struct bignum* src) {
	dst->isNaN = src->isNaN;
	dst->negate = src->negate;
	dst->sz = src->sz;
	free(dst->digits);
	if (!(dst->digits = malloc(sizeof(uint64_t) * dst->sz))) return -1;
	memcpy(dst->digits, src->digits, sizeof(uint64_t) * dst->sz);
	return 0;
}

int add(struct bignum* a, struct bignum* b) {
	if (a->isNaN || b->isNaN) {
		a->isNaN = 1;
		return 0;
	}
	if(a->negate != b->negate) {
		b->negate ^= 1;
		int code = sub(a, b);
		b->negate ^= 1;
		return code;
	}
}


int sub(struct bignum* a, struct bignum* b) {
	if (a->isNaN || b->isNaN) {
		a->isNaN = 1;
		return 0;
	}
	if(a->negate != b->negate) {
		b->negate ^= 1;
		int code = add(a, b);
		b->negate ^= 1;
		return code;
	}
}


int bfree(struct bignum* number) {
	free(number->digits);
}

int isNull(struct bignum* number) {
	if(number->sz == 1 && !number->digits[0]) return 1;
	return 0;
}

void print(struct bignum* number) {
	printf("Debug:\n\tnegate: %i\n\tsize: %zi\n", number->negate, number->sz);

	if (number->isNaN) {
		printf("NaN");
		return;
	}

	if (isNull(number)) {
		printf("0");
		return;
	}

	if (number->negate) printf("-");
	printf("%lu", number->digits[number->sz - 1]);

	for (size_t i = 1; i < number->sz; i++) {
		uint64_t n = 10;
		for (int j = 0; j < 8; j++)
		{
			if (number->digits[number->sz - i - 1] < n) printf("0");
			n *= 10;
		}
		printf("%lu", number->digits[number->sz - i - 1]);
	}
}

int main(int argc, char* argv[]) {
	struct bignum number;
	init(&number, "-0000000000000000000000000");
	printf("-10000865464987848998000000000000000000000000000000003\n");
	print(&number);
	printf("\n");
	bfree(&number);
	return 0;
}
