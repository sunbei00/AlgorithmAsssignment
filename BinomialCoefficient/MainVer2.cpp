#include <stdio.h>
#include <fstream>
#include <chrono>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>

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

unsigned int reserveSize = 1000; // 글자 수
const unsigned char none = 255; // 초기 값

class infByte {
public: // 리틀엔디안 방식 저장.
	vector<unsigned char> data;
	infByte() {
		data.reserve(reserveSize);
		for (int i = 0; i < reserveSize; i++)
			data.push_back(none);
	}
	infByte(const infByte& other) {
		data = other.data;
	}
	infByte& operator=(const infByte& other) {
		data = other.data;
		return *this;
	}
	infByte operator+(const infByte& other) const {
		infByte ret;

		// 각 원소를 더하기.
		for (int i = 0; i < reserveSize; i++) { 
			if (data[i] == none && other.data[i] == none)
				break;
			else if (data[i] != none && other.data[i] != none) 
				ret.data[i] = data[i] + other.data[i];
			else if (data[i] != none)
				ret.data[i] = data[i];
			else if (other.data[i] != none)
				ret.data[i] = other.data[i];
		}

		// 각 원소가 10 이상이면 Carry
		for (int i = 0; i < reserveSize; i++) { 
			if (ret.data[i] == none)
				break;
			if (ret.data[i] / 10 != 0) {
				if (ret.data[i + 1] != none)
					ret.data[i + 1] += ret.data[i] / 10;
				else
					ret.data[i + 1] = ret.data[i] / 10;
				ret.data[i] = ret.data[i] % 10;
			}
		}
		return ret;
	}
};

infByte** buffer = nullptr;

void Release(int row) { // 버퍼 해제
	if (buffer != nullptr) {
		for (int r = 0; r <= row; r++)
			if (buffer[r] != nullptr)
				delete[] buffer[r];
		delete[] buffer;
		buffer = nullptr;
	}
}

void Setting(int row, int col) { // 버퍼 초기화
	Release(row);

	buffer = new infByte * [row + 1];
	for (int r = 0; r <= row; r++)
		buffer[r] = new infByte[min(col + 1, r + 1)];
	// n과 k는 0~n , 0~k 중 선택할 수 있으므로, 1씩 더한 배열을 만들었다.
}

void printError() { // argument 오류 시
	printf("[Command] command n k\n");
	printf("[Condition] unsgined : n,k | k <= n\n");
	exit(0);
}

infByte binomialCoefficient(int n, int k) {
	// if (n - k < k)
	//	k = n - k;
	// if (k == 0)
	//	return 1;
	// 최적화를 위해서는 필요하지만, 성능을 확인하기 위해서 주석처리.

	infByte initData; // 값 : 1
	initData.data[0] = 1;

	for (int row = 0; row <= n; row++) {
		for (int col = 0; col <= min(k, row); col++) {
			if (col == 0 || col == row)
				buffer[row][col] = initData;
			else
				buffer[row][col] = buffer[row - 1][col - 1] + buffer[row - 1][col];
		}
	}
	return buffer[n][k];
}

int main(int argc, char* argv[]) {

	if (argc < 3)
		printError();
	int n = atoi(argv[1]);
	int k = atoi(argv[2]);
	if (k > n || n < 0 || k < 0)
		printError();

	Timer timer((char*)"Timer");

	Setting(n, k);
	
	timer.On();
	infByte result = binomialCoefficient(n, k);
	timer.Off();
	
	for (int i = reserveSize - 1; i >= 0; i--)
		if (result.data[i] != none)
			printf("%d", result.data[i]);
	printf("\n\n");

	timer.print();

	Release(n);
}