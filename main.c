#include <malloc.h>
#include <memory.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

struct bignum {
	size_t sz;
	uint8_t negate;
	uint64_t* digits;
};

int init(struct bignum* num, const char* str) {
	char sub[10];
	size_t sz = -1, null_sz = 0;
	memset(num, 0, sizeof(struct bignum));

	while(str[++sz]);
	if (!sz) return -1;
	num->sz = sz / 9 + 1;
	if (!(num->digits = malloc(sizeof(uint64_t) * num->sz))) return -1;
	memset(num->digits, 0, sizeof(uint64_t) * num->sz);

	if (str[0] == '-') {
		null_sz++;
		num->negate = 1;
	}

	for (long long i = sz - 1, index = 0; i >= (long long)null_sz; i -= 9, index++)
	{
		size_t start = (i < 9) ? null_sz : (i - 8);
		size_t size = (i < 9) ? (i - null_sz + 1) : 9;
		memcpy(sub, &str[start], size);
		sub[size] = 0;
		num->digits[index] = atoi(sub);
	}

	while(!num->digits[num->sz - 1]) num->sz--;
}

int bfree(struct bignum* number) {
	free(number->digits);
}

void print(struct bignum* number) {
	printf("Debug:\n\tnegate: %i\n\tsize: %zi\n", number->negate, number->sz);

	if(number->negate) printf("-");
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
	init(&number, "-10000865464987848998000000000000000000000000000000003");
	printf("-10000865464987848998000000000000000000000000000000003\n");
	print(&number);
	printf("\n");
	bfre(&number);
	return 0;
}
