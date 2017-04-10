#include <iostream>
#include "MyMinHeap.h"
#include "n_puzzle.h"
#include <string>
#include <ctime>

using namespace std;

void main() {
	int puzzle_size;
	string initial_state, temp;
	time_t begin, end;

	cout << "Please insert size of puzzle(ex: 9 or 16...): " << flush;
	getline(cin, temp);
	puzzle_size = stoi(temp);

	n_puzzle *puzzle = new n_puzzle(puzzle_size);

	cout << "Please insert initial state(ex: 0 2 1 5 6 4 3 7 8): " << flush;
	getline(cin, initial_state);
	//initial_state = "0 1 2 15 14 13 12 11 10 9 7 8 5 6 4 3";
	//initial_state = "0 3 1 2 4 5 6 7 8 10 11 9 12 13 14 15";  //32 secs

	puzzle->set_initial_State(initial_state);

	time(&begin);

	if(puzzle->isSolvable())
		puzzle->find_shortest_path();

	time(&end);
	
	puzzle->outputPath();

	cout << "time spent: " << difftime(end, begin) << " seconds" << endl;

	delete puzzle;

	system("pause");
}