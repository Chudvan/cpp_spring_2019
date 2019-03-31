#pragma once
#define BASELEN 4
#define BASE 10000
#define MAXLEN 100
#include <iostream>
#include <string>
#include <cmath>
using namespace std;

class BigInt {
public:
	BigInt() {
		bI = new int64_t[MAXLEN + 1]();
	}
	BigInt(int64_t b) {
		bI = new int64_t[MAXLEN + 1]();
		char *str = new char[MAXLEN];
		intToStr(b, str);
		strTolong(str, bI);
	}
	BigInt(const BigInt& value) : bI(new int64_t[MAXLEN + 1]()) {
		copy(value.bI, value.bI + MAXLEN + 1, bI);
	}
	~BigInt() {
		delete[] bI;
	}
	BigInt& operator=(int64_t b) {
		char *str = new char[MAXLEN];
		intToStr(b, str);
		strTolong(str, bI);
		return *this;
	}
	BigInt& operator=(const BigInt& value) {
		copy(value.bI, value.bI + MAXLEN + 1, bI);
		return *this;
	}
	BigInt operator+(const BigInt& value) const {
		int64_t *bI_2, *bI_3;
		bI_2 = value.getBigInt();
		if ((bI[0] * bI_2[0] < 0) && (bI[abs(bI[0])] != 0) && (bI_2[abs(bI_2[0])] != 0)) {
			//a>0, b<0 || a<0, b>0 удобнее использовать вычитание
			return *this - (-value);
		}
		int64_t m = max(abs(bI[0]), abs(bI_2[0]));
		BigInt c;
		bI_3 = c.getBigInt();
		int64_t buffer = 0;
		for (int64_t i = 1; i <= m + 1; i++) {
			bI_3[i] = bI[i] + bI_2[i] + buffer;
			buffer = 0;
			if (bI_3[i] >= BASE) {
				bI_3[i] -= BASE;
				buffer = 1; //перенос
			}
		}
		if (!bI_3[m + 1])bI_3[0] = m; //не вышли за границу
		else bI_3[0] = m + 1; //число стало на 1 разряд больше
		if ((bI[0] < 0) || (bI_2[0] < 0))bI_3[0] *= -1;
		return c;
	}
	BigInt operator-(const BigInt& value) const {
		int64_t *bI_2, *bI_3;
		bI_2 = value.getBigInt();
		if (bI[0] * bI_2[0] < 0)return *this + (-value);//аналогично, если числа разного знака
		BigInt c;
		bI_3 = c.getBigInt();
		int64_t m = abs(bI[0]);
		int64_t buffer = 0;
		if (bigIntCMP(bI, bI_2) == -1) { //число 1-е < 2-го НУЖНО СДЕЛАТЬ РАЗНОСТЬ ДЛЯ ЭТОГО СЛУЧАЯ ТОЖЕ
			return (-value) - (-*this);
		}
		for (int64_t i = 1; i <= m; i++) {
			if (bI[i] < bI_2[i]) {
				bI[i] += BASE;
				buffer = 1;
			}
			bI_3[i] = bI[i] - bI_2[i]; //псевдовычитание
			bI[i + 1] -= buffer;
			buffer = 0;
		}
		if (!bI_3[m])while (!bI_3[m] && m > 1)m--; //уменьшилось ли число разрядов?? - ответ.
		bI_3[0] = sgn(bI[0])*m;
		return c;
	}
	BigInt operator-(int64_t& value) {
		BigInt b = value;
		return *this - b;
	}
	BigInt operator-() const {
		bI[0] = (-1)*bI[0];
		BigInt b = *this;
		bI[0] = (-1)*bI[0];
		return b;
	}
	bool operator<(const BigInt& value) const {
		int64_t *bI_buffer_2 = value.getBigInt();
		if (!abs(bI[0]) || !abs(bI_buffer_2[0])) throw "Ошибка вывода. Число не инициализировано.";
		BigInt c = *this - value;
		int64_t *bI_buffer = c.getBigInt();
		return bI_buffer[0] < 0;
	}
	bool operator<(int64_t value) const {
		BigInt c = value;
		return *this < c;
	}
	bool operator>(const BigInt& value) const {
		int64_t *bI_buffer_2 = value.getBigInt();
		if (!abs(bI[0]) || !abs(bI_buffer_2[0])) throw "Ошибка вывода. Число не инициализировано.";
		BigInt c = *this - value;
		int64_t *bI_buffer = c.getBigInt();
		return bI_buffer[0] > 0 && bI_buffer[1];
	}
	bool operator>(int64_t value) const {
		BigInt c = value;
		return *this > c;
	}
	bool operator==(const BigInt& value)const {
		int64_t *bI_buffer_2 = value.getBigInt();
		if (!abs(bI[0]) || !abs(bI_buffer_2[0])) throw "Ошибка вывода. Число не инициализировано.";
		BigInt c = *this - value;
		int64_t *bI_buffer = c.getBigInt();
		return !bI_buffer[1];
	}
	bool operator==(int64_t value)const {
		BigInt c = value;
		return *this == c;
	}
	bool operator!=(const BigInt& value)const {
		return !(*this == value);
	}
	bool operator!=(int64_t value)const {
		BigInt c = value;
		return !(*this == value);
	}
	bool operator>=(const BigInt& value)const {
		return !(*this < value);
	}
	bool operator>=(int64_t value)const {
		BigInt c = value;
		return !(*this < c);
	}
	bool operator<=(const BigInt& value)const {
		return !(*this > value);
	}
	bool operator<=(int64_t value)const {
		BigInt c = value;
		return !(*this > c);
	}
	int64_t bigIntCMP(int64_t* la, int64_t*  lb) const { //Метод класса BigInt, СРАВНЕНИЕ КОЛ-ВА РАЗРЯДОВ у двух длинных чисел
		if (abs(la[0]) > abs(lb[0]))return 1;
		if (abs(la[0]) < abs(lb[0]))return -1;
		for (int64_t i = abs(la[0]); i >= 1; i--) {
			if (la[i] > lb[i]) return 1;
			if (la[i] < lb[i]) return -1;
		}
		return 0;
	}
	int64_t max(int64_t a, int64_t b) const {//Метод класса BigInt,          max
		int64_t c = (a > b) ? a : b;
		return c;
	}
	int64_t* getBigInt() const {
		return bI;
	}
	void strTolong(string a, int64_t* la) { //Метод класса BigInt, преобразует СТРОКУ в ДЛИННОЕ ЧИСЛО (int*)
		if (a[0] == '-') {
			la[0] = -static_cast<int64_t>(ceil((double)(a.size() - 1) / BASELEN));
			a.erase(0, 1);
		}
		else la[0] = static_cast<int64_t>(ceil((double)a.size() / BASELEN));
		int64_t i = 0;
		for (i = 1; i < abs(la[0]); i++) {
			la[i] = atois(a.substr(a.size() - i * BASELEN, BASELEN));
		}
		la[abs(la[0])] = atois(a.substr(0, a.size() - (i - 1)*BASELEN));
	}
	void intToStr(int64_t a, char* b) {
		int count, cur = 0;
		if (a < 0) {
			b[0] = '-';
			a = abs(a);
			cur = 1;
		}
		else if (!a) {
			b[0] = '0';
			b[1] = '\0';
		}
		count = digitCount(a);
		b[cur + count] = '\0';
		for (int i = cur + count - 1; i >= cur; i--) {
			b[i] = static_cast<char>(48 + a % 10);
			a /= 10;
		}
	}
	int64_t sgn(int64_t a) const { //Метод класса BigInt,                sgn
		if (!a)return 0;
		return a / abs(a);
	}
	int digitCount(int64_t a) const { //Метод класса BigInt, выдаёт КОЛ-ВО РАЗРЯДОВ в числе (в 10-ричной системе)
		int64_t count = 0;
		if (!a)return 1;
		while (a) {
			count++;
			a /= 10;
		}
		return count;
	}
	int64_t atois(string str) {//Метод класса BigInt, ПРЕОБРАЗОВАНИЕ STRING В ЧИСЛО
		const char *c = str.c_str();
		int64_t a = atoi(c);
		return a;
	}
private:
	int64_t *bI;
};
ostream& operator<<(ostream& out, const BigInt& value)
{
	int64_t *bI = value.getBigInt();
	if (!bI[0]) throw "Ошибка вывода. Число не инициализировано.";
	out << value.sgn(bI[0]) * bI[abs(bI[0])];
	int64_t dc;
	for (int64_t i = abs(bI[0]) - 1; i >= 1; i--) {
		dc = value.digitCount(bI[i]);
		for (int64_t j = 0; j < BASELEN - dc; j++)out << "0";
		out << bI[i];
	}
	return out;
}