#include <stdio.h>
#include <string.h>

#include <vector>									// �˰����� �ùٸ��� ���ϱ� ����
#include <algorithm>								// �˰����� �ùٸ��� ���ϱ� ����


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
		if (size + 1 > capacity) { // �迭�� �۴ٸ� �� ū �迭�� ���� ���� ���� �����Ѵ�.
			int* tmp = new int[2 * capacity];
			memcpy(tmp, array, sizeof(int) * capacity);
			delete[] array;
			array = tmp;
			capacity *= 2;
		}
		array[size] = value;
		int pos = size;
		size++;

		// �� �ؿ� ������ ���� Ʈ���� �������� �ø��� ����.
		// ������������ ������ ���̹Ƿ�, ���� ���� Ʈ���� �������� �ö󰣴�.
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
		array[0] = array[size]; // �迭�� �� �������� ����� ���� Ʈ���� head�� �����Ѵ�.

		// �ش� ���� ������ �����鼭 HeapSort�� �����ϰ� ���� ������ ���� �ø���.
		int pos = 0;
		while (2 * pos + 1 < size) { // ������ �ڽ� ��尡 �������� �� (������ ��尡 �����Ϸ��� ���� ��尡 �̹� �����ؾ� �ϱ� ����
			int swapIndex = 2 * pos + 1; // ���� �ڽ�
			if (2 * pos + 2 < size && array[2 * pos + 2] < array[2 * pos + 1])   // ������ �ڽ��� �����ϰ� �������� �� ������
				swapIndex = 2 * pos + 2;
			if (array[swapIndex] < array[pos]) { // ������������ ������ ���̹Ƿ�, ���� ���� Ʈ���� �������� �ö󰣴�.
				std::swap(array[pos], array[swapIndex]);
				pos = swapIndex;
			}
			else
				break; // �ڽ� ��庸�� ���� ��� �״�� �����ϰ� ������ �ȴ�.
		}
		return true;
	}
	int getSize() const {
		return size;
	}
};


int main(int argc, char* argv[]) {
	if (argc < 3) {			  // ���ڰ� ������ ���
		printf("[command]> command src dest");
		return -1;
	}
	HeapSort heapSort(50000);
	std::vector<int> our;										// �˰����� �ùٸ��� ���ϱ� ����
	std::vector<int> correct;									// �˰����� �ùٸ��� ���ϱ� ����
	our.reserve(50000);											// �˰����� �ùٸ��� ���ϱ� ����
	correct.reserve(50000);										// �˰����� �ùٸ��� ���ϱ� ����

	FILE* src;
	FILE* dest;
	fopen_s(&src, argv[1], "r");
	fopen_s(&dest, argv[2], "w+");

	if (src == NULL) {
		printf("[ERROR] Can't find src file");
		return -1;
	}

	int read;
	while (fscanf_s(src,"%d",&read) != EOF) { // ������ �����͸� ����
		heapSort.push(read);
		correct.push_back(read);									// �˰����� �ùٸ��� ���ϱ� ����
	}
	while (heapSort.getSize() != 0) {		  // Heap Tree�� ����� ���� ������ ����
		if (heapSort.pop(read) == true) {
			fprintf(dest, "%d\n", read);
			our.push_back(read);									// �˰����� �ùٸ��� ���ϱ� ����
		}
	}

	bool resultCheck = true;										// �˰����� �ùٸ��� ���ϱ� ����
	std::sort(correct.begin(), correct.end());						// �˰����� �ùٸ��� ���ϱ� ����
	for (int i = 0; i < std::max(correct.size(), our.size()); i++)  // �˰����� �ùٸ��� ���ϱ� ����
		if (correct[i] != our[i])									// �˰����� �ùٸ��� ���ϱ� ����
			resultCheck = false;									// �˰����� �ùٸ��� ���ϱ� ����


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