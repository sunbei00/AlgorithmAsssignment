//#include <stdio.h>
//#include <fstream>
//#include <chrono>
//#include <string.h>
//#include <stdlib.h>
//#include <vector>
//#include <algorithm>
//
//using namespace std;
//using namespace chrono;
//
//
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
//typedef long long ll; // Short is the best..
//ll** buffer = nullptr;
//
//void Release(int row) {
//	if (buffer != nullptr) {
//		for (int r = 0; r <= row; r++)
//			if (buffer[r] != nullptr)
//				delete[] buffer[r];
//		delete[] buffer;
//		buffer = nullptr;
//	}
//}
//
//void Setting(int row, int col) {
//	Release(row);
//
//	buffer = new ll*[row+1]; 
//	for (int r = 0; r <= row; r++)
//		buffer[r] = new ll[min(col+1,r+1)]; 
//	// n과 k는 0~n , 0~k 중 선택할 수 있으므로, 1씩 더한 배열을 만들었다.
//}
//
//void printError() {
//	printf("[Command] command n k\n");
//	printf("[Condition] unsgined : n,k | k <= n\n");
//	exit(0);
//}
//
//ll binomialCoefficient(int n, int k) {
//	// if (n - k < k)
//	//	k = n - k;
//	// if (k == 0)
//	//	return 1;
//	// 최적화를 위해서는 필요하지만, 성능을 확인하기 위해서 주석처리.
//	
//	
//
//	for (int row = 0; row <= n; row++) { 
//		for (int col = 0; col <= min(k,row); col++) { 
//			if (col == 0 || col == row)
//				buffer[row][col] = 1;
//			else
//				buffer[row][col] = buffer[row - 1][col - 1] + buffer[row - 1][col];
//			printf("%.10lld ", buffer[row][col]);
//		}
//		printf("\n");
//	}
//	return buffer[n][k];
//}
//
//int main(int argc, char* argv[]) {
//
//	if (argc < 3)
//		printError();
//	int n = atoi(argv[1]);
//	int k = atoi(argv[2]);
//	if (k > n || n < 0 || k < 0)
//		printError();
//
//	Setting(n, k);
//
//	ll result = binomialCoefficient(n, k);
//	printf("result:%lld", result);
//
//	Release(n);
//}