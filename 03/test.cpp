#include "matrix.h"
#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "");
	const size_t rows = 5;
	const size_t cols = 3;
	Matrix A(rows, cols);
	A.show();
	A[2][1] = 5;
	A.show();
	A[1][1] = 7;
	A *= 10;
	A.show();
	Matrix B(3, 4);
	cout << (A.getColumns() == 3) << endl << (A.getRows() == 5) << endl;
	cout << endl;
	auto m2 = A *= 5;
	m2.show();
	int n;
	std::cin >> n;
	return 0;
}