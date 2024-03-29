#include "deduction.h"

// GLOBAL VARIABLES
///////////////////

int curSet;
int curLinkPos;
int curLogicID;
int observedValue;
Logic* curLogic;
int curTimestamp;
vector<pair<int,int>> curStudySetList;

int kill=0;

// LOCAL FUNCTIONS
//////////////////

// Sorting the observations by set
bool compareObservationBySet(const Observation& obs1, const Observation& obs2){
	if(obs1.set < obs2.set) return true;
	if(obs1.set == obs2.set) return (obs1.position < obs2.position);
    return false;
}

// Sorting the logic by observation
bool compareConditionByObservation(const Condition& cond1, const Condition& cond2){
	return (cond1.observationList.size() < cond2.observationList.size());
}

// Remove a specific element from the list to copy
vector<Condition> filterCondition(vector<Condition> conditionList, int element){
	vector<Condition> filteredConditionList;
	if(conditionList.size()>1){
		list<Condition> filteredListCheck(conditionList.begin(), next(conditionList.begin(), element));
		filteredListCheck.insert(filteredListCheck.end(), next(conditionList.begin(), element+1), conditionList.end());
		vector<Condition> filteredConditionListTemp(filteredListCheck.begin(), filteredListCheck.end());
		filteredConditionList = filteredConditionListTemp;
	}
	return filteredConditionList;
}

bool isSimpler(Condition cond1, Condition cond2){
	// Is cond1 simpler than cond2 ?
	if(cond1.observationList == cond2.observationList) return true;
	
	for(Observation observation1 : cond1.observationList){
		bool localSimpler(false);
		for(Observation observation2 : cond2.observationList){
			if(observation2 == observation1) localSimpler=true;
		}
		if(!localSimpler) return false;
	}
	return true;
}

bool simplerConditionInList(Condition condition, vector<Condition> condList){
	printObservationList(condition.observationList);
	for(Condition cond : condList){
		cout << "Try:" << endl;
		printObservationList(cond.observationList);
		if(isSimpler(cond,condition)){
			cout << "Discarded" << endl;
			return true;
		}
	}
	cout << "Conserved" << endl;
	return false;
}

vector<pair<int,int>> differenceBetweenStates(vector<int> state1, vector<int> state2){
	vector<pair<int,int>> diffPoints;
	for(int i=0; i<state1.size(); i++){
		if(state1[i]!=state2[i]) diffPoints.push_back(make_pair(i,state2[i]));
	}
	return diffPoints;
}

vector<int> verifyBellowState(Condition condition, int logicPos){

	vector<int> conflictingStates;

	// For each previous link
	for(int i=0; i<logicPos; i++){
	
		cout << endl << "Verify bellow " << i << endl;
		vector<int> timestampList = linkSuperList[curSet][curLinkPos].logicList[i].timestampList;
		
		// For each attached memory
		for(int timestamp : timestampList){
			cout << "Timestamp: " << timestamp << endl;
			
			// Check if lower gives same result
			if(testLogic(timestamp, condition)){
				cout << "Conflicting..." << endl;
				conflictingStates.push_back(timestamp);
			}
			else cout << "Good to go..." << endl;
			
		}
	}
	return conflictingStates;
}

Condition establishCondition(int timestampPrev, int timestampNew){

	Condition condition;
	
	for(pair<int,int> studySet : curStudySetList){
	
		int set = studySet.first;
		int shift = studySet.second;
		
		vector<int> statePrev, stateNew;
		
		if(timestampPrev-shift<0) statePrev.assign(resultSize, -1);
		else statePrev = stateMemSuperList[timestampPrev-shift][set].state;
		
		if(timestampNew-shift<0) stateNew.assign(resultSize, -1);
		else stateNew = stateMemSuperList[timestampNew-shift][set].state;
		
		cout << "Set " << set << " | Prev timestamp " << timestampPrev << ":" << endl << "> ";
		for(int val : statePrev) cout << val << " ";
		cout << endl;
		
		cout << "Set " << set << " | New timestamp " << timestampNew << ":" << endl << "> ";
		for(int val : stateNew) cout << val << " ";
		cout << endl;
		
		auto diffList = differenceBetweenStates(statePrev, stateNew);
		cout << "Differences:" << endl;
		for(pair<int,int> diff : diffList) cout << diff.first << "/" << diff.second << " ";
		cout << endl;
		
		for(pair<int,int> diff : diffList){
			Observation newObservation;
			newObservation.set = set;
			newObservation.position = diff.first;
			newObservation.value = diff.second;
			newObservation.shift = shift;
			condition.observationList.push_back(newObservation);
			cout << "[ADD CONDITION]" << endl;
			printObservationList({newObservation});
		}
		
		cout << "FINISHED" << endl;
		
	}
	
	if(condition.observationList.size()>0) return condition;
	
	// PROBLEM : no difference found
	cout << endl << "No diff found (incorrect inputs)" << endl;
	exit(1);

}

vector<Condition> correctCondition(Condition condition, int timestamp, vector<int> conflictingBellow){

	vector<Condition> correctedConditionList;

	for(int conflicting : conflictingBellow){
	
		Condition correctedCondition = establishCondition(conflicting, timestamp);
		
		for(Observation observation : condition.observationList) correctedCondition.observationList.push_back(observation);
		correctedConditionList.push_back(correctedCondition);
		
	}
	
	return correctedConditionList;
	
}

vector<Condition> checkCondition(Condition condition, int logicPos, int timestamp){

	if(kill++==5000){
		cout << "Loop..." << endl;
		exit(1);
	}

	cout << endl << "CHECKING!" << endl;
	printObservationList(condition.observationList);

	vector<Condition> conditionList = {condition};
	vector<int> conflictingBellow = verifyBellowState(condition, logicPos);
	
	// Condition is fine
	if(conflictingBellow.size()==0) return conditionList;
	
	// Some conflict found
	else{
		// Generate list of corrected for conflict
		cout << endl;
		vector<Condition> correctedConditionList = correctCondition(condition, timestamp, conflictingBellow);
		vector<Condition> retainedConditionList;
		// For each corrected, re-generate list of corrected (until condition is fine)
		for(Condition correctedCondition : correctedConditionList){
			vector<Condition> checkedConditionList = checkCondition(correctedCondition, logicPos, timestamp);
			for(Condition checkedCondition : checkedConditionList) retainedConditionList.push_back(checkedCondition);
		}
		// Return last row of corrected condition
		return retainedConditionList;
	}
}

void processChecked(vector<Condition> checkedConditionList, int logicPos, Logic* logic){
	for(int i=0; i<checkedConditionList.size(); i++){
		Condition checkedCondition = checkedConditionList[i];
		cout << endl << "> Sub Condition " << i << endl;
		printObservationList(checkedCondition.observationList);
		
		//Simplify found by checked
		cout << endl << "[SEARCH SIMPLER CHECKED]" << endl;
		if(simplerConditionInList(checkedCondition, filterCondition(checkedConditionList, i))){
			checkedConditionList.erase(checkedConditionList.begin() + i);
			i--;
			continue;
		}
		
		//Simplify found by known
		cout << endl << "[SEARCH SIMPLER KNOWN]" << endl;
		if(simplerConditionInList(checkedCondition, linkSuperList[curSet][curLinkPos].logicList[logicPos].conditionList)) continue;
	
		//Finally adding the condition to the list
		logic->conditionList.push_back(checkedCondition);
		
		//Simplify known by known
		cout << endl << "[ORGANIZE KNOWN]" << endl;
		for(int j=0; j<logic->conditionList.size(); j++){
			Condition knownCondition = logic->conditionList[j];
			
			vector<Condition> filteredKnownCondList = filterCondition(logic->conditionList, j);
			
			if(simplerConditionInList(knownCondition, filteredKnownCondList)){
				logic->conditionList.erase(logic->conditionList.begin()+j);
				j--;
			}
		}
		cout << endl;
	}
}

pair<int,int> checkAbove(){
	
	cout << endl << "[WITH STATE]" << endl;
		
	// For each above link
	for(int i=curLogicID+1; i<linkSuperList[curSet][curLinkPos].logicList.size(); i++){
		
		cout << endl << "Verify Above " << i << endl;
		
		vector<Condition> conditionList = linkSuperList[curSet][curLinkPos].logicList[i].conditionList;
		
		for(int j=0; j<conditionList.size(); j++){
			Condition condition = conditionList[j];
		
			// Check if above has valid cond
			if(testLogic(curTimestamp, condition)){
				cout << "> Conflicting..." << endl;
				return make_pair(i,j);
			}
			else cout << "> Good to go..." << endl;
		}
	}
	return(make_pair(-1,-1));
}

vector<int> findTimestamp(int logicID, int conditionID){

	vector<int> foundTimestampList;
	vector<int> timestampList = linkSuperList[curSet][curLinkPos].logicList[logicID].timestampList;
	
	// For each attached memory
	for(int timestamp : timestampList){
		cout << "Timestamp: " << timestamp << endl;
		
		bool verified(false);
		for(int i=0; i<linkSuperList[curSet][curLinkPos].logicList[logicID].conditionList.size(); i++){
			if(i==conditionID) continue;
			if(testLogic(timestamp,linkSuperList[curSet][curLinkPos].logicList[logicID].conditionList[i])){
				verified=true;
				break;
			}
		}
		if(!verified) foundTimestampList.push_back(timestamp);
		
	}
	
	return foundTimestampList;
	
}

void correctAbove(){

	cout << endl << "[CORRECT ABOVE]" << endl;
	
	while(1){
		pair<int,int> conflictAbove = checkAbove();
		if(conflictAbove==make_pair(-1,-1)) break;
		
		cout << "Conflict with: ";
		printObservationList(linkSuperList[curSet][curLinkPos].logicList[conflictAbove.first].conditionList[conflictAbove.second].observationList);
		
		auto& conditionVector = linkSuperList[curSet][curLinkPos].logicList[conflictAbove.first].conditionList;
		Condition condition = conditionVector[conflictAbove.second];
			
		vector<int> timestampList = findTimestamp(conflictAbove.first, conflictAbove.second);
		conditionVector.erase(conditionVector.begin() + conflictAbove.second);
		
		cout << "Found problematic timestamp:" << endl;
		for(int timestamp : timestampList){
			cout << timestamp << endl;
		}
		
		for(int timestamp : timestampList){
			cout << endl << "[CHECK CONDITION]" << endl;
			vector<Condition> checkedConditionList = checkCondition(condition, conflictAbove.first, timestamp);
			processChecked(checkedConditionList, conflictAbove.first, &linkSuperList[curSet][curLinkPos].logicList[conflictAbove.first]);
		}
	}
}

vector<Condition> generateConditionFromState(){

	cout << endl << "[SEARCH DIFF]" << endl;
	
	vector<Condition> conditionList;
	
	// For each previous link
	for(int i=curLogicID-1; i>=0; i--){
	
		cout << endl << "Check link " << i << endl;
		
		vector<int> timestampList = linkSuperList[curSet][curLinkPos].logicList[i].timestampList;
		
		// For each attached memory
		for(int j=timestampList.size()-1; j>=0; j--){
		
			/*int foundResult = output(curSet, curLinkPos, curTimestamp);
			cout << "Computed value: " << foundResult << endl;
			if(foundResult==observedValue) break;*/
			
			
			Condition newCondition = establishCondition(timestampList[j], curTimestamp);
				
			if(newCondition.observationList.size()==0) cout << "CAN'T OBSERVE ANY CHANGE" << endl;
			else conditionList.push_back(newCondition);
		}
	}
	
	return conditionList;
}

void solveLogic(){

	cout << endl << "[SOLVE LOGIC]" << endl;
	
	//List differences with previous logic
	vector<Condition> newConditionList = generateConditionFromState();
	
	for(int i=0; i<newConditionList.size(); i++){
	
		Condition conditionFound = newConditionList[i];
		cout << endl << "Condition " << i << endl;
		printObservationList(conditionFound.observationList);
	
		//Simplify found by found
		cout << endl << "[SEARCH SIMPLER FOUND]" << endl;
		if(simplerConditionInList(conditionFound, filterCondition(newConditionList, i))){
			newConditionList.erase(newConditionList.begin() + i);
			i--;
			continue;
		}
		
		//Simplify found by known
		cout << endl << "[SEARCH SIMPLER KNOWN]" << endl;
		if(simplerConditionInList(conditionFound, linkSuperList[curSet][curLinkPos].logicList[curLogicID].conditionList)) continue;
	
		//Update the condition to match previous
		cout << endl << "[CHECK CONDITION]" << endl;
		vector<Condition> checkedConditionList = checkCondition(conditionFound, curLogicID, curTimestamp);
		kill=0;
		
		//Process the checked condition
		processChecked(checkedConditionList, curLogicID, curLogic);
		
	}
}


// MAIN FUNCTIONS
//////////////////

void associateLink(int set, int linkPos, vector<pair<int,int>> studySetList, bool mute){

	ofstream null_stream("/dev/null");
	streambuf* old_cout = cout.rdbuf();
	
	if(mute) cout.rdbuf(null_stream.rdbuf());
	
	cout << "<LINK SET: " << set << " | POS: " << linkPos << ">" << endl;

	curSet = set;
	curLinkPos = linkPos;
	curTimestamp = stateMemSuperList.size()-1;
	curStudySetList = studySetList;
	
	observedValue = stateMemSuperList.back()[curSet].state[curLinkPos];
	
	int logicFound(-1);
	for(int j=0; j<linkSuperList[curSet][curLinkPos].logicList.size(); j++){
		if(linkSuperList[curSet][curLinkPos].logicList[j].outcome==observedValue){
			logicFound = j;
			break;
		}
	}

	if(logicFound==-1){
		cout << "Logic " << observedValue << " unknown, creating new..." << endl;
		Logic newLogic;
		newLogic.timestampList.push_back(timeCount);
		newLogic.outcome = observedValue;
		linkSuperList[curSet][curLinkPos].logicList.push_back(newLogic);
		
		curLogicID = linkSuperList[curSet][curLinkPos].logicList.size()-1;
		curLogic = &linkSuperList[curSet][curLinkPos].logicList[curLogicID];
		cout << "Cur logic ID: " << curLogicID << endl;
		solveLogic();
	
	}
	else{
		cout << "Logic " << observedValue << " known, adding timestamp..." << endl;
		linkSuperList[curSet][curLinkPos].logicList[logicFound].timestampList.push_back(timeCount);
		
		int foundResult = output(curSet, curLinkPos, curTimestamp);
		if(foundResult == observedValue){
			cout << "Functionnal logic, no process" << endl;
			cout.rdbuf(old_cout);
			return;
		}
		cout << "Logic doesn't match current definition, searching differences" << endl;
		
		curLogicID = logicFound;
		curLogic = &linkSuperList[curSet][curLinkPos].logicList[logicFound];
		cout << "Cur logic ID: " << curLogicID << endl;
		solveLogic();
		correctAbove();
	}
	
	// Sort every logic to get a proper view
	for(vector<Link> &linkList : linkSuperList){
		for(Link &link : linkList){
			for(Logic &logic : link.logicList){
				for(Condition &condition : logic.conditionList){
					sort(condition.observationList.begin(), condition.observationList.end(), compareObservationBySet);
				}
				sort(logic.conditionList.begin(), logic.conditionList.end(), compareConditionByObservation);
			}
		}
	}
	
	cout.rdbuf(old_cout);
	
}
