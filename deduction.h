#ifndef MEM_DEDUCTION_H
#define MEM_DEDUCTION_H

#include "utils.h"
#include "line.h"

using namespace std;

void reviewCondition();
vector<bool> checkLogic();

void associateLink(int set, int linkPos, vector<pair<int,int>> studySetList, bool mute);

#endif
