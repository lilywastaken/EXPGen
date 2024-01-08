#include "utils.h"

// PRINT STUFF
//////////////////

Link initLinkResult1(){
	Observation observation1_1_1 = {0,0,1,0};
	Observation observation1_1_2 = {1,0,0,1};
	Observation observation1_2_1 = {0,0,0,0};
	Observation observation1_2_2 = {1,0,1,1};
	
	Observation observation2_1_1 = {0,0,1,0};
	Observation observation2_1_2 = {1,0,1,1};
	Observation observation2_2_1 = {0,0,0,0};
	Observation observation2_2_2 = {1,0,2,1};
	
	Observation observation3_1_1 = {0,0,1,0};
	Observation observation3_1_2 = {1,0,2,1};
	Observation observation3_2_1 = {0,0,0,0};
	Observation observation3_2_2 = {1,0,3,1};
	
	Condition condition1_1 = {{observation1_1_1,observation1_1_2}};
	Condition condition1_2 = {{observation1_2_1,observation1_2_2}};
	
	Condition condition2_1 = {{observation2_1_1,observation2_1_2}};
	Condition condition2_2 = {{observation2_2_1,observation2_2_2}};
	
	Condition condition3_1 = {{observation3_1_1,observation3_1_2}};
	Condition condition3_2 = {{observation3_2_1,observation3_2_2}};
	
	Logic logic0 = {{}, {}, 0};
	Logic logic1 = {{}, {condition1_1,condition1_2}, 1};
	Logic logic2 = {{}, {condition2_1,condition2_2}, 2};
	Logic logic3 = {{}, {condition3_1,condition3_2}, 3};
	
	return {{logic0,logic1,logic2,logic3}};
}

Link initLinkResult2(){
	Observation observation1_1_1 = {0,0,2,0};
	Observation observation1_1_2 = {1,0,1,1};
	Observation observation1_2_1 = {0,0,1,0};
	Observation observation1_2_2 = {1,1,1,1};
	Observation observation1_3_1 = {0,0,0,0};
	Observation observation1_3_2 = {1,1,1,1};
	
	Observation observation2_1_1 = {0,0,2,0};
	Observation observation2_1_2 = {1,0,2,1};
	Observation observation2_2_1 = {0,0,1,0};
	Observation observation2_2_2 = {1,1,2,1};
	Observation observation2_3_1 = {0,0,0,0};
	Observation observation2_3_2 = {1,1,2,1};
	
	Observation observation3_1_1 = {0,0,2,0};
	Observation observation3_1_2 = {1,0,3,1};
	Observation observation3_2_1 = {0,0,1,0};
	Observation observation3_2_2 = {1,1,3,1};
	Observation observation3_3_1 = {0,0,0,0};
	Observation observation3_3_2 = {1,1,3,1};
	
	
	
	Condition condition1_1 = {{observation1_1_1,observation1_1_2}};
	Condition condition1_2 = {{observation1_2_1,observation1_2_2}};
	Condition condition1_3 = {{observation1_3_1,observation1_3_2}};
	
	Condition condition2_1 = {{observation2_1_1,observation2_1_2}};
	Condition condition2_2 = {{observation2_2_1,observation2_2_2}};
	Condition condition2_3 = {{observation2_3_1,observation2_3_2}};
	
	Condition condition3_1 = {{observation3_1_1,observation3_1_2}};
	Condition condition3_2 = {{observation3_2_1,observation3_2_2}};
	Condition condition3_3 = {{observation3_3_1,observation3_3_2}};
	
	Logic logic0 = {{}, {}, 0};
	Logic logic1 = {{}, {condition1_1,condition1_2,condition1_3}, 1};
	Logic logic2 = {{}, {condition2_1,condition2_2,condition2_3}, 2};
	Logic logic3 = {{}, {condition3_1,condition3_2,condition3_3}, 3};
	
	return {{logic0,logic1,logic2,logic3}};
}

Link initLinkReward(){
	Observation observation6_1 = {1,0,3,0};
	Observation observation6_2 = {1,1,3,0};
	Condition condition6 = {{observation6_1,observation6_2}};
	Logic logic6 = {{}, {condition6}, 6};
	return {{logic6}};
}

void printObservationList(vector<Observation> observationList){
	for(Observation observation : observationList){
		if(observation.relativeTime==0)
			cout << "[" << "set " << observation.set << ": " << observation.position << "/" << observation.value << "], ";
		else
			cout << "[" << "(-" << observation.relativeTime << ") set " << observation.set << ": " << observation.position << "/" << observation.value << "], ";
	}
	cout << "End" << endl;
}

void printSumUp(){
	cout << endl << "==== [SUM UP " << timeCount << "] ====" << endl;
	//line.display();
	
	for(int i=0; i<linkSuperList[1].size(); i++){
		Link myLink = linkSuperList[1][i];
		cout << "Result " << i << ":" << endl;
		for(Logic myLogic : myLink.logicList){
			cout << myLogic.outcome << endl;
			for(Condition myCondition : myLogic.conditionList){
				cout << "If: ";
				printObservationList(myCondition.observationList);
			}
		}
	}
	
	for(int i=0; i<linkSuperList[2].size(); i++){
		Link myLink = linkSuperList[2][i];
		cout << "Result " << i << ":" << endl;
		for(Logic myLogic : myLink.logicList){
			cout << myLogic.outcome << endl;
			for(Condition myCondition : myLogic.conditionList){
				cout << "If: ";
				printObservationList(myCondition.observationList);
			}
		}
	}
	cout << endl << endl << endl;
}

// CHECK VALUE
//////////////////

bool testLogic(int timestamp, Condition condition){
	for(Observation observation : condition.observationList){
		int valToCheck;
		if(timestamp-observation.relativeTime<0) valToCheck=-1;
		else valToCheck = stateMemSuperList[timestamp-observation.relativeTime][observation.set].state[observation.position];
		if(valToCheck!=observation.value) return false;
	}
	return true;
}

int output(int set, int pos, int timestamp){
	for(int i=linkSuperList[set][pos].logicList.size()-1; i>0; i--){
		for(Condition condition : linkSuperList[set][pos].logicList[i].conditionList){
			if(testLogic(timestamp, condition)) return linkSuperList[set][pos].logicList[i].outcome;
		}
	}
	return linkSuperList[set][pos].logicList[0].outcome;
}

// REGISTER INFO
//////////////////

void associateStateMem(){
	StateMem memoryCommand = StateMem{line.getCommand()};
	StateMem memoryResult = StateMem{line.getResult()};
	StateMem memoryReward = StateMem{line.getReward()};
	
	vector<StateMem> allMem = {memoryCommand, memoryResult, memoryReward};
	stateMemSuperList.push_back(allMem);
}





