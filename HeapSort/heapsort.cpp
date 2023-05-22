#include <stdio.h>
#include <string.h>

#include <vector>									// 알고리즘이 올바른지 비교하기 위해
#include <algorithm>								// 알고리즘이 올바른지 비교하기 위해


class HeapSort {
	int* array = NULL;
	int size = 0;
	int capacity;
public:
	HeapSort(int capacity) {
		array = new int[capacity];
		this->capacity = capacity;
		
	}
	~HeapSort() {
		if(array != NULL)
			delete[] array;
	}
	void push(int value) {
		if (size + 1 > capacity) { // 배열이 작다면 더 큰 배열을 만들어서 이전 값을 저장한다.
			int* tmp = new int[2 * capacity];
			memcpy(tmp, array, sizeof(int) * capacity);
			delete[] array;
			array = tmp;
			capacity *= 2;
		}
		array[size] = value;
		int pos = size;
		size++;

		// 맨 밑에 저장한 값을 트리의 위쪽으로 올리는 역할.
		// 오름차순으로 저장할 것이므로, 작은 값이 트리의 위쪽으로 올라간다.
		while (pos != 0 && array[pos] < array[(pos - 1) / 2]) { // 
			std::swap(array[pos], array[(pos - 1) / 2]);
			pos = (pos - 1) / 2;
		}

	}
	bool pop(int& ret) {
		if (size == 0)
			return false;
		ret = array[0];
		size--;
		array[0] = array[size]; // 배열의 맨 마지막에 저장된 값을 트리의 head에 저장한다.

		// 해당 값을 밑으로 내리면서 HeapSort를 유지하게 작은 노드들을 위로 올린다.
		int pos = 0;
		while (2 * pos + 1 < size) { // 왼쪽의 자식 노드가 존재했을 때 (오른쪽 노드가 존재하려면 왼쪽 노드가 이미 존재해야 하기 때문
			int swapIndex = 2 * pos + 1; // 왼쪽 자식
			if (2 * pos + 2 < size && array[2 * pos + 2] < array[2 * pos + 1])   // 오른쪽 자식이 존재하고 오른쪽이 더 작으면
				swapIndex = 2 * pos + 2;
			if (array[swapIndex] < array[pos]) { // 오름차순으로 저장할 것이므로, 작은 값이 트리의 위쪽으로 올라간다.
				std::swap(array[pos], array[swapIndex]);
				pos = swapIndex;
			}
			else
				break; // 자식 노드보다 작을 경우 그대로 유지하고 끝내면 된다.
		}
		return true;
	}
	int getSize() const {
		return size;
	}
};


int main(int argc, char* argv[]) {
	if (argc < 3) {			  // 인자가 부족할 경우
		printf("[command]> command src dest");
		return -1;
	}
	HeapSort heapSort(50000);
	std::vector<int> our;										// 알고리즘이 올바른지 비교하기 위해
	std::vector<int> correct;									// 알고리즘이 올바른지 비교하기 위해
	our.reserve(50000);											// 알고리즘이 올바른지 비교하기 위해
	correct.reserve(50000);										// 알고리즘이 올바른지 비교하기 위해

	FILE* src;
	FILE* dest;
	fopen_s(&src, argv[1], "r");
	fopen_s(&dest, argv[2], "w+");

	if (src == NULL) {
		printf("[ERROR] Can't find src file");
		return -1;
	}

	int read;
	while (fscanf_s(src,"%d",&read) != EOF) { // 파일의 데이터를 읽음
		heapSort.push(read);
		correct.push_back(read);									// 알고리즘이 올바른지 비교하기 위해
	}
	while (heapSort.getSize() != 0) {		  // Heap Tree에 저장된 값을 꺼내서 정렬
		if (heapSort.pop(read) == true) {
			fprintf(dest, "%d\n", read);
			our.push_back(read);									// 알고리즘이 올바른지 비교하기 위해
		}
	}

	bool resultCheck = true;										// 알고리즘이 올바른지 비교하기 위해
	std::sort(correct.begin(), correct.end());						// 알고리즘이 올바른지 비교하기 위해
	for (int i = 0; i < std::max(correct.size(), our.size()); i++)  // 알고리즘이 올바른지 비교하기 위해
		if (correct[i] != our[i])									// 알고리즘이 올바른지 비교하기 위해
			resultCheck = false;									// 알고리즘이 올바른지 비교하기 위해


	if (resultCheck == true) {
		printf("***************************\n");
		printf("Correct\n");
		printf("***************************\n");
	}
	else {
		printf("***************************\n");
		printf("Fail\n");
		printf("***************************\n");
	}

	fclose(src);
	fclose(dest);

	return 0;
}