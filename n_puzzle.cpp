#define _CRT_SECURE_NO_DEPRECATE

#include "n_puzzle.h"
#include "MyMinHeap.h"
#include <cmath>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <stack>

using namespace std;

n_puzzle::n_puzzle(int size) {
	puzzle_size = size;
	edgeLength = sqrt((double)size);
	initial_state = new int[size];
}

n_puzzle::~n_puzzle() {
	for(int i = 0; i < visited.size(); i++) {
		delete[] visited[i]->state;
		delete visited[i];
	}

	delete[] initial_state;
}

void n_puzzle::set_initial_State(std::string initial_state) {
	char *state = new char[initial_state.length()+1];
	strcpy(state, initial_state.c_str());

	char *p = strtok(state, " ");
	for(int i = 0; i < puzzle_size; i++, p = strtok(NULL, " "))
		this->initial_state[i] = atoi(p);

	delete[] state;
}

void n_puzzle::outputPath() {
	char *movement[4] = {"up", "down", "left", "right"};

	cout << "Initial state: ";
	for (int i = 0; i < puzzle_size; i++)
		cout << initial_state[i] << " ";
	cout << endl;
	
	if(isGoalState(initial_state))
		cout << "It is already in a goal state." << endl;
	else if(!isSolvable())
		cout << "No solution." << endl;
	else {
		cout << "Solution:" << endl;

		stack<dataNode*> stack;
		while(goal) {
			stack.push(goal);
			goal = goal->parent;
		}

		stack.pop();
		while(!stack.empty()) {
			cout << "move 0 to " << movement[stack.top()->action] << endl;
			stack.pop();
		}
	}
}

bool n_puzzle::isSolvable() {
	int disorder_digits = permutation_inversion();

	if(puzzle_size % 2 == 1)
		return (disorder_digits % 2 == 1)? false : true;
	else {
		int zero_pos;
		for(int i = 0; i < puzzle_size; i++) {
			if(initial_state[i] == 0)
				zero_pos = i;
		}

		return (((zero_pos/edgeLength) + disorder_digits) % 2 == 1) ? false : true;
	}
}

void n_puzzle::find_shortest_path() { 
	if(!isSolvable())
		return;

	MyMinHeap heap;
	dataNode *new_node, *mynode;
	int nextStateCount, *nextState[4], heapIndex;
	Action nextAction[4];

	for(int i = 0; i < 4; i++)
		nextState[i] = new int[puzzle_size];

	new_node = new dataNode;
	new_node->state = new int[puzzle_size];
	for(int i = 0; i < puzzle_size; i++)
		new_node->state[i] = initial_state[i];
	new_node->total_cost = 0;
	new_node->estimated_cost = heuristic(new_node->state);
	new_node->parent = NULL;
	heap.insert(new_node);
	visited.push_back(new_node);

	while(!heap.isEmpty()) {
		//cout << heap.size() << endl;

		mynode = heap.find_root();
		heap.delete_root();
		if(isGoalState(mynode->state)) {
			goal = mynode;
			break;
		}

		nextStateCount = findSuccessor(mynode->state, nextState, nextAction);
		for(int i = 0; i < nextStateCount; i++) {
			new_node = new dataNode;
			new_node->state = new int[puzzle_size];
			for(int j = 0; j < puzzle_size; j++)
				new_node->state[j] = nextState[i][j];
			new_node->action = nextAction[i];
			new_node->total_cost = mynode->total_cost + 1;
			new_node->estimated_cost = heuristic(new_node->state);
			new_node->parent = mynode;

			heapIndex = heap.inHeap(new_node->state, puzzle_size);
			if(heapIndex)
				heap.housekeeping(new_node, heapIndex);
			else if(!inVisited(new_node->state)) {
				heap.insert(new_node);
				visited.push_back(new_node);;
			}
				else {
				delete[] new_node->state;
				delete new_node;
			}
		}
	}

	for(int i = 0; i < 4; i++)
		delete[] nextState[i];
}

int n_puzzle::heuristic(int state[]) {
	int h_value = 0;

	for(int i = 0; i < puzzle_size; i++) {
		//row displacement
		if((i/edgeLength) > (state[i]/edgeLength))		//current position is higher than it should be in row
			h_value += ((i/edgeLength) - (state[i]/edgeLength));
		else											//current position is lower than or equal to it should be in row
			h_value += ((state[i]/edgeLength) - (i/edgeLength));
		//column displacement
		if((i%edgeLength) > (state[i]%edgeLength))		//current position is on the right to where it should be in cloumn
			h_value += ((i%edgeLength) - (state[i]%edgeLength));
		else											//current position is on the left or equal to where it should be in cloumn
			h_value += ((state[i]%edgeLength) - (i%edgeLength));
	}

	return h_value;
}

int n_puzzle::findSuccessor(int state[], int **nextState, Action nextAction[]) {
	int *tempState = new int[puzzle_size], successor_count = 0;
	int blankIndex = get_blank_index(state);
	bool doable;

	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < puzzle_size; j++)
			tempState[j] = state[j];

		switch(i) {
			case 0:		//move 0 UP
				if((blankIndex)/edgeLength > 0) {
					doable = true;
					nextAction[successor_count] = UP;
					swap(tempState[blankIndex], tempState[blankIndex-edgeLength]);
				}
				else
					doable = false;
				break;
			case 1:		//move 0 DOWN
				if((blankIndex)/edgeLength < edgeLength-1) {
					doable = true;
					nextAction[successor_count] = DOWN;
					swap(tempState[blankIndex], tempState[blankIndex+edgeLength]);
				}
				else
					doable = false;
				break;
			case 2:		//move 0 LEFT
				if((blankIndex)%edgeLength > 0) {
					doable = true;
					nextAction[successor_count] = LEFT;
					swap(tempState[blankIndex], tempState[blankIndex-1]);
				}
				else
					doable = false;
				break;
			case 3:		//move 0 RIGHT
				if((blankIndex)%edgeLength < edgeLength-1) {
					doable = true;
					nextAction[successor_count] = RIGHT;
					swap(tempState[blankIndex], tempState[blankIndex+1]);
				}
				else
					doable = false;
				break;
		}

		if(doable) {
			for(int j = 0; j < puzzle_size; j++)
				nextState[successor_count][j] = tempState[j];
			successor_count++;
		}
	}

	return successor_count;
}

bool n_puzzle::isGoalState(int state[]) {
	for(int i = 0; i < puzzle_size; i++) {
		if(state[i] != i)
			return false;
	}
	return true;
}

int n_puzzle::permutation_inversion() {
	int inversion_count = 0;

	for(int i = 0; i < puzzle_size; i++) {
		if(initial_state[i] == 0)
			continue;

		for(int j = i+1; j < puzzle_size; j++) {
			if(initial_state[j] != 0 && initial_state[i] > initial_state[j])
				inversion_count++;
		}
	}

	return inversion_count;
}

int n_puzzle::get_blank_index(int state[]) {
	for(int i = 0; i < puzzle_size; i++) {
		if(state[i] == 0)
			return i;
	}
}

bool n_puzzle::inVisited(int state[]) {
	int i, j;

	for(i = 0; i < visited.size(); i++) {
		for(j = 0; j < puzzle_size; j++) {
			if(visited[i]->state[j] != state[j])
				break;
		}

		if(j == puzzle_size)
			return true;
	}

	return false;
}