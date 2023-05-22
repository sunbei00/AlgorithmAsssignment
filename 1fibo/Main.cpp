#include <stdio.h>
#include <chrono>

using namespace std;
using namespace chrono;

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
		printf("%s : %f ms\n", name,(float)duration_cast<chrono::microseconds>(end-start).count() / 1000);
	}
};


long long fib_re(int n) {
	if (n <= 1)
		return n;
	return fib_re(n - 1) + fib_re(n-2);
}

long long fib_for(int n) {
	long long ret;
	long long* array = new long long[n+1];
	array[0] = 0;
	array[1] = 1;

	for (int i = 2; i <= n; i++) 
		array[i] = array[i - 2] + array[i - 1];
	
	ret = array[n];
	delete[] array;
	return ret;
}


void main() {
	int n;
	printf("n : ");
	scanf_s("%d", &n);
	if (n < 0) {
		printf("it can input [0,2^31)");
		return;
	}

	long long ret1;
	long long ret2;

	Timer reTimer((char*)"recycle");
	Timer forTimer((char*)"for");

	reTimer.On();
	ret1 = fib_re(n);
	reTimer.Off();

	forTimer.On();
	ret2 = fib_for(n);
	forTimer.Off();

	if (ret1 != ret2)
		printf("value is different, re : %lld, for : %lld\n",ret1, ret2);
	else
		printf("value is same, re : %lld, for : %lld\n",ret1,ret2);

	reTimer.print();
	forTimer.print();
}




// https://en.wikipedia.org/wiki/Millisecond : ms 단위 확인
// chrono 헤더파일 확인 https://two-parks.tistory.com/32
// [0] 0 , [1] 1, [2] 1, ,,,
// printf format 확인 https://woogyun.tistory.com/391