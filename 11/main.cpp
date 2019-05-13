#include "sort.h"

int main()
{
	setlocale(LC_ALL, "");
	std::string ofname = "file.out";
	std::string ofnameSort = "fileSort.out";
	std::ofstream ofile(ofname, std::ios::binary);
	size_t count = 5;
	uint64_t *ar = new uint64_t[count];
	std::cout << "Запись в файл:" << std::endl;
	for (size_t i = 0; i < count; i++) {
		std::cin >> ar[i];
		if (!ofile.write(reinterpret_cast<char *>(&ar[i]), sizeof(uint64_t))) {
			throw std::runtime_error("can't write in " + ofname + " file.");
		}
	}
	ofile.close();
	//Сортировка
	Sort sort;
	sort(ofname, ofnameSort);
	std::cout << "Чтение из файла:" << std::endl;
	std::ifstream ifile(ofnameSort, std::ios::binary);
	int i = 0;
	while (!ifile.read(reinterpret_cast<char *>(&ar[i]), sizeof(uint64_t)).eof()) {
		std::cout << ar[i] << std::endl;
		i++;
	}
	ifile.close();

	/*Сортируем ещё раз, для чистоты эксперимента.*/
	std::cout << "Запись в файл:" << std::endl;
	std::ofstream ofile2(ofname, std::ios::binary);
	for (size_t i = 0; i < count; i++) {
		std::cin >> ar[i];
		if (!ofile2.write(reinterpret_cast<char *>(&ar[i]), sizeof(uint64_t))) {
			throw std::runtime_error("can't write in " + ofname + " file.");
		}
	}
	ofile2.close();

	sort(ofname, ofnameSort);

	std::cout << "Чтение из файла:" << std::endl;
	std::ifstream ifile2(ofnameSort, std::ios::binary);
	i = 0;
	while (!ifile2.read(reinterpret_cast<char *>(&ar[i]), sizeof(uint64_t)).eof()) {
		std::cout << ar[i] << std::endl;
		i++;
	}
	ifile2.close();
	delete[] ar;
	system("pause>nul");
	return 0;
}