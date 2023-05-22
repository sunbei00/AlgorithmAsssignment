#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>

typedef struct Obejct {
	float price;
	float weight;
	int index;

	bool operator<(const Obejct& other) {
		// 단위 무게 당 가치, 내림차순 정리
		if (price / weight > other.price / other.weight)
			return true;
		else
			return false;
	}
} Obj;


int main(int argc, char* argv[]) {
	int weight;
	float currentPrice = 0;
	int currentWeight = 0;

	if (argc < 2) {
		printf("[Command] $command weight\n");
		exit(1);
	}
	weight = atoi(argv[1]);
	if (weight <= 0) {
		printf("[Error] weight's min value is 1\n");
		printf("[Error] input weight is %d\n", weight);
		exit(2);
	}

	FILE* priceFile;
	FILE* weightFile;
	fopen_s(&priceFile, "./price.txt", "r");
	fopen_s(&weightFile, "./weight.txt", "r");

	if (priceFile == NULL || weightFile == NULL) {
		printf("[Error] Files is NULL");
		exit(3);
	}


	std::vector<Obj> vec;
	vec.reserve(500);

	int priceInput;
	int weightInput;
	int count = 0;
	while (!feof(priceFile) || !feof(weightFile)) {
		fscanf_s(priceFile, "%d\n", &priceInput);
		fscanf_s(weightFile, "%d\n", &weightInput);
		vec.push_back({ (float)priceInput,(float)weightInput, (int)count++});
	}

	fclose(priceFile);
	fclose(weightFile);
	std::sort(vec.begin(), vec.end()); 

	printf("순서 : ");
	std::vector<Obj>::iterator it;
	for (it = vec.begin(); it != vec.end(); it++) {
		if (currentWeight + it->weight > weight)
			break;
		currentWeight += it->weight;
		currentPrice += it->price;
		printf("%d ",it->index);
	}

	int remaining = weight - currentWeight;
	currentWeight = weight;
	currentPrice += remaining * (it->price / it->weight);
	if(remaining != 0)
		printf("%d ", it->index);
		
	printf("\nInput Weight : %d, Price : %.2f", weight ,currentPrice);

	return 0;
}