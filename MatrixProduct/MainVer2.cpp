#include <stdio.h>
#include <chrono>
#include <string.h>
#include <stdlib.h>
#include <vector>

using namespace std;
using namespace chrono;

#pragma region Timer
class Timer {
private:
	char* name;
	system_clock::time_point start;
	system_clock::time_point end;
public:
	Timer(char* name) : name(name) {}
	void On() {
		start = system_clock::now();
	}
	void Off() {
		end = system_clock::now();
	}
	void print() {
		printf("%s : %f ms\n", name, (float)duration_cast<chrono::microseconds>(end - start).count() / 1000);
	}
};
#pragma endregion
#pragma region AccumulateTimer
class AccumulateTimer {
private:
	char* name;
	system_clock::time_point start;
	system_clock::time_point end;
	float time = 0;
public:
	AccumulateTimer(char* name) : name(name) {}
	void On() {
		start = system_clock::now();
	}
	void Off() {
		end = system_clock::now();
		time += (float)duration_cast<chrono::microseconds>(end - start).count() / 1000;
	}
	void print() {
		printf("%s : %f ms\n", name, time);
	}
};
#pragma endregion

class Matrix { // Square Matrix, n*n : n is 2^k
private:
	void printError() const {
		printf("[Error]\n");
		exit(0); // error
	}
public:
	int* buffer = nullptr;
	int size;
	Matrix(int size) {
		this->size = size;
		buffer = (int*)malloc(sizeof(int) * size * size);
		memset(buffer, 0, sizeof(int) * size * size);
	}
	Matrix(const Matrix& mat) : size(mat.size) {
		buffer = (int*)malloc(sizeof(int) * mat.size * mat.size);
		memcpy(buffer, mat.buffer, sizeof(int) * mat.size * mat.size);
	}
	~Matrix() {
		if (buffer != nullptr)
			free(buffer);
	}
	Matrix operator+(const Matrix& other) {
		if (size != other.size)
			printError();
		Matrix matrix(size);
		for (int i = 0; i < size * size; i++)
			matrix.buffer[i] = buffer[i] + other.buffer[i];
		return matrix;
	}
	Matrix operator-(const Matrix& other) {
		if (size != other.size)
			printError();
		Matrix matrix(size);
		for (int i = 0; i < size * size; i++)
			matrix.buffer[i] = buffer[i] - other.buffer[i];
		return matrix;
	}
	Matrix& operator=(const Matrix& other) {
		if (buffer != nullptr)
			free(buffer);
		size = other.size;
		buffer = (int*)malloc(sizeof(int) * other.size * other.size);
		memcpy(buffer, other.buffer, sizeof(int) * other.size * other.size);
		return *this;
	}
	Matrix operator*(const Matrix& other) {
		if (size != other.size)
			printError();
		Matrix ret(size);

		for (int row = 0; row < size; row++)
			for (int col = 0; col < size; col++)
				for (int dot = 0; dot < size; dot++)
					ret.buffer[size * row + col] += buffer[size * row + dot] * other.buffer[size * dot + col];
		return ret;
	}
	Matrix Strassen(const Matrix& other) {
		if (size != other.size)
			printError();
		if (size <= 32) {
			return *this * other;
		}
		Matrix ret(size);
		Matrix a11(size / 2);
		Matrix a12(size / 2);
		Matrix a21(size / 2);
		Matrix a22(size / 2);
		Matrix b11(size / 2);
		Matrix b12(size / 2);
		Matrix b21(size / 2);
		Matrix b22(size / 2);
		split(a11, a12, a21, a22);
		other.split(b11, b12, b21, b22);

		Matrix m1 = (a11 + a22).Strassen(b11 + b22);
		Matrix m2 = (a21 + a22).Strassen(b11);
		Matrix m3 = a11.Strassen(b12 - b22);
		Matrix m4 = a22.Strassen(b21 - b11);
		Matrix m5 = (a11 + a12).Strassen(b22);
		Matrix m6 = (a21 - a11).Strassen(b11 + b12);
		Matrix m7 = (a12 - a22).Strassen(b21 + b22);

		ret.combine(m1 + m4 - m5 + m7, m3 + m5, m2 + m4, m1 + m3 - m2 + m6);
		return ret;
	}
	void split(Matrix& a11, Matrix& a12, Matrix& a21, Matrix& a22) const {
		if (a11.size != a12.size || a12.size != a21.size || a21.size != a22.size || a22.size != size / 2)
			printError();
		for (int i = 0; i < a11.size; i++)
			for (int j = 0; j < a11.size; j++) {
				a11.buffer[i * a11.size + j] = buffer[i * size + j];
				a12.buffer[i * a12.size + j] = buffer[i * size + j + a12.size];
				a21.buffer[i * a21.size + j] = buffer[(i + a21.size) * size + j];
				a22.buffer[i * a22.size + j] = buffer[(i + a22.size) * size + j + a22.size];
			}
	}
	void combine(const Matrix& a11, const Matrix& a12, const Matrix& a21, const Matrix& a22) {
		if (a11.size != a12.size || a12.size != a21.size || a21.size != a22.size || a22.size != size / 2)
			printError();
		for (int i = 0; i < a11.size; i++)
			for (int j = 0; j < a11.size; j++) {
				buffer[i * size + j] = a11.buffer[i * a11.size + j];
				buffer[i * size + j + a12.size] = a12.buffer[i * a12.size + j];
				buffer[(i + a21.size) * size + j] = a21.buffer[i * a21.size + j];
				buffer[(i + a22.size) * size + j + a22.size] = a22.buffer[i * a22.size + j];
			}
	}
};


int main(int argc, char* argv[]) {

#pragma region setting
	bool binary = true; // true : binary, false : int
	if (argc < 5) {
		printf("[Error] command  matrixA_FilePath  matrixB_FilePath naive_FilePath strassen_FilePath\n");
		exit(0);
	}
	FILE* FileA;
	FILE* FileB;
	FILE* NaiveResultFile;
	FILE* StrassenResultFile;
	fopen_s(&FileA, argv[1], "rb");
	fopen_s(&FileB, argv[2], "rb");
	fopen_s(&NaiveResultFile, argv[3], "wb");
	fopen_s(&StrassenResultFile, argv[4], "wb");

	if (FileA == NULL || FileB == NULL || NaiveResultFile == NULL || StrassenResultFile == NULL) {
		printf("[File ERROR]\n");
		return -1;
	}
	Matrix A(1024);
	Matrix B(1024);

	for (int i = 0; i < 1024 * 1024; i++) {
		fscanf_s(FileA, "%c", &A.buffer[i]);
		fscanf_s(FileB, "%c", &B.buffer[i]);
	}

	printf("[Success] Read File\n");

	Timer naiveTimer((char*)"Naive");
	Timer strassenTimer((char*)"strassen");
#pragma endregion

	naiveTimer.On();
	Matrix C = A * B;
	naiveTimer.Off();

	if (binary)
		fwrite(&C.buffer[0], sizeof(int), 1024 * 1024, NaiveResultFile);
	else
		for (int i = 0; i < 1024; i++)
			for (int j = 0; j < 1024; j++)
				fprintf(NaiveResultFile, "%d\n", C.buffer[i * 1024 + j]);

	fclose(NaiveResultFile);


	strassenTimer.On();
	Matrix D = A.Strassen(B);
	strassenTimer.Off();

	if (binary)
		fwrite(&D.buffer[0], sizeof(int), 1024 * 1024, StrassenResultFile);
	else
		for (int i = 0; i < 1024; i++)
			for (int j = 0; j < 1024; j++)
				fprintf(StrassenResultFile, "%d\n", D.buffer[i * 1024 + j]);

	fclose(StrassenResultFile);

	naiveTimer.print();
	strassenTimer.print();
}


