#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <queue>

typedef struct Object {
	float price;
	float weight;
	int index;

	bool operator<(const Object& other) {
		// 단위 무게 당 가치, 내림차순 정리
		if (price / weight > other.price / other.weight)
			return true;
		else
			return false;
	}
} Obj;

std::vector<Obj> vec;
int limitWeight;

class Node {
public:
	struct comp {
		bool operator()(Node* n1, Node* n2) {
			return n1->boundary < n2->boundary;
		}
	};
	static std::priority_queue<Node*, std::vector<Node*>, comp> priority;
	static Node* currentMaxPriceNode;
	static int currentMaxPrice;;
	int price;
	int havingWeight;
	int boundary;
	int level;
	bool isContained;
	Node* containedNode = NULL;
	Node* nContainedNode = NULL;
	Node* parent = NULL;

	Node(int price, int havingWeight, int level, bool isContained, Node* parent) {
		this->price = price;
		this->havingWeight = havingWeight;
		this->level = level;
		this->isContained = isContained;
		getboundary();
		this->parent = parent;

		if (currentMaxPrice < this->price) {
			currentMaxPrice = this->price;
			currentMaxPriceNode = this;
		}
	}

	~Node() {
		if (containedNode != NULL)
			delete containedNode;
		if (nContainedNode != NULL)
			delete nContainedNode;
	}

	void getboundary() {
		int now = havingWeight;
		boundary = price;

		std::vector<Obj>::iterator it = vec.begin();

		it += (level + 1);
		for (; it != vec.end(); it++) {
			if (now + it->weight > limitWeight)
				break;
			now += it->weight;
			boundary += it->price;
		}
		int remaining = limitWeight - now;
		if (it != vec.end())
			boundary += remaining * (it->price / it->weight);
	}

	void makeNode() {
		if (currentMaxPrice > boundary)
			return;
		if (level + 1 >= vec.size())
			return;

		if (havingWeight + vec[level + 1].weight <= limitWeight) {
			containedNode = new Node(price + vec[level + 1].price, havingWeight + vec[level + 1].weight, level + 1, true, this);
			priority.push(containedNode);
		}

		nContainedNode = new Node(price, havingWeight, level + 1, false, this);
		priority.push(nContainedNode);
	}

};



int main(int argc, char* argv[]) {

	if (argc < 2) {
		printf("[Command] $command weight\n");
		exit(1);
	}
	limitWeight = atoi(argv[1]);
	if (limitWeight <= 0) {
		printf("[Error] weight's min value is 1\n");
		printf("[Error] input weight is %d\n", limitWeight);
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


	vec.reserve(500);

	int priceInput;
	int weightInput;
	int count = 0;
	while (!feof(priceFile) || !feof(weightFile)) {
		fscanf_s(priceFile, "%d\n", &priceInput);
		fscanf_s(weightFile, "%d\n", &weightInput);
		vec.push_back({ (float)priceInput,(float)weightInput,count++ });
	}

	fclose(priceFile);
	fclose(weightFile);

	std::sort(vec.begin(), vec.end());

	Node* startNode = new Node(0, 0, -1, false, NULL);
	startNode->makeNode();

	while (!Node::priority.empty()) {
		Node* node = Node::priority.top();
		Node::priority.pop();
		node->makeNode();
	}

	printf("optimized price : %d\n", Node::currentMaxPriceNode->price);
	printf("having weight : %d\n\n", Node::currentMaxPriceNode->havingWeight);


	Node* output = Node::currentMaxPriceNode;
	std::vector<Node*> optimized; 
	while (true) {
		if (output->isContained)
			optimized.push_back(output);
		output = output->parent;
		if (output->parent == NULL)
			break;
	}

	for (int i = optimized.size() - 1; i != -1; i--) {
		printf("%d / ", vec[optimized[i]->level].index);
	}
	printf("\n\n");
	printf("\n");
	printf("(Object Index , accumulate price, accumulate weight)\n");
	for (int i = optimized.size() - 1; i != -1; i--) {
		printf("(%5d , %5d , %5d)\t\t", vec[optimized[i]->level].index, optimized[i]->price, optimized[i]->havingWeight);
	}

	delete startNode;

	return 0;
}


std::priority_queue<Node*, std::vector<Node*>, Node::comp> Node::priority;
Node* Node::currentMaxPriceNode;
int Node::currentMaxPrice = -1;