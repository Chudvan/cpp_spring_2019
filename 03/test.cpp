#include "matrix.h"
int main() {
	Matrix A(3, 4);
	A[2][1] = 5;
	std::cout << A[2][1];
	int n;
	std::cin >> n;
	return 0;
}