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
    int relativeTime;
    bool operator==(const Observation& rhs) const {
        return (set == rhs.set) && (position == rhs.position) && (value == rhs.value) && (relativeTime == rhs.relativeTime);
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
struct Path{
	vector<int> initialValue;
	vector<int> finalValue;
	vector<int> action;
	int reward;
};

// Finding pattern
/////////////////////////////////

/*

==========================================================================

WHAT I WANT:

ACT 0 : ([0](0) => [0](-1))         for all
ACT 1 : ([0](0) => [0](-1) + 1)     if [0](-1) E [0:2]
ACT 1 : ([0](0) => 0)               if [0](-1) == 3
ACT 2 : ([0](0) => 0)               for all


==========================================================================


OTHER THAN 1
If: [set 0: 0/0], [(-1) set 1: 0/2], End
If: [set 0: 0/0], [(-1) set 1: 0/3], End

Set 1 - [0] : 1 if 1 at [0] during T-1 and [ACT 0]
  -> [[ Set 1 - [0] : x if x at [0]   during T-1 and [ACT 1] ]] ?
  -> [[ Set 1 - [0] : x if 1 at [x-1] during T-1 and [ACT 1] ]] ?
  -> [[ Set 1 - [0] : x if x at [x-1] during T-1 and [ACT 1] ]] ?

Set 1 - [0] : 1 if 0 at [0] during T-1 and [ACT 1]
  -> [[ Set 1 - [0] : x if x-1 at [0]   during T-1 and [ACT 1] ]] ?
  -> [[ Set 1 - [0] : x if 0   at [x-1] during T-1 and [ACT 1] ]] ?
  -> [[ Set 1 - [0] : x if x-1 at [x-1] during T-1 and [ACT 1] ]] ?

==========================================================================

Set 1 - [0] : 2 if 2 at [0] during T-1 and [ACT 0]
  -> [[ Set 1 - [0] : x if x at [0]   during T-1 and [ACT 1] ]] !!!
  -> [[ Set 1 - [0] : x if 2 at [x-2] during T-1 and [ACT 1] ]] ?
  -> [[ Set 1 - [0] : x if x at [x-2] during T-1 and [ACT 1] ]] ?

Set 1 - [0] : 2 if 1 at [0] during T-1 and [ACT 1]
  -> [[ Set 1 - [0] : x if x-1 at [0]   during T-1 and [ACT 1] ]] !!!
  -> [[ Set 1 - [0] : x if 1   at [x-2] during T-1 and [ACT 1] ]] ?
  -> [[ Set 1 - [0] : x if x-1 at [x-2] during T-1 and [ACT 1] ]] ?

==========================================================================

Set 1 - [0] : 3 if 3 at [0] during T-1 and [ACT 0]
  -> [[ Set 1 - [0] : x if x at [0]   during T-1 and [ACT 1] ]] !!!
  -> [[ Set 1 - [0] : x if 3 at [x-3] during T-1 and [ACT 1] ]] ?
  -> [[ Set 1 - [0] : x if x at [x-3] during T-1 and [ACT 1] ]] ?

Set 1 - [0] : 3 if 2 at [0] during T-1 and [ACT 1]
  -> [[ Set 1 - [0] : x if x-1 at [0]   during T-1 and [ACT 1] ]] !!!
  -> [[ Set 1 - [0] : x if 2   at [x-3] during T-1 and [ACT 1] ]] ?
  -> [[ Set 1 - [0] : x if x-1 at [x-3] during T-1 and [ACT 1] ]] ?
  
  
==========================================================================
SIMILAR
==========================================================================

  -> [[ Set 1 - [0] : x if x at [0] during T-1 and [ACT 1] ]]
  -> [[ Set 1 - [0] : x if x at [0] during T-1 and [ACT 1] ]]
  -> [[ Set 1 - [0] : x if x at [0] during T-1 and [ACT 1] ]]
  
  -> [[ Set 1 - [0] : x if x-1 at [0] during T-1 and [ACT 1] ]]
  -> [[ Set 1 - [0] : x if x-1 at [0] during T-1 and [ACT 1] ]]
  -> [[ Set 1 - [0] : x if x-1 at [0] during T-1 and [ACT 1] ]]

==========================================================================
RESULT
==========================================================================

[[ Set 1 - [0] : x if x at [0]   during T-1 and [ACT 1] ]]  FOR x E[1,2,3]
[[ Set 1 - [0] : x if x-1 at [0] during T-1 and [ACT 1] ]]  FOR x E[1,2,3]

*/


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

extern vector<Path> pathList;

extern vector<int> actionList;

extern int timeCount;
extern int action;
extern int setAmount;

extern int commandSize;
extern int resultSize;
extern int rewardSize;

#endif
