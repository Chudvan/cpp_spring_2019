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
		ofile.write(reinterpret_cast<char *>(&ar[i]), sizeof(uint64_t));
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

	system("pause>nul");
	return 0;
}