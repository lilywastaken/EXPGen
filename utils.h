#ifndef MEM_UTILS_H
#define MEM_UTILS_H

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <time.h>
#include <stdio.h>
#include <set>

#include "Cube.h"
#include "Beginner.h"

using namespace std;

//STRUCTURES
////////////////////////

// Original values
struct Set{
	string name;
	vector<int> valueList;
};

// Pattern common value
struct PCV{
	vector<pair<Set*,int>> valueList;
	bool reversed = false;
};

// Pattern equal value
struct PEV{
	vector<pair<Set*,int>> valueList;
	vector<int> equalValueList;
	bool reversed = false;
};

// Sub-Condition
struct SC{
	vector<PCV> pcvList;
	vector<PEV> pevList;
	int checkState = 0; // Undefined - false - true
};

// Top-Condition
struct TC{
	string name;
	vector<pair<vector<TC>,vector<SC>>> conditionList; // Respect one of them
	int checkState = 0; // Undefined - false - true
};


//FUNCTIONS
////////////////////////

void loadSet(vector<Set*>& setList, Cube &c);
void directAction(vector<int> actionList, Cube &c);
void printPCV(PCV pcv);
void printPEV(PEV pev);
void printSC(SC sc);
void printTC(TC tcOriginal);
bool checkSC(SC scOriginal);
bool checkTC(TC tcOriginal);
vector<PCV> stateComposition(vector<Set*>& setList, Cube &c);
vector<PCV> correctComposition(vector<PCV> pCVL1, vector<PCV> pCVL2);
vector<pair<Set*,int>> getSimilarList(vector<Set> setListPrev, vector<Set*> &setListNew);
bool messWithCube(int remainingMove, vector<vector<int>> &actionSeriesList, vector<int> currentActionSeries, vector<Set*>& setList, Cube c, SC scCondition);

void inputSolvedCube(Cube &c);
void cubeManip(Cube &c);
void generateTestCube(Cube &c, int iteration);

streambuf* mute();
void activate(streambuf* old_cout);

#endif


















