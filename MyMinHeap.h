#ifndef _MYMINHEAP_H_
#define _MYMINHEAP_H_

#include <vector>
#include "n_puzzle.h"

class MyMinHeap{
private:
	std::vector<n_puzzle::dataNode*> heap;

	bool compare(n_puzzle::dataNode *a, n_puzzle::dataNode *b);

public:
	MyMinHeap();
	~MyMinHeap();
	bool isEmpty();
	int size();
	void insert(n_puzzle::dataNode *item);
	n_puzzle::dataNode* find_root();
	void delete_root();
	void build_heap();
	void heapify(int n);
	void housekeeping(n_puzzle::dataNode* node, int index);
	int inHeap(int state[], int size);
};

#endif