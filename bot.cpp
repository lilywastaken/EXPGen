#include "utils.h"
#include "line.h"
#include "deduction.h"
#include "core.h"

using namespace std;

int timeCount = 0;
int action = 0;
int setAmount = 2;

int commandSize = 1;
int resultSize = 2;
int rewardSize = 1;

Line line(commandSize,resultSize,rewardSize);

vector<vector<StateMem>> stateMemSuperList;
vector<vector<Link>> linkSuperList;
vector<Path> pathList;
vector<int> actionList = {0, 1, 2};

int main(){

	vector<int> durList;

	stateMemSuperList.clear();
	linkSuperList.clear();
	
	linkSuperList.resize(3);
	
	linkSuperList[0].resize(commandSize); //values
	linkSuperList[1].resize(resultSize); //results
	linkSuperList[2].resize(rewardSize); //results
	
	linkSuperList[1][0] = initLinkResult1();
	linkSuperList[1][1] = initLinkResult2();
	linkSuperList[2][0] = initLinkReward();	
	printSumUp();
	
	associateStateMem();
	
	/*for(int i=0; i<4; i++){
		line.setCommand({2});
		for(int j=0; j<6+i; j++){
			line.condTest();
			associateStateMem();
			line.setCommand({1});
		}
	}*/
	
	auto start_time = chrono::high_resolution_clock::now();
	
	cout << "WORKING" << endl;
	coreProcess();
	
	/*
	
	while(timeCount<5){
	
		// RESULTS UPDATE
		cout << "=== " << timeCount << " ===" << endl;
		line.setCommands({action});
		line.condTest();
		line.display();
		cout << endl;
		
		associateStateMem();
		
		// PROCESS LOGIC
		
		observeMethod();
		
		action = 1;
		
		if(timeCount==7) action = 2;
		if(timeCount==9) action = 2;
		if(timeCount==12) action = 2;
		
		if(timeCount==14) action = 2;
		if(timeCount==15) action = 0;
		if(timeCount==17) action = 0;
		if(timeCount==19) action = 0;
		if(timeCount==21) action = 0;
		if(timeCount==23) action = 0;
		if(timeCount==25) action = 0;
		
		printSumUp();
		
		timeCount++;
	}
	
	*/
	
	// Please re-write this function accordingly
	//reviewCondition();
	//printSumUp();
	
	auto end_time = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
	
	cout << endl << "Time taken: " << duration.count() << " milliseconds" << endl;
	durList.push_back(duration.count());
	
    return 0;
}







