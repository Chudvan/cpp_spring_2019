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
	cout << -4 + a + 0 + b + 78 + c - d << endl << endl;
	//Тестирование базовых арифметических операций.
	cout << (a < b) << " " << (a == b) << " " << (b >= d) << endl << endl;
	//Тестирование базовых операций сравнения.
	for (int i = 0; i < 450; i++) a = a + a + a + a + a + a + a + a + a + a;
	cout << a << endl << endl;
	//Сформировали очень большое число.
	cout << a + 11 + d << endl << endl;
	//С ним также можно выполнять арифметические операции.
	BigInt e = a + 11 + d;
	//Им можно инициализировать.
	cout << e << endl << endl;
	cout << (-e > -b) << " " << (e <= 44) << " " << (a >= d) << endl;
	//Тестирование операций сравнения с большим числом.
	system("pause>nul");
	return 0;
}