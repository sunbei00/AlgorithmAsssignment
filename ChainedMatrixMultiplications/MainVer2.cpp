#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <vector>
#include <algorithm>

typedef struct matrix {
	size_t row;
	size_t col;
	matrix(size_t row, size_t col) {
		this->row = row;
		this->col = col;
	}
}Mat;

void orderPrint(size_t i, size_t j, FILE* file, std::vector<std::vector<size_t>>& p) {
	if (i == j)
		fprintf(file, "A%lld", i);
	else {
		fprintf(file, "(");
		orderPrint(i, p[i][j],file,p);
		orderPrint(p[i][j] + 1, j, file, p);
		fprintf(file, ")");
	}
}

int main(int argc, char* argv[]) {

	// Command Interprete
	if (argc < 3) {
		printf("[Command]Command FilePath OutputPath\n");
		return 0;
	}

	// File Read
	FILE* file;
	file = fopen(argv[1], "r");
	if (file == NULL) {
		printf("File Not Found");
		return 0;
	}
	std::vector<Mat> list;
	list.reserve(1000);
	int read;
	while (!feof(file)) {
		fscanf(file, "%d\n", &read);

		if (list.size() == 0)
			list.push_back(Mat(0, read));
		else
			list.push_back(Mat(list[list.size() - 1].col, read));
	}
	fclose(file);

	// Chained Matrix Algorithm
	int n = list.size() - 1;
	std::vector<std::vector<size_t>> d;
	std::vector<std::vector<size_t>> p;
	p.resize(list.size());
	d.resize(list.size());
	for (size_t i = 0; i < list.size(); i++) {
		p[i].resize(list.size()); 
		d[i].resize(list.size(), 9999999999); // infinite value
	}
	for (size_t i = 1; i <= n; i++) 
		d[i][i] = 0;

	for(size_t diagonal = 1; diagonal <= n-1; diagonal++)
		for (size_t i = 1; i <= n - diagonal; i++) {
			size_t j = i + diagonal;
			for (size_t k = i; k < j; k++) {
				if (d[i][j] > d[i][k] + d[k + 1][j] + list[i].row * list[k].col * list[j].col) {
					d[i][j] = d[i][k] + d[k + 1][j] + list[i].row * list[k].col * list[j].col;
					p[i][j] = k;
				}
			}			
		}

	// FileWrite
	file = fopen(argv[2], "wt");
	orderPrint(1,n,file,p);
	fclose(file);


	return 0;
}
