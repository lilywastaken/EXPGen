#include "utils.h"
#include "line.h"
#include "deduction.h"
#include "search.h"

using namespace std;

int timeCount = 0;
int action = 0;
int setAmount = 2;
//vector<int> studySet = {0,1};

int commandSize = 1;
int resultSize = 2;
int rewardSize = 1;

Line line(commandSize,resultSize,rewardSize);

vector<vector<StateMem>> stateMemSuperList;
vector<vector<Link>> linkSuperList;
vector<Map> mapList;

vector<int> actionList = {0, 1, 2};
vector<int> valueList = {0, 1, 2, 3};

int main(){

	vector<int> durList;

	stateMemSuperList.clear();
	linkSuperList.clear();
	
	linkSuperList.resize(3);
	
	linkSuperList[0].resize(commandSize); //values
	linkSuperList[1].resize(resultSize); //results
	linkSuperList[2].resize(rewardSize); //results
	
	/*linkSuperList[1][0] = initLinkResult1();
	linkSuperList[1][1] = initLinkResult2();
	linkSuperList[2][0] = initLinkReward();
	
	printSumUp();
	exit(1);*/
	
	//associateStateMem();
	
	/*for(int i=0; i<4; i++){
		line.setCommand({2});
		for(int j=0; j<6+i; j++){
			line.condTest();
			associateStateMem();
			line.setCommand({1});
		}
	}*/
	
	auto start_time = chrono::high_resolution_clock::now();
	srand(time(nullptr));
	
	while(timeCount<1000){
	
		// RESULTS UPDATE
		cout << endl << "=== " << timeCount << " ===" << endl;
		line.setCommand({action});
		line.condTest();
		line.display();
		cout << endl;
		
		associateStateMem();
		
		// PROCESS LOGIC
		// SET - POS - DEPEND OF SET/TIME - MUTE    
		associateLink(1, 0, {{0,0},{1,1}}, true);
		associateLink(1, 1, {{0,0},{1,1}}, true);
		//associateLink(1, 2, {{0,0},{1,1}}, true);
		associateLink(2, 0, {{1,0}}, true);
		
		// DEFINE NEXT ACTION
		
		/*if(timeCount==1) action = 1;
		if(timeCount==2) action = 0;
		if(timeCount==3) action = 1;
		if(timeCount==4) action = 0;
		if(timeCount==5) action = 1;
		if(timeCount==6) action = 0;
		if(timeCount==7) action = 1;
		if(timeCount==8) action = 0;*/
		
		
   		action = (rand() % 2) + 1;
   		
   		/*if(timeCount==1) action = 1;
   		if(timeCount==2) action = 0;
   		if(timeCount==4) action = 1;
   		if(timeCount==5) action = 0;
   		if(timeCount==7) action = 1;
   		if(timeCount==8) action = 0;
   		if(timeCount==10) action = 1;
   		if(timeCount==11) action = 0;
   		
   		if(timeCount==12) break;*/
	
		printSumUp();
		
		timeCount++;
	}
	
	//printSumUp();
	
	vector<int> actionList = searchPath();
	
	// Please re-write this function accordingly
	//reviewCondition();
	
	auto end_time = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
	
	cout << endl << "Time taken: " << duration.count() << " milliseconds" << endl;
	durList.push_back(duration.count());
	
    return 0;
}







