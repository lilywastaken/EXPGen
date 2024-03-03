#ifndef MEM_UTILS_H
#define MEM_UTILS_H

#include "line.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <chrono>
#include <thread>
#include <cstring>
#include <algorithm>
#include <random>
#include <utility>
#include <set>

using namespace std;

//STRUCTURES
////////////////////////

// Understanding the values logic
/////////////////////////////////
struct Observation{
    int set;
    int position;
    int value;
    int shift;
    bool operator==(const Observation& rhs) const {
        return (set == rhs.set) && (position == rhs.position) && (value == rhs.value) && (shift == rhs.shift);
    }
};

struct Condition{
	vector<Observation> observationList;
};

struct Logic{
	vector<int> timestampList;
	vector<Condition> conditionList;
	int outcome;
};

struct Link{
	vector<Logic> logicList;
};

// Registering data in memory
/////////////////////////////////
struct StateMem{
	vector<int> state;
};

struct Associate{
	int timestamp;
	int set;
	int link;
	int reward;
};

struct Explain{
	int timestamp;
	int set;
	int link;
	int timeOfLink;
	int reward;
};

struct ActionMem{
	vector<Associate> associateList;
	vector<Explain> explainList;
};

// Acquiring data
/////////////////////////////////
struct ALResult{
	bool logicFound;
	bool linkWorks;
	vector<int> conflictingStates;
};

// Checking path
/////////////////////////////////

struct Transition{
	vector<int> initialValue;
	int action;
};

struct Navigation{
	vector<int> state;
	vector<int> actionList;
};

struct StepPath{
	Transition transition;
	int finalValuePos;
};

struct PotentialPath{
	vector<vector<int>> initialValueList;
	int action;
	PotentialPath(int resultSize) : initialValueList(resultSize, vector<int>()) {}
};

struct Path{
	Transition transition;
	vector<int> finalValue;
	int reward;
};

struct Map{
	vector<int> state;
	vector<Navigation> inputNavigationList;
	vector<Navigation> outputNavigationList;
	bool examined = false;
};


//FUNCTIONS
////////////////////////

Link initLinkResult1();
Link initLinkResult2();
Link initLinkReward();

void printObservationList(vector<Observation> observationList);
void printSumUp();

bool testLogic(int timestamp, Condition condition);
int output(int set, int pos, int timestamp);

void associateStateMem();

//VARIABLES
////////////////////////

extern Line line;
extern vector<vector<StateMem>> stateMemSuperList;
extern vector<vector<Link>> linkSuperList;

extern vector<Map> mapList;

extern vector<int> actionList;
extern vector<int> valueList;

extern int timeCount;
extern int action;
extern int setAmount;

extern int commandSize;
extern int resultSize;
extern int rewardSize;

#endif
