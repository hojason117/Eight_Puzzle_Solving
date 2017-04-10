#ifndef _N_PUZZLE_H_
#define _N_PUZZLE_H_

#include <vector>
#include <string>

class n_puzzle {
private:
	enum Action{UP, DOWN, LEFT, RIGHT};

public :
	struct dataNode {
		int *state;
		int total_cost, estimated_cost;
		Action action;
		dataNode *parent;
	};

private:	
	int puzzle_size;
	int edgeLength;
	int *initial_state;
	dataNode *goal;
	std::vector<dataNode*> visited;

	int heuristic(int state[]);
	int findSuccessor(int state[], int **nextState, Action nextAction[]);
	bool isGoalState(int state[]);
	int permutation_inversion();
	int get_blank_index(int state[]);
	bool inVisited(int state[]);

public:	
	n_puzzle(int size);
	~n_puzzle();
	void set_initial_State(std::string initial_state);
	void outputPath();
	bool isSolvable();
	void find_shortest_path();
};

#endif