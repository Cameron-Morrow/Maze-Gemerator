// mazeGenerator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <ctime>
 
using namespace std;

struct mazeBlock {
	int blockType;
	int xPOS;
	int yPOS;
	bool topWall;
	bool bottomWall;
	bool leftWall;
	bool rightWall;
	mazeBlock *next = NULL;
};
int MazeSize = 3; //minimum is 2X2 max is 20X20
int MazeGenerator(int, int);
bool blockCreator(mazeBlock *current, mazeBlock *root, mazeBlock *prev, int x, int y, int goalx, int goaly);
bool Check_Sibling_Gate(mazeBlock *block, int x, int y, int neighbor);
bool printMaze(mazeBlock *block, int, int);
bool Assign_Block_Type(mazeBlock *block);
bool buildPseudoMaze(mazeBlock *block, int, int, char*, int);

int main() {
	srand(time(NULL));
	int something;
	MazeGenerator(MazeSize, MazeSize);
	cout << endl;
	cin >> something;
	return 1;
}

int MazeGenerator(int x, int y) {
	int input = 1;
	while(input){
		//starting point is always going to be 0,0 with either a LB block(1) an LRB block(2) or a LR block(3)
		int startBlock = (rand() % 3) + 1;
		int goalx = 0;
		int goaly = 0;
		int test = rand() % 2;
		if (test) {
			goalx = rand() % x;
			goaly = y - 1;
		}
		else {
			goaly = rand() % y;
			goalx = x - 1;
		}
		mazeBlock *root;
		root = new mazeBlock;
		root->xPOS = 0;
		root->yPOS = 0;
		if (startBlock == 1) {
			root->bottomWall = false;
			root->leftWall = false;
			root->rightWall = true;
			root->topWall = true;
			root->blockType = 6;
		}
		else if (startBlock == 2) {
			root->bottomWall = false;
			root->leftWall = false;
			root->rightWall = false;
			root->topWall = true;
			root->blockType = 11;
		}
		else {
			root->bottomWall = true;
			root->leftWall = false;
			root->rightWall = false;
			root->topWall = true;
			root->blockType = 2;
		}
		blockCreator(root->next, root, root, x, y, goalx, goaly);
		printMaze(root, x, y);
		int num = (x * 3) * (x * 3);
		char arr[10000];
		for (int i = 0; i < num; ++i) {
			arr[i] = 219;
		}
		cout << endl << "  ";
		buildPseudoMaze(root, x, y, arr, 1);
		for (int i = 0; i < num; i++) {
			if (i % (x * 3) == 0) {
				cout << endl << "  ";
			}
			cout << arr[i];
			cout << arr[i];
		}
		for (int i = 0; i < num; i++) {
			if (i % (x * 3) == 0) {
				cout << endl;
			}
			if (i % ((x * 3) * 3) == 0) {
				for (int j = 0; j < x; j++) {
					cout << char(196) << char(196) << char(196) << char(196) << char(196) << char(196) << char(196);
				}
				cout << endl;
			}
			if (i % 3 == 0) {
				cout << char(179);
			}
			cout << arr[i];
			cout << arr[i];
		}
		cin >> input;
	}
	return 0;
}
bool blockCreator(mazeBlock *current, mazeBlock *root, mazeBlock *prev, int x, int y, int goalx, int goaly) {
	current = new mazeBlock;
	int percentage = 70;
	int perTotal = 100;
	prev->next = current;
	if (prev->xPOS < x - 1) {//iterate to next block position
		current->xPOS = prev->xPOS + 1;
		current->yPOS = prev->yPOS;
	}
	else {
		current->xPOS = 0;
		current->yPOS = prev->yPOS + 1;
	}
	if (current->xPOS == 0 || current->xPOS == x - 1 || current->yPOS == 0 || current->yPOS == y - 1) {//edge case
		if (current->yPOS == 0 && current->xPOS == x - 1) { // dont check top sibling && cant go top or right unless goal .... corner case
			current->topWall = true;
			current->leftWall = Check_Sibling_Gate(root, current->xPOS - 1, current->yPOS, 0);
			//randomly generate bottom gate
			current->bottomWall = (rand() % perTotal) > percentage;
			//check if goal and if goal right gate open else closed
			current->rightWall = !(current->xPOS == goalx && current->yPOS == goaly);
		}
		else if (current->yPOS == y - 1 && current->xPOS == 0) { // dont check left sibling && cant go bottom or left .... corner case
			current->topWall = Check_Sibling_Gate(root, current->xPOS, current->yPOS - 1, 1);
			current->leftWall = true;
			current->bottomWall = !(current->xPOS == goalx && current->yPOS == goaly);
			current->rightWall = (rand() % perTotal) > percentage;
		}
		else if (current->yPOS == y - 1 && current->xPOS == x - 1) { // cant go bottom or right unless goal.... corner case
			current->topWall = Check_Sibling_Gate(root, current->xPOS, (current->yPOS - 1), 1);
			current->leftWall = Check_Sibling_Gate(root, (current->xPOS - 1), current->yPOS, 0);
			if (current->xPOS == goalx && current->yPOS == goaly) {
				if (rand() % 2) {
					current->bottomWall = true;
					current->rightWall = false;
				}
				else {
					current->bottomWall = false;
					current->rightWall = true;
				}
			}
			else {
				current->bottomWall = true;
				current->rightWall = true;
			}
		}
		else if (current->yPOS == 0) { // dont check top sibling && cant go top
			current->topWall = true;
			current->leftWall = Check_Sibling_Gate(root, (current->xPOS - 1), current->yPOS, 0);
			current->bottomWall = (rand() % perTotal) > percentage;
			current->rightWall = (rand() % perTotal) > percentage;
		}
		else if (current->xPOS == 0) {// dont check left sibling && cant go left
			current->topWall = Check_Sibling_Gate(root, current->xPOS, (current->yPOS - 1), 1);
			current->leftWall = true;
			current->bottomWall = (rand() % perTotal) > percentage;
			current->rightWall = (rand() % perTotal) > percentage;
		}
		else if (current->yPOS == y - 1) {//cant go bottom unless goal
			current->topWall = Check_Sibling_Gate(root, current->xPOS, (current->yPOS - 1), 1);
			current->leftWall = Check_Sibling_Gate(root, (current->xPOS - 1), current->yPOS, 0);
			current->bottomWall = !(current->xPOS == goalx && current->yPOS == goaly);
			current->rightWall = (rand() % perTotal) > percentage;
		}
		else if (current->xPOS == x - 1) {//cant go right unless goal
			current->topWall = Check_Sibling_Gate(root, current->xPOS, (current->yPOS) - 1, 1);
			current->leftWall = Check_Sibling_Gate(root, (current->xPOS - 1), current->yPOS, 0);
			current->bottomWall = (rand() % perTotal) > percentage;
			current->rightWall = !(current->xPOS == goalx && current->yPOS == goaly);
		}
	}
	else {
		current->topWall = Check_Sibling_Gate(root, current->xPOS, (current->yPOS - 1), 1);
		current->leftWall = Check_Sibling_Gate(root, (current->xPOS - 1), current->yPOS, 0);
		current->bottomWall = (rand() % perTotal) > percentage;
		current->rightWall = (rand() % perTotal) > percentage;
	}

	Assign_Block_Type(current);

	//recursive part
	if (current->xPOS == x - 1 && current->yPOS == y - 1) {
		return true;
	}
	else {
		return blockCreator(current->next, root, current, x, y, goalx, goaly);
	}
	return 0;
}
bool Check_Sibling_Gate(mazeBlock *block, int a, int b, int neighbor) {
	if (block->xPOS == a && block->yPOS == b) {
		if (neighbor) {
			return block->bottomWall;
		}
		if (!neighbor) {
			return block->rightWall;
		}	
	} else {
		if (block->next != NULL) {
			return(Check_Sibling_Gate(block->next, a, b, neighbor));
		}
	}
	return 0;
}
bool Assign_Block_Type(mazeBlock *block) {
	if (!block->topWall && !block->leftWall && !block->rightWall && !block->bottomWall) {
		block->blockType = 0;
	}
	else if (!block->topWall && !block->leftWall && !block->rightWall && block->bottomWall) {
		block->blockType = 9;
	}
	else if (!block->topWall && !block->leftWall && block->rightWall && !block->bottomWall) {
		block->blockType = 10;
	}
	else if (!block->topWall && block->leftWall && !block->rightWall && !block->bottomWall) {
		block->blockType = 8;
	}
	else if (block->topWall && !block->leftWall && !block->rightWall && !block->bottomWall) {
		block->blockType = 11;
	}
	else if (!block->topWall && !block->leftWall && block->rightWall && block->bottomWall) {
		block->blockType = 5;
	}
	else if (!block->topWall && block->leftWall && !block->rightWall && block->bottomWall) {
		block->blockType = 4;
	}
	else if (block->topWall && !block->leftWall && !block->rightWall && block->bottomWall) {
		block->blockType = 2;
	}
	else if (!block->topWall && block->leftWall && block->rightWall && !block->bottomWall) {
		block->blockType = 1;
	}
	else if (block->topWall && !block->leftWall && block->rightWall && !block->bottomWall) {
		block->blockType = 6;
	}
	else if (block->topWall && block->leftWall && !block->rightWall && !block->bottomWall) {
		block->blockType = 7;
	}
	else if (!block->topWall && block->leftWall && block->rightWall && block->bottomWall) {
		block->blockType = 12;
	}
	else if (block->topWall && !block->leftWall && block->rightWall && block->bottomWall) {
		block->blockType = 15;
	}
	else if (block->topWall && block->leftWall && block->rightWall && !block->bottomWall) {
		block->blockType = 14;
	}
	else if (block->topWall && block->leftWall && !block->rightWall && block->bottomWall) {
		block->blockType = 13;
	}
	else {
		block->blockType = 3;
	}
	return true;
}
bool printMaze(mazeBlock *block, int x, int y) {
	if (block->xPOS == 0) {
		cout << endl;
	}
	cout << block->blockType << " ";
	if (block->xPOS == x && block->yPOS == y) {
		return 1;
	}
	else {
		if (block->next != NULL) {
			printMaze(block->next, x, y);
		}
	}
	return 0;
}
bool buildPseudoMaze(mazeBlock *block, int x, int y, char *arr, int counter) {
	if (counter == x * 1 + 1 && x >= 2) {
		counter += x * 2;
	}
	if (counter == x * 4 + 1 && x >= 3) {
		counter += x * 2;
	}
	if (counter == x * 7 + 1 && x >= 4) {
		counter += x * 2;
	}
	if (counter == x * 10 + 1 && x >= 5) {
		counter += x * 2;
	}
	if (counter == x * 13 + 1 && x >= 6) {
		counter += x * 2;
	}
	if (counter == x * 16 + 1 && x >= 7) {
		counter += x * 2;
	}
	if (counter == x * 19 + 1 && x >= 8) {
		counter += x * 2;
	}
	if (counter == x * 22 + 1 && x >= 9) {
		counter += x * 2;
	}
	if (counter == x * 25 + 1 && x >= 10) {
		counter += x * 2;
	}
	if (counter == x * 28 + 1 && x >= 11) {
		counter += x * 2;
	}
	if (counter == x * 31 + 1 && x >= 12) {
		counter += x * 2;
	}
	if (counter == x * 34 + 1 && x >= 13) {
		counter += x * 2;
	}
	if (counter == x * 37 + 1 && x >= 14) {
		counter += x * 2;
	}
	if (counter == x * 40 + 1 && x >= 15) {
		counter += x * 2;
	}
	if (counter == x * 43 + 1 && x >= 16) {
		counter += x * 2;
	}
	if (counter == x * 46 + 1 && x >= 17) {
		counter += x * 2;
	}
	if (counter == x * 49 + 1 && x >= 18) {
		counter += x * 2;
	}
	if (counter == x * 52 + 1 && x >= 19) {
		counter += x * 2;
	}
	if (counter == x * 55 + 1 && x >= 20) {
		counter += x * 2;
	}
	if (!block->topWall) {
		arr[((counter * 3) - 2)] = 32;
	}
	if (!block->leftWall) {
		arr[(((counter+x) * 3) - 3)] = 32;
	}
	if (true) {
		arr[(((counter + x) * 3) - 2)] = 32;
	}
	if (!block->rightWall) {
		arr[(((counter + x) * 3) - 1)] = 32;
	}
	if (!block->bottomWall) {
		arr[(((counter + (2 * x)) * 3) - 2)] = 32;
	}
	if (block->next != NULL) {
		buildPseudoMaze(block->next, x, y, arr, (counter + 1));
	}
	else {
		return 0;
	}
}
