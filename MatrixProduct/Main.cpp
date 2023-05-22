//#include <stdio.h>
//#include <chrono>
//#include <string.h>
//#include <vector>
//#include <stdlib.h>
//#include <vector>
//
//#pragma region data
//using namespace std;
//using namespace chrono;
//
//char A[1024][1024];
//char B[1024][1024];
//int naive[1024][1024];
//int strassen[1024][1024];
//
//#pragma endregion
//#pragma region Timer
//class Timer {
//private:
//	char* name;
//	system_clock::time_point start;
//	system_clock::time_point end;
//public:
//	Timer(char* name) : name(name) {}
//	void On() {
//		start = system_clock::now();
//	}
//	void Off() {
//		end = system_clock::now();
//	}
//	void print() {
//		printf("%s : %f ms\n", name, (float)duration_cast<chrono::microseconds>(end - start).count() / 1000);
//	}
//};
//#pragma endregion
//
//void naiveMatrixProduct(int* dest, int size) {
//	for (int row = 0; row < size; row++)
//		for (int col = 0; col < size; col++)
//			for (int dot = 0; dot < size; dot++)
//				dest[row * size + col] += A[row][dot] * B[dot][col];
//}
//int* matrixAdd(int* src1, int* src2, int size) {
//	int* ret = (int*)malloc(sizeof(int) * size * size);
//	for (int i = 0; i < size * size; i++)
//		ret[i] = src1[i] + src2[i];
//	return ret;
//}
//int* matrixSub(int* src1, int* src2, int size) {
//	int* ret = (int*)malloc(sizeof(int) * size * size);
//	for (int i = 0; i < size * size; i++)
//		ret[i] = src1[i] - src2[i];
//	return ret;
//}
//void split(const int* src, int* a11, int* a12, int* a21, int* a22, const int size) {
//	for (int i = 0; i < size / 2; i++) {
//		for (int j = 0; j < size / 2; j++)
//			a11[i * (size / 2) + j] = src[i * size + j];
//		for (int j = size / 2; j < size; j++)
//			a12[i * (size / 2) + j - size / 2] = src[i * size + j];
//	}
//	for (int i = size / 2; i < size; i++) {
//		for (int j = 0; j < size / 2; j++)
//			a21[(i - size / 2) * (size / 2) + j] = src[i * size + j];
//		for (int j = size / 2; j < size; j++)
//			a22[(i - size / 2) * (size / 2) + j] = src[i * size + j];
//	}
//}
//void combine(int* src, int* a11, const int* a12, const int* a21, const int* a22, const int size) {
//	for (int i = 0; i < size / 2; i++) {
//		for (int j = 0; j < size / 2; j++)
//			src[i * size + j] = a11[i * (size / 2) + j] = src[i * size + j];
//		for (int j = size / 2; j < size; j++)
//			src[i * size + j] = a12[i * (size / 2) + j - size / 2];
//	}
//	for (int i = size / 2; i < size; i++) {
//		for (int j = 0; j < size / 2; j++)
//			src[i * size + j] = a21[(i - size / 2) * (size / 2) + j];
//		for (int j = size / 2; j < size; j++)
//			src[i * size + j] = a22[(i - size / 2) * (size / 2) + j];
//	}
//}
//int* strassenAlgorithm(int* src1, int* src2, int size) {
//	int* ret = (int*)malloc(size * size * sizeof(int));;
//	if (size == 1) {
//		ret[0] = src1[0] + src2[0];
//		return ret;
//	}
//	int nDiv2 = size / 2;
//	int* a11 = (int*)malloc(sizeof(int) * nDiv2 * nDiv2);
//	int* a12 = (int*)malloc(sizeof(int) * nDiv2 * nDiv2);
//	int* a21 = (int*)malloc(sizeof(int) * nDiv2 * nDiv2);
//	int* a22 = (int*)malloc(sizeof(int) * nDiv2 * nDiv2);
//	split(src1, a11, a12, a21, a22, size);
//
//	int* b11 = (int*)malloc(sizeof(int) * nDiv2 * nDiv2);
//	int* b12 = (int*)malloc(sizeof(int) * nDiv2 * nDiv2);
//	int* b21 = (int*)malloc(sizeof(int) * nDiv2 * nDiv2);
//	int* b22 = (int*)malloc(sizeof(int) * nDiv2 * nDiv2);
//	split(src2, b11, b12, b21, b22, size);
//
//	int* a11a22 = matrixAdd(a11, a22, nDiv2);
//	int* b11b22 = matrixAdd(b11, b22, nDiv2);
//	int* m1 = strassenAlgorithm(a11a22, b11b22, nDiv2);
//	free(a11a22);
//	free(b11b22);
//
//	int* a21a22 = matrixAdd(a21, a22, nDiv2);
//	int* m2 = strassenAlgorithm(a21a22, b11, nDiv2);
//	free(a21a22);
//
//	int* b12b22 = matrixAdd(b12, b22, nDiv2);
//	int* m3 = strassenAlgorithm(a11, b12b22, nDiv2);
//	free(b12b22);
//
//	int* b21b11 = matrixAdd(b21, b11, nDiv2);
//	int* m4 = strassenAlgorithm(a22, b21b11, nDiv2);
//	free(b21b11);
//
//	int* a11a12 = matrixAdd(a11, a12, nDiv2);
//	int* m5 = strassenAlgorithm(a11a12, b22, nDiv2);
//	free(a11a12);
//
//	int* a21ma11 = matrixSub(a21, a11, nDiv2);
//	int* b11b12 = matrixAdd(b11, b12, nDiv2);
//	int* m6 = strassenAlgorithm(a21ma11, b11b12, nDiv2);
//	free(a21ma11);
//	free(b11b12);
//
//	int* a12ma22 = matrixSub(a12, a22, nDiv2);
//	int* b21b22 = matrixAdd(b21, b22, nDiv2);
//	int* m7 = strassenAlgorithm(a12ma22, b21b22, nDiv2);
//	free(b21b22);
//	free(a12ma22);
//
//	int* M1M4 = matrixAdd(m1, m4, nDiv2);
//	int* M1M4mM5 = matrixSub(M1M4, m5, nDiv2);
//	free(M1M4);
//	int* M1M4mM5M7 = matrixSub(M1M4mM5, m7, nDiv2);
//	free(M1M4mM5);
//
//	int* M3M5 = matrixAdd(m3, m5, nDiv2);
//
//	int* M2M4 = matrixAdd(m2, m4, nDiv2);
//
//	int* M1M3 = matrixAdd(m1, m3, nDiv2);
//	int* M1M3mM2 = matrixSub(M1M3, m2, nDiv2);
//	free(M1M3);
//	int* M1M3mM2M6 = matrixAdd(M1M3mM2, m6, nDiv2);
//	free(M1M3mM2);
//
//	free(b11);
//	free(b12);
//	free(b21);
//	free(b22);
//	free(a11);
//	free(a12);
//	free(a21);
//	free(a22);
//
//	combine(ret, M1M4mM5M7, M3M5, M2M4, M1M3mM2M6, size);
//
//	free(M1M4mM5M7);
//	free(M3M5);
//	free(M2M4);
//	free(M1M3mM2M6);
//
//	return ret;
//}
//
//
//int main(int argc, char* argv[]) {
//
//	if (argc < 3) {
//		printf("[Error] command  matrixA_FilePath  matrixB_FilePath\n");
//		exit(0);
//	}
//	FILE* FileA;
//	FILE* FileB;
//	fopen_s(&FileA, argv[1], "rb");
//	fopen_s(&FileB, argv[2], "rb");
//
//	if (FileA == NULL || FileB == NULL) {
//		printf("[ERROR] Can't search file\n");
//		return -1;
//	}
//
//	fread(A, sizeof(char), 1024 * 1024, FileA);
//	fread(B, sizeof(char), 1024 * 1024, FileB);
//
//	printf("[Success] Read File\n");
//
//	Timer naiveTimer((char*)"Naive");
//	Timer strassenTimer((char*)"strassen");
//
//	memset(naive, 0, sizeof(int) * 1024 * 1024);
//	memset(strassen, 0, sizeof(int) * 1024 * 1024);
//
//	naiveTimer.On();
//	naiveMatrixProduct(&naive[0][0], 1024);
//	naiveTimer.Off();
//
//	int* intA = (int*)malloc(sizeof(int) * 1024 * 1024);
//	int* intB = (int*)malloc(sizeof(int) * 1024 * 1024);
//	for (int i = 0; i < 1024; i++)
//		for (int j = 0; j < 1024; j++) {
//			intA[i * 1024 + j] = A[i][j];
//			intB[i * 1024 + j] = B[i][j];
//		}
//
//	strassenTimer.On();
//	int* result = strassenAlgorithm(intA, intB, 1024);
//	strassenTimer.Off();
//
//	naiveTimer.print();
//	strassenTimer.print();
//}
//
//
