#ifndef Operator_h
#define Operator_h

#include <vector>

using namespace std;
using Vector1 = vector<double>;
using dmatrix = vector<Vector1>;

Vector1 operator+=(Vector1 & a, const Vector1 & b) {
	int size = a.size();
	for (unsigned int i = 0; i < a.size(); i++) {
		a[i] += b[i];
	}
	return a;
}

Vector1 operator/=(Vector1 & a, const double & b) {
	int size = a.size();
	for (unsigned int i = 0; i < a.size(); i++) {
		a[i] = a[i] / b; // what if b's input were int? still the a[i] be dbl?
	}
	return a;
}

Vector1 operator+(const Vector1 & a, const Vector1 &  b) {
	int size = a.size();
	Vector1 c(size);
	for (unsigned int i = 0; i < a.size(); i++) {
		c[i] = a[i] + b[i];
	}
	return c;
}

Vector1 operator-(const Vector1 & a, const Vector1 &  b) {
	int size = a.size();
	Vector1 c(size);
	for (unsigned int i = 0; i < a.size(); i++) {
		c[i] = a[i] - b[i];
	}
	return c;
}

Vector1 operator/(const Vector1 & a, const Vector1 &  b) {
	int size = a.size();
	Vector1 c(size);
	for (unsigned int i = 0; i < a.size(); i++) {
		c[i] = a[i] / b[i];
	}
	return c;
}

/*

dmatrix operator+(const dmatrix & a, const dmatrix & b) {
	int size = a.size();
	dmatrix c(size);
	for (unsigned int i = 0; i < a.size; i++) {
		for (unsigned int j = 0; j < b.size; j++) {
			c[i][j] = a[i][j] + b[i][j];
		}
	}
	return c;
}

dmatrix operator-(const dmatrix & a, const dmatrix & b) {
	int size = a.size();
	dmatrix c(size);
	for (unsigned int i = 0; i < a.size; i++) {
		for (unsigned int j = 0; j < b.size; j++) {
			c[i][j] = a[i][j] - b[i][j];
		}
	}
	return c;
}

dmatrix operator/(const dmatrix & a, const dmatrix & b) {
	int size = a.size();
	dmatrix c(size);
	for (unsigned int i = 0; i < a.size; i++) {
		for (unsigned int j = 0; j < b.size; j++) {
			c[i][j] = a[i][j] / b[i][j];
		}
	}
	return c;
}

*/

#endif