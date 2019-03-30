#pragma once
#include <iostream>
using namespace std;

class Array {
	int *list;
	int i;
	int columns;
public:
	const int& operator[](int j) const{
		if (j > columns) throw out_of_range("");
		return list[i * columns + j - 1];
	}
	int& operator[](int j) {
		if (j > columns) throw out_of_range("");
		return list[i * columns + j - 1];
	}
	Array(int* m, size_t i, size_t columns) : list(m), i(i), columns(columns){
	}
	~Array() {
	}
};

class Matrix
{
	size_t rows;
	size_t columns;
	int *m;
public:
	Matrix() {}
	Matrix(size_t row, size_t column) {
		rows = row;
		columns = column;
		m = new int[rows * columns]();
	}
	~Matrix() {
		delete[] m;
	}
	size_t getRows() const{
		return rows;
	}
	size_t getColumns() const{
		return columns;
	}
	void show()
	{
		for (size_t i = 0; i < rows; i++) {
			for (size_t j = 0; j < columns; j++) {
				printf("%3d", m[i * columns + j]);
			}
			cout << endl;
		}
		cout << endl;
	}
	Matrix& operator *=(int a) {
		for (size_t i = 0; i < rows; i++) {
			for (size_t j = 0; j < columns; j++) {
				m[i * columns + j] *= a;
			}
		}
		return *this;
	}
	bool operator==(const Matrix& B)  const
	{
		if (this == &B)return true;
		else if (!(rows == B.rows && columns == B.columns))return false;
		else {
			for (size_t i = 0; i < rows; i++) {
				for (size_t j = 0; j < columns; j++) {
					if (m[i * columns + j] != B.m[i * columns + j]) return false;
				}
			}
		}
		return true;
	}
	bool operator !=(const Matrix& B) const
	{
		return (!(*this == B));
	}
	const Array operator[](size_t i) const{
		if (i > rows) throw out_of_range("");
		return Array(m, i - 1, columns);
	}
	Array operator[](size_t i) {
		if (i > rows) throw out_of_range("");
		return Array(m, i - 1, columns);
	}
};