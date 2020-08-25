// Daniel Tse
// rec03.cpp
// OOP Lab 3
// 2/9/18
// This is the game "Minesweeper"

#include <vector>
#include <iostream>	// cin, cout, cerr, endl
#include <fstream>	// ifstream
#include <string>	// string
#include <cstdlib>  
#include <ctime>

using namespace std;
const int COLSIZE = 10;
const int ROWSIZE = 10;
const int PERCENTBOMB = 10;

class Minesweeper {
public:
	Minesweeper() {
		srand(time(NULL));
		//int bombTile = -1;
		int emptyTile = 0;
		//create bombs
		for (int row = 0; row < ROWSIZE + 2; row++) {
			vector <int> emptyVec;
			bombCount.push_back(emptyVec);
			for (int col = 0; col < COLSIZE + 2; col++) {
				if (row > 0 && row < ROWSIZE + 1 && col > 0 && col < COLSIZE + 1) {
					int bomb = rand() % PERCENTBOMB + 1;
					if (bomb == 1) {
						numBombs++;
						bombCount[row].push_back(bombTile);
					}
					else {
						bombCount[row].push_back(emptyTile);
					}
				}
				else {
					bombCount[row].push_back(emptyTile);
				}
			}
		}
		//fill other info
		for (int row = 1; row < ROWSIZE + 1; row++) {
			for (int col = 1; col < COLSIZE + 1; col++) {
				if (bombCount[row][col] != bombTile) {
					if (bombCount[row - 1][col] == bombTile)bombCount[row][col]++;//top
					if (bombCount[row - 1][col + 1] == bombTile)bombCount[row][col]++;//top right
					if (bombCount[row][col + 1] == bombTile)bombCount[row][col]++;//right
					if (bombCount[row + 1][col + 1] == bombTile)bombCount[row][col]++;//bottom right
					if (bombCount[row + 1][col] == bombTile)bombCount[row][col]++;//bottom
					if (bombCount[row + 1][col - 1] == bombTile)bombCount[row][col]++;//bottom left
					if (bombCount[row][col - 1] == bombTile)bombCount[row][col]++;//left
					if (bombCount[row - 1][col - 1] == bombTile)bombCount[row][col]++;//top left
				}
			}
		}
		cout << endl;
		//hide all info in visibleCells at game start
		for (int row = 0; row < ROWSIZE + 2; row++) {
			vector <bool> emptyVec;
			visibleCells.push_back(emptyVec);
			for (int col = 0; col < COLSIZE + 2; col++) {
				visibleCells[row].push_back(false);
			}
		}
	}
	//displays board. If game is over displays bombs as well
	void display(bool gameOver) {
		if (!gameOver) {
			for (int row = 1; row < ROWSIZE + 1; row++) {
				for (int col = 1; col < COLSIZE + 1; col++) {
					if (visibleCells[row][col]) {
						cout << bombCount[row][col];
					}
					else {
						cout << '*';
					}
				}
				cout << endl;
			}
		}
		else {
			for (int row = 1; row < ROWSIZE + 1; row++) {
				for (int col = 1; col < COLSIZE + 1; col++) {
					if (bombCount[row][col] == bombTile) {
						cout << "X";
					}
					else {
						cout << bombCount[row][col];
					}
				}
				cout << endl;
			}
		}
	}
	//returns true if game is over
	bool done() {
		int nonBombs = ROWSIZE * COLSIZE - numBombs;
		for (int row = 1; row < ROWSIZE + 1; row++) {
			for (int col = 1; col < COLSIZE + 1; col++) {
				if (bombCount[row][col] == bombTile) {
					//returns true if bomb is visible
					if (visibleCells[row][col]){
						return true;
					}
				}
				else {
					//count number of visible nonBomb tiles
					if (visibleCells[row][col]) {
						nonBombs--;
						if (nonBombs == 0) {
							return true;
						}
					}
				}
			}
		}
		return false;
	}
	//takes a col number and returns true if the col number is valid and false otherwise
	bool validcol(int col) {
		return (col <= COLSIZE && col > 0);
	}
	//takes a rowumn number and returns true if the rowumn number is valid and false otherwise
	bool validrow(int row) {
		return (row <= ROWSIZE && row > 0);
	}
	//takes a cell col and rowumn and returns true if it is visible and false otherwise
	bool isVisible(int col, int row) {
		return visibleCells[row][col];
	}
	//takes a cell col and rowumn and changes the state of the board according to move
	//returns false if move results in an explosion and true otherwise
	bool play(int col, int row) {
		//bomb selected
		if (bombCount[row][col] == -1) {
			visibleCells[row][col] = true;
			return false;
		}
		//tile adjacent to bomb selected
		else if (!(bombCount[row][col] == 0)) {
			visibleCells[row][col] = true;
		}		
		//empty tile selected
		else { //(bombCount[row][col] == 0)
			vector<vector<int>> loc;
			vector<int> coords = { row, col };
			loc.push_back(coords);
			while (loc.size() > 0) {
				vector<int> curr = loc.back();
				loc.pop_back();
				if (!visibleCells[curr[0]][curr[1]]){
					visibleCells[curr[0]][curr[1]] = true;
					//if tile is valid and not adjacent to bombs then reveal all 
					//adjacent tiles
					if (bombCount[curr[0]][curr[1]] == 0 &&
						validcol(curr[0]) && validrow(curr[1])) {
						for (int i = -1; i < 2; i++){
							for (int j = -1; j < 2; j++){
								if (!(visibleCells[curr[0] + i][curr[1] + j])){								
									vector<int> coordin = { curr[0] + i , curr[1] + j };
									loc.push_back(coordin);
								}
							}
						}
					}
				}
			}
		}
		return true;
	}
private:
	vector<vector<int>> bombCount;
	vector<vector<bool>> visibleCells;
	int bombTile = -1;
	int numBombs = 0;
};
int main() {
	Minesweeper sweeper;
	// Continue until only invisible cells are bombs
	while (!sweeper.done()) {
		sweeper.display(false); // display board without bombs

		int col_sel = -1, row_sel = -1;
		while (col_sel == -1) {
			// Get a valid move
			int r, c;
			cout << "col? ";
			cin >> r;
			if (!sweeper.validcol(r)) {
				cout << "col out of bounds\n";
				continue;
			}
			cout << "row? ";
			cin >> c;
			if (!sweeper.validcol(c)) {
				cout << "rowumn out of bounds\n";
				continue;
			}
			if (sweeper.isVisible(r, c)) {
				cout << "Square already visible\n";
				continue;
			}
			col_sel = r;
			row_sel = c;
		}
		// Set selected square to be visible. May effect other cells.
		if (!sweeper.play(col_sel, row_sel)) {
			cout << "Sorry, you died..\n";
			sweeper.display(true); // Final board with bombs shown
			exit(0);
		}
	}
	// Ah! All invisible cells are bombs, so you won!
	cout << "You won!!!!\n";
	sweeper.display(true); // Final board with bombs shown
}