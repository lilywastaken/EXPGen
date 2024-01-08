#include "search.h"

int searchBestPath(vector<int> currentState){

	int bestReward = 0;
	int bestPath = -1;
	
	for(int i=0; i<pathList.size(); i++){
		Path path = pathList[i];
		if(path.initialValue != currentState) continue;
		if(path.reward > bestReward){
			bestReward = path.reward;
			bestPath = i;
		}
	}
	
	return bestPath;
}

vector<pair<int,int>> defineConsequence(int action, vector<int> positionList){
	vector<pair<int,int>> output;
	
	for(int position : positionList){
		for(Logic logic : linkSuperList[1][position].logicList){
			for(int i=logic.conditionList.size()-1; i--; i>=0){
				/*for(Condition condition : logic.conditionList[i]){
					
				}*/
			}
		}
	}
	return output;
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

void createPath(vector<Condition> conditionList){

	vector<Path> currentPathList;
	
	cout << endl << "Condition to reach:" << endl;
	for(Condition condition : conditionList) printObservationList(condition.observationList);

	Condition condition = conditionList[0];
	
	for(Observation observation : condition.observationList){
		cout << observation.set << "-" << observation.position << "-" << observation.value << endl;
		for(Logic logic : linkSuperList[observation.set][observation.position].logicList){
			if(logic.outcome!=observation.value) continue;

			cout << "Known outcome: " << logic.outcome << endl;
			for(Condition conditionKnown : logic.conditionList){
				printObservationList(conditionKnown.observationList);
				
				Path newPath;
				newPath.finalValue = {3,3};
				newPath.initialValue = {-1,-1};
				for(Observation observation : conditionKnown.observationList){
					if(observation.set==0) newPath.action.push_back(observation.value);
					else newPath.initialValue[observation.position] = observation.value;
				}
				currentPathList.push_back(newPath);
				break;
			}
			break;
		}
		break;
	}
	
	vector<vector<int>> valueRange;
	
	cout << endl << "=== [SUM UP] ===" << endl;
	for(Path path : currentPathList){
		cout << "Initial: ";
		for(int val : path.initialValue) cout << val << " ";
		cout << endl;
		cout << "Final: ";
		for(int val : path.finalValue) cout << val << " ";
		cout << endl;
		cout << "Action: ";
		for(int val : path.action) cout << val << " ";
		cout << endl;
		
		for(int i=0; i<path.initialValue.size(); i++){
			int val = path.initialValue[i];
			if(val!=-1){
				valueRange.push_back({val});
				continue;
			}
			
			vector<int> possibleVal;
			
			for(Logic logic : linkSuperList[1][i].logicList){
				if(logic.outcome!=path.finalValue[i]) continue;
				
				for(Condition conditionKnown : logic.conditionList){
					for(Observation observation : conditionKnown.observationList){
						if(observation.set==0){
							if(observation.value!=path.action[0]) break;
							cout << "Known outcome: " << logic.outcome << endl;
						}
						if(observation.set==1 && observation.position==i){
							cout << "-- origin: " << observation.value << endl;
							possibleVal.push_back(observation.value);
						}
					}
				}
			}
			valueRange.push_back(possibleVal);
		}
	}
	
	for(int i=0; i<valueRange.size(); i++){
		cout << "TERM " << i << ":" << endl;
		for(int term : valueRange[i]) cout << term << " ";
		cout << endl;
	}
	
	// linkSuperList[1][0] = initLinkResult1();
	// linkSuperList[1][1] = initLinkResult2();
	
	exit(1);
	
	Path newPath;
	newPath.initialValue = {0,0};
	newPath.finalValue = {3,3};
	newPath.action = {1,1,1,2,1,1,1};
	newPath.reward = 6;
	
	pathList.push_back(newPath);
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

vector<Condition> getBestConditionList(){

	if(linkSuperList[2][0].logicList.size()==0) return {};
	
	int bestReward = linkSuperList[2][0].logicList[0].outcome;
	vector<Condition> conditionListBestState = linkSuperList[2][0].logicList[0].conditionList;
	
	for(int i=1; i<linkSuperList[2][0].logicList.size(); i++){
		Logic logic = linkSuperList[2][0].logicList[i];
		if(logic.outcome > bestReward){
			bestReward = logic.outcome;
			conditionListBestState = logic.conditionList;
			break;
		}
	}
	
	return conditionListBestState;

}

vector<int> searchPath(){
	cout << endl << "[SEARCH PATH]" << endl;
	
	vector<Condition> bestConditionList = getBestConditionList();
	
	/////////////////////////////////////
	
	vector<int> currentState = line.getResult();
	
	while(searchBestPath(currentState)==-1){
		createPath(bestConditionList);
	}
	int bestPath = searchBestPath(currentState);
	
	if(bestPath==-1) return {};
	return pathList[bestPath].action;
	
}

