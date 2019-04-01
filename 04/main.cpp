#include <iostream>
#include "BigInt.h"
using namespace std;
int main()
{
	setlocale(LC_ALL, "");
	BigInt a = -9223372036854775807;
	BigInt b = 9223372036854775807;
	BigInt c = 9223372036854775807;
	int64_t d = 999999999999999999;
	cout <<-4 + a + 0 + b + 78 + c - d<< endl;
	cout << (a < b) << " " << (a == b) << " " << (b >= d);
	system("pause>nul");
	return 0;
}