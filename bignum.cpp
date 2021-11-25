#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

#define MAXDIGITS	100
#define PLUS		  1
#define MINUS		 -1

typedef long long ll;
typedef struct {
	char digits[MAXDIGITS];
	int signbit;
	int lastdigit;
} bignum; //basic operations are in repository

void ll_to_bignum(ll s, bignum *n) {
	ll aux;

	if (s>=0) n->signbit = PLUS;
	else n->signbit = MINUS;
	aux = (s >= 0) ? s : -s;

	for(int i = 0; i < MAXDIGITS; i++)
		n->digits[i] = (char) 0;
	n->lastdigit= -1;

	while (aux > 0) {
		n->lastdigit++;
		n->digits[ n->lastdigit ] = (char) (aux%10);
		aux /= 10;
	}

	if (s == 0) n->lastdigit = 0;
}

void initialize_bignum(bignum *n) {
	ll_to_bignum(0, n);
}

int compare_bignum(bignum *a, bignum *b) {
	if ((a->signbit == MINUS) && (b->signbit == PLUS)) return(PLUS);
	if ((a->signbit == PLUS) && (b->signbit == MINUS)) return(MINUS);

	if (a->lastdigit > b->lastdigit) return (MINUS * a->signbit);
	if (b->lastdigit > a->lastdigit) return (PLUS * a->signbit);

	for (int i = a->lastdigit; i >= 0; i--) {
		if (a->digits[i] > b->digits[i]) return(MINUS * a->signbit);
		if (b->digits[i] > a->digits[i]) return(PLUS * a->signbit);
	}
	
	return (0);
}

void digit_shift(bignum *n, int d) {	//multiply n by 10^d
	if ((n->lastdigit == 0) && (n->digits[0] == 0)) return;

	for (int i = n->lastdigit; i >= 0; i--)
		n->digits[i+d] = n->digits[i];

	for (int i = 0; i < d; i++)
		n->digits[i] = 0;

	n->lastdigit = n->lastdigit + d;
}

void zero_justify(bignum *n) {
	while ((n->lastdigit > 0) && (n->digits[ n->lastdigit ] == 0))
		n->lastdigit--;

	//hack to avoid -0
	if ((n->lastdigit == 0) && (n->digits[0] == 0))
		n->signbit = PLUS;
}

void subtract_bignum(bignum *a, bignum *b, bignum *c);
void add_bignum(bignum *a, bignum *b, bignum *c) {
	int carry = 0, aux;
	initialize_bignum(c);

	if (a->signbit == b->signbit) c->signbit = a->signbit;
	else {
		if (a->signbit == MINUS) {
			a->signbit = PLUS;
			subtract_bignum(b,a,c);
			a->signbit = MINUS;
		} else {
			b->signbit = PLUS;
			subtract_bignum(a,b,c);
			b->signbit = MINUS;
		}
		return;
	}

	c->lastdigit = max(a->lastdigit, b->lastdigit) + 1;

	for(int i = 0; i <= (c->lastdigit); i++) {
		aux = carry + a->digits[i] + b->digits[i];
		c->digits[i] = (char) aux % 10;
		carry = aux / 10;
	}
	zero_justify(c);
}

void subtract_bignum(bignum *a, bignum *b, bignum *c) {
	int borrow = 0, temp;
	initialize_bignum(c);

	if ((a->signbit == MINUS) || (b->signbit == MINUS)) {
		b->signbit = MINUS * b->signbit;
		add_bignum(a,b,c);
		b->signbit = MINUS * b->signbit;
		return;
	}

	if (compare_bignum(a,b) == PLUS) {
		subtract_bignum(b,a,c);
		c->signbit = MINUS;
		return;
	}

	c->lastdigit = max(a->lastdigit, b->lastdigit);
	
	for(int i = 0; i <= (c->lastdigit); i++) {
		temp = (a->digits[i] - borrow - b->digits[i]);
		if (a->digits[i] > 0)
			borrow = 0;
		if (temp < 0) {
			temp += 10;
			borrow = 1;
		}
		c->digits[i] = (char) temp;
	}
	zero_justify(c);
}

void multiply_bignum(bignum *a, bignum *b, bignum *c) {
	bignum row = *a, tmp;
	initialize_bignum(c);

	for(int i = 0; i <= b->lastdigit; i++) {
		for (int j = 1; j <= b->digits[i]; j++)
		{
			add_bignum(c,&row,&tmp);
			*c = tmp;
		}
		digit_shift(&row,1);
	}

	c->signbit = a->signbit * b->signbit;

	zero_justify(c);
}

void divide_bignum(bignum *a, bignum *b, bignum *c) {
	bignum row, tmp;
	int asign = a->signbit;
	int bsign = b->signbit;
	initialize_bignum(c); initialize_bignum(&row); initialize_bignum(&tmp);

	c->lastdigit = a->lastdigit;
	c->signbit = a->signbit * b->signbit;

	a->signbit = PLUS; b->signbit = PLUS;

	for(int i = a->lastdigit; i >= 0; i--) {
		digit_shift(&row,1);
		row.digits[0] = a->digits[i];
		c->digits[i] = 0;
		while(compare_bignum(&row,b) != PLUS) {
			c->digits[i]++;
			subtract_bignum(&row,b,&tmp);
			row = tmp;
		}
	}
	zero_justify(c);
	a->signbit = asign;
	b->signbit = bsign;
}

void print_bignum(bignum *n) {
	if (n->signbit == MINUS)
		cout << '-';
	for (int i = n->lastdigit; i >= 0; i--) {
		cout << (char) (n->digits[i] + '0');
	}
	cout << endl;
}

void string_to_bignum(string s, bignum *n) {
		n->lastdigit = s.length()-1;
	for(int i = 0; i <= n->lastdigit; i++)
		n->digits[i] = s[n->lastdigit - i] - '0';
}
