#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

#define PLUS		  1
#define MINUS		 -1

typedef long long ll;
typedef struct {
	vector<char> digits;
	int signbit;
} bignum; //basic operations are in repository

void ll_to_bignum(ll s, bignum *n) {
	ll aux;

	n->digits.resize(0);

	if (s>=0) n->signbit = PLUS;
	else n->signbit = MINUS;
	aux = (s >= 0) ? s : -s;

	while (aux > 0) {
		n->digits.push_back((char) (aux%10));
		aux /= 10;
	}

	if (s == 0) n->digits.push_back((char) 0);
}

void initialize_bignum(bignum *n) {
	ll_to_bignum(0, n);
}

int compare_bignum(bignum *a, bignum *b) {
	if ((a->signbit == MINUS) && (b->signbit == PLUS)) return(PLUS);
	if ((a->signbit == PLUS) && (b->signbit == MINUS)) return(MINUS);

	if (a->digits.size() > b->digits.size()) return (MINUS * a->signbit);
	if (b->digits.size() > a->digits.size()) return (PLUS * a->signbit);

	for (int i = a->digits.size()-1; i >= 0; i--) {
		if (a->digits[i] > b->digits[i]) return(MINUS * a->signbit);
		if (b->digits[i] > a->digits[i]) return(PLUS * a->signbit);
	}
	
	return (0);
}

void digit_shift(bignum *n) {	//multiply n by 10
	if ((n->digits.size()-1 == 0) && (n->digits[0] == 0)) return;

	n->digits.push_back(n->digits[ n->digits.size() - 1 ]);
	for (int i = n->digits.size() - 2; i >= 0; i--)
		n->digits[i+1] = n->digits[i];

	for (int i = 0; i < 1; i++)
		n->digits[i] = 0;
}

void zero_justify(bignum *n) {
	while ((n->digits.size()-1 > 0) && (n->digits[ n->digits.size()-1 ] == 0))
		n->digits.pop_back();

	//hack to avoid -0
	if ((n->digits.size()-1 == 0) && (n->digits[0] == 0))
		n->signbit = PLUS;
}

void subtract_bignum(bignum *a, bignum *b, bignum *c) {
	int borrow = 0, temp, maxSize;
	char safeB;
	initialize_bignum(c);
	c->digits.pop_back();

	maxSize = a->digits.size();
	
	for(int i = 0; i <= maxSize-1; i++) {
		safeB = i <= b->digits.size() - 1 ? b->digits[i] : 0;
		temp = (a->digits[i] - borrow - safeB);
		if (a->digits[i] > 0)
			borrow = 0;
		if (temp < 0) {
			temp += 10;
			borrow = 1;
		}
		c->digits.push_back((char) temp);
	}
	zero_justify(c);
}

void divide_bignum(bignum *a, bignum *b, bignum *c, int altMod=0) {
	if (compare_bignum(a,b) == PLUS) {
		if(!altMod) {
			initialize_bignum(c);
			return;
		}
		else {
			*c = *a;
			return;
		}
	}
	bignum row, tmp;
	int asign = a->signbit;
	int bsign = b->signbit;
	initialize_bignum(c); initialize_bignum(&row);

	for (int i = a->digits.size(); i > 1; i--)
		c->digits.push_back((char) 0);
	c->signbit = a->signbit * b->signbit;

	a->signbit = PLUS; b->signbit = PLUS;

	for(int i = a->digits.size()-1; i >= 0; i--) {
		digit_shift(&row);
		row.digits[0] = a->digits[i];
		c->digits[i] = 0;
		while(compare_bignum(&row,b) != PLUS) {
			c->digits[i]++;
			subtract_bignum(&row,b,&tmp);
			row = tmp;
		}
	}
	if (altMod)
		*c = row;
	zero_justify(c);
	a->signbit = asign;
	b->signbit = bsign;
}

void print_bignum(bignum *n) {
	if (n->signbit == MINUS)
		cout << '-';
	for (int i = n->digits.size()-1; i >= 0; i--) {
		cout << (char) (n->digits[i] + '0');
	}
	cout << endl;
}

void string_to_bignum(string s, bignum *n) {
	n->digits.resize(0);
	
	for(int i = 0; i <= s.length()-1; i++)
		n->digits.push_back( s[ s.length()-1 - i] - '0');
}
