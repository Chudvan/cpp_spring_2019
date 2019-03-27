#pragma once
#include <iostream>
class Proxy {
	int *n;
public:
	void operator=(int a) {
		*n = a;
	}
	void operator*=(int a) {
		*n *= a;
	}
	bool operator !=(Proxy b) {
		return (*n != b.getter());
	}
	Proxy(int &element) : n(&element) {};
	int & getter() {
		return *n;
	}
	~Proxy() {}
};

class Array {
	int * list;
	int columns;
	int errorInt = 1;
public:
	Proxy operator[](int j) {
		try {
			if (j < 1 || j > columns) throw std::out_of_range("");
			return Proxy(list[j - 1]);
		}
		catch (std::out_of_range) {
			std::cout << "Выход за пределы матрицы." << std::endl;
			return Proxy(errorInt);
		}
	}
	void declaration(int cols) {
		list = new int[cols]();
		columns = cols;
	}
	void setter(int j, int a) {
		list[j] = a;
	}
	int getter(int j) {
		return list[j];
	}
	Array() {}
	Array(int error) {}
	~Array() {
		delete[] list;
	}
};

class Matrix
{
	size_t rows;
	size_t columns;
	Array errorArray;
	Array *m;
public:
	Matrix(size_t row, size_t column) {
		rows = row;
		columns = column;
		m = new Array[rows];
		for (int i = 0; i < rows; i++) {
			m[i].declaration(columns);
		}
	}
	~Matrix() {
		delete[] m;
	}
	size_t getRows() {
		return rows;
	}
	size_t getColumns() {
		return columns;
	}
	void show() const
	{
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				printf("%3d", m[i].getter(j));
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
	void operator *=(int a) {
		for (int i = 0; i < rows; i++) {
			for (int j = 1; j <= columns; j++) {
				m[i][j] *= a;
			}
		}
	}
	bool operator==(const Matrix &B) 
	{
		if (this == &B)return true;
		else if (!(rows == B.rows && columns == B.columns))return false;
		else {
			for (int i = 0; i < rows; i++) {
				for (int j = 1; j <= columns; j++) {
					if (m[i][j] != B.m[i][j]) return false;
				}
			}
		}
		return true;
	}
	bool operator !=(Matrix &B)
	{
		return (!(*this == B));
	}
	Array& operator[](size_t i) {
		try {
			if (i < 1 || i > rows) throw std::out_of_range("");
			return m[i - 1];
		}
		catch (std::out_of_range) {
			std::cout << "Выход за пределы матрицы." << std::endl;
			return errorArray;
		}
	}
};

std::ostream& operator<<(std::ostream& out, Proxy a) {
	out << a.getter();
	return out;
}