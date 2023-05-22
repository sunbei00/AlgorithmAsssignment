//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include <vector>
//#include <algorithm>
//
//typedef struct matrix {
//	size_t row;
//	size_t col;
//	matrix(size_t row, size_t col) {
//		this->row = row;
//		this->col = col;
//	}
//}Mat;
//
//int main(int argc, char* argv[]) {
//
//	if (argc < 2) {
//		printf("[Command]Command FilePath");
//		return 0;
//	}
//
//	FILE* file;
//	file = fopen(argv[1], "r");
//	if (file == NULL) {
//		printf("File Not Found");
//		return 0;
//	}
//	std::vector<Mat> list;
//	list.reserve(1000);
//	int read;
//	while (!feof(file)) {
//		fscanf(file, "%d\n", &read);
//
//		if (list.size() != 0)
//			list[list.size() - 1].col = read;
//		list.push_back(Mat(read, 0));
//
//	}
//	list.pop_back(); // for delete last { read, 0 }
//
//	size_t size = list.size();
//
//	std::vector<std::vector<size_t>> calc;
//	std::vector<std::vector<size_t>> path;
//	path.resize(size);
//	calc.resize(size);
//	for (size_t i = 0; i < size; i++) {
//		path[i].resize(size, 9999999999); // infinite value
//		calc[i].resize(size, 9999999999); // infinite value
//	}
//	for (size_t i = 0; i < size; i++) {
//		calc[i][i] = 0;
//	}
//
//	for (size_t diagonal = 0; diagonal <= size - 2; diagonal++) {
//		for (size_t i = 0; i <= size - 2 - diagonal; i++) {
//			size_t j = 1 + diagonal;
//			for (size_t k = i; k < j; k++) {
//				if (calc[i][j] > calc[i][k] + calc[k + 1][j] + list[i].row * list[k].col * list[i].col) {
//					calc[i][j] = calc[i][k] + calc[k + 1][j] + list[i].row * list[k].col * list[i].col;
//					path[i][j] = k;
//				}
//			}
//		}
//	}
//
//
//
//	fclose(file);
//
//	return 0;
//}
