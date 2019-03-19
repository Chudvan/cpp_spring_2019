#include <iostream>
#include <cstdlib>
#include "numbers.dat"
using namespace std;

/*� ������ ������������ ������� isPrime ��� ������ ������� �����.
  ��������: ������� ���������.
  ���������: O(sqrt(n))
  
  ������� findIndex ������� ������ ����� � �������.
  ������ ��������: true - ��� ������ ����� �������
				   false - ��� ������ ������
  ��������: �������� �����.
  ���������: O(log(n))
  
  ������� print ������� ��������� �� �����, � ������ ������� ������.*/

bool isPrime(int n);
int findIndex(int number, bool isLeft);
void print(int input, int* numPrint);

int main(int argc, char* argv[])
{
	if ((argc - 1) % 2 || argc == 1) return -1;
	int from, to, i, j, numPrint = 0, count;
	for (int k = 1; k < argc; k += 2) {
		from = atoi(argv[k]);
		to = atoi(argv[k + 1]);
		i = findIndex(from, true);
		j = findIndex(to, false);
		if (i < 0 || j < 0)print(0, &numPrint);
		else if (j < i)print(0, &numPrint);
		else {
			count = 0;
			for (int t = i; t <= j; t++) {
				if (isPrime(Data[t]))count++;
			}
			print(count, &numPrint);
		}
	}
	return 0;
}

bool isPrime(int n) {
	if (n == 1) return false;
	for (int i = 2; i*i <= n; i++) {
		if (!(n % i)) {
			return false;
		}
	}
	return true;
}

int findIndex(int number, bool isLeft) {
	int l = 0, r = Size - 1, mid, i;
	bool flag = true;
	while ((l <= r) && flag) {
		mid = (r + l) / 2;
		if (Data[mid] == number) {
			i = mid;
			if (isLeft) {
				while ((i >= 0) && (Data[i] == number))i--;
				i++;
			}
			else {
				while ((i <= Size - 1) && (Data[i] == number))i++;
				i--;
			}
			l = mid + 1; r = mid;
		}
		else if ((l == r) && Data[l] != number) flag = false;
		else if (Data[mid] > number)r = mid;
		else l = mid + 1;
	}
	if (flag) return i;
	return -1;
}

void print(int input, int* numPrint) {
	if (*numPrint)cout << "\n" << input;
	else cout << input;
	(*numPrint)++;
}