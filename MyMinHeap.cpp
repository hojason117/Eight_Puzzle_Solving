#include "MyMinHeap.h"
#include "n_puzzle.h"
#include <vector>
#include <algorithm>

using namespace std;

MyMinHeap::MyMinHeap() {
	n_puzzle::dataNode *temp = new n_puzzle::dataNode;
	heap.push_back(temp);
}

MyMinHeap::~MyMinHeap() {
	delete heap[0];
	heap.pop_back();
}

bool MyMinHeap::isEmpty() {
	return (heap.size() == 1)? true : false;
}

int MyMinHeap::size() {
	return heap.size()-1;
}

void MyMinHeap::insert(n_puzzle::dataNode *item) {
	heap.push_back(item);

	int n = heap.size()-1;
	while(compare(heap[n], heap[n/2])) {
		swap(heap[n], heap[n/2]);
		n /= 2;
	}
}

n_puzzle::dataNode* MyMinHeap::find_root() {
	return heap[1];
}

void MyMinHeap::delete_root() {
	swap(heap[1], heap[heap.size()-1]);
	heap.pop_back();
	heapify(1);
}

void MyMinHeap::build_heap() {
	for(int i = (heap.size()-1) / 2; i > 0; i--)
		heapify(i);
}

void MyMinHeap::heapify(int n) {
	while(n*2 <= heap.size()-1) {
		if(n*2+1 <= heap.size()-1) {									//two child
			int less;
			less = compare(heap[n*2], heap[n*2+1])? n*2 : n*2+1;		//find the smaller child
			if(compare(heap[less], heap[n])) {
				swap(heap[less], heap[n]);
				n = less;
			}
			else
				break;
		}
		else {															//one child
			if(compare(heap[n*2], heap[n])) {
				swap(heap[n*2], heap[n]);
				n *= 2;
			}
			else
				break;
		}
	}
}

void MyMinHeap::housekeeping(n_puzzle::dataNode* node, int index) {
	if(compare(node, heap[index])) {
		heap[index]->action = node->action;
		heap[index]->estimated_cost = node->estimated_cost;
		heap[index]->parent = node->parent;
		heap[index]->total_cost = node->total_cost;
		
		delete[] node->state;
		delete node;

		while(compare(heap[index], heap[index/2])) {
			swap(heap[index], heap[index/2]);
			index /= 2;
		}
	}
}

int MyMinHeap::inHeap(int state[], int size) {
	int i, j;

	for(i = 1; i < heap.size(); i++) {
		for(j = 0; j < size; j++) {
			if(heap[i]->state[j] != state[j])
				break;
		}

		if(j == size)
			return i;
	}

	return 0;
}

bool MyMinHeap::compare(n_puzzle::dataNode *a, n_puzzle::dataNode *b) {
	return (a->total_cost + a->estimated_cost) <= (b->total_cost + b->estimated_cost);
}