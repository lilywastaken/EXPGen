#include "search.h"

int searchBestPath(vector<int> currentState){

	int bestReward = 0;
	int bestPath = -1;
	
	for(int i=0; i<pathList.size(); i++){
		Path path = pathList[i];
		if(path.initialValue != currentState) continue;
		
		cout << "PATH FOUND!!" << endl;
		exit(1);
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

void printPath(Path path){
	cout << "  -Initial: ";
	for(int val : path.initialValue) cout << val << " ";
	cout << endl;
	cout << "  -Final: ";
	for(int val : path.finalValue) cout << val << " ";
	cout << endl;
	cout << "  -Action: " << path.action << endl;
	cout << endl;
}

void searchFunctionalPath(PotentialPath potentialPath, vector<ComplexPath> currentPathList, int startPos, vector<vector<int>> &functionalPathList, vector<int> pathList){

	//cout << "Act " << potentialPath.action << endl;
	
	int i = startPos;
	//cout << "--> " << i << endl;

	vector<int> initialValue = potentialPath.initialValue[i];
	//cout << " Pos " << i << endl;
	for(int val : initialValue){
		
		pathList.push_back(val);
		
		//cout << "  -> PATH" << endl;
		vector<int> possibleValue(currentPathList[0].initialValue.size(), -1);
		for(int path : pathList){
			//cout << "  ID " << path << ": ";
			for(int i=0; i<currentPathList[path].initialValue.size(); i++){
				int val = currentPathList[path].initialValue[i];
				//cout << val << " ";
				if(possibleValue[i] != -1 && val !=-1 && possibleValue[i] != val) return;
				if(val!=-1) possibleValue[i] = val;
			}
			//cout << endl;
		}
		
		if(startPos==potentialPath.initialValue.size()-1){
			functionalPathList.push_back(pathList);
			//cout << "RETURN VAL" << endl;
			return;
		}
		//cout << "ENTER FUNC" << endl;
		searchFunctionalPath(potentialPath, currentPathList, i+1, functionalPathList, pathList);
		
		
	}
	//cout << endl;
}

ComplexPath getComplexPath(Condition condition, int finalPos){
	ComplexPath newPath;
	vector<int> initialValue = {-1,-1};
	printObservationList(condition.observationList);
	
	for(Observation observation : condition.observationList){
		if(observation.set==0) newPath.action = observation.value;
		if(observation.set==1) initialValue[observation.position] = observation.value;
	}
	
	newPath.initialValue = initialValue;
	newPath.finalValuePos = finalPos;
	
	return newPath;
	
}

vector<pair<int,vector<int>>> getBannedValueList(Logic logic){

	cout << "Studying opposite " << logic.outcome << "..." << endl;

	vector<pair<int,vector<int>>> bannedValueList;

	for(Condition condition : logic.conditionList){
		vector<int> bannedValue = {-1, -1};
		int bannedAction = -1;
		for(Observation observation : condition.observationList){
			if(observation.set==0) bannedAction = observation.value;
			if(observation.set==1) bannedValue[observation.position] = observation.value;
		}
		bannedValueList.push_back(make_pair(bannedAction, bannedValue));
	}
	
	cout << "Banned values:" << endl;
	for(auto bannedValue : bannedValueList){
		cout << bannedValue.first << " - ";
		for(auto value : bannedValue.second) cout << value << " ";
		cout << endl;
	}
	
	return bannedValueList;
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

void improveMap(Path path){
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

void createPath(vector<int> endState){
	
	vector<ComplexPath> currentPathList;
	
	cout << "Ending:" << endl;
	for(int element : endState) cout << element << " ";
	cout << endl;
	
	
	for(int i=0; i<endState.size(); i++){
		cout << "<VAL " << i << ">" << endl;
		for(int j=0; j<linkSuperList[1][i].logicList.size(); j++){
			Logic logic = linkSuperList[1][i].logicList[j];
			if(logic.outcome!=endState[i]) continue;
			
			cout << "Known outcome: " << logic.outcome << endl;
			
			// Look for banned states
			vector<pair<int,vector<int>>> bannedValueList;
			for(int k=j+1; k<linkSuperList[1][i].logicList.size(); k++){
				bannedValueList = getBannedValueList(linkSuperList[1][i].logicList[k]);
			}
			
			// Look for paths that can be directly reached
			for(Condition condition : logic.conditionList){
				ComplexPath newPath = getComplexPath(condition,i);
				newPath.bannedValueList = bannedValueList;
				currentPathList.push_back(newPath);
			}
			
			if(logic.conditionList.size()==0){
				cout << "Works for act -1: {-1, -1}" << endl;
				ComplexPath newPath;
				newPath.initialValue = {-1,-1};
				newPath.bannedValueList = bannedValueList;
				newPath.finalValuePos = i;
				newPath.action = -1;
				currentPathList.push_back(newPath);
			}
			
		}
	}
	
	vector<PotentialPath> potentialPathList;
	
	cout << endl;
	for(int i=0; i<currentPathList.size(); i++){
		ComplexPath path = currentPathList[i];
		cout << "  -Initial: ";
		for(int val : path.initialValue) cout << val << " ";
		cout << endl;
		cout << "  -Final: " << path.finalValuePos << endl;
		cout << "  -Action: " << path.action << endl;
		cout << endl;
		
		bool pathFound = false;
		for(auto &potentialPath : potentialPathList){
			if(potentialPath.action == path.action || potentialPath.action == -1 || path.action == -1){
				potentialPath.initialValue[path.finalValuePos].push_back(i);
				pathFound = true;
				break;
			}
		}
		if(!pathFound){
			PotentialPath potentialPath;
			potentialPath.action = path.action;
			potentialPath.initialValue[path.finalValuePos].push_back(i);
			potentialPathList.push_back(potentialPath);
		}
	}
	
	cout << "==List==" << endl;
	for(PotentialPath potentialPath : potentialPathList){
	
		vector<vector<int>> functionalPathList;
		searchFunctionalPath(potentialPath, currentPathList, 0, functionalPathList, vector<int>());
		
		cout << "Action " << potentialPath.action << endl;
		
		for(vector<int> functionalPath : functionalPathList){
			cout << "- Path: ";
			for(int val : functionalPath) cout << val << " ";
			cout << endl;
			
			
			
			vector<int> possibleValue(currentPathList[0].initialValue.size(), -1);
			for(int path : functionalPath){
				for(int i=0; i<currentPathList[path].initialValue.size(); i++){
					int val = currentPathList[path].initialValue[i];
					if(possibleValue[i] != -1 && val !=-1 && possibleValue[i] != val) return;
					if(val!=-1) possibleValue[i] = val;
				}
			}
			
			cout << "- Vals: ";
			for(int val : possibleValue) cout << val << " ";
			cout << endl;
			
			
			Path newPath;
			
			newPath.action = potentialPath.action;
			newPath.initialValue = possibleValue;
			newPath.finalValue = endState;
			
			pathList.push_back(newPath);
			
			improveMap(newPath);
		}
		
		cout << "=====" << endl;
	}
	
	cout << endl << "=== FINAL SUM UP ===" << endl;
	for(auto path : pathList){
		cout << "Action " << path.action << ": ";
		for(int val : path.initialValue) cout << val << " ";
		cout << " -> ";
		for(int val : path.finalValue) cout << val << " ";
		cout << endl;
	}	
}

vector<vector<int>> getBestStateList(){

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
	
	vector<vector<int>> possibleEndingList;
	
	for(Condition condition : conditionListBestState){		
		vector<int> currentEnding = {-1,-1};
		for(int i=0; i<condition.observationList.size(); i++){
			Observation observation = condition.observationList[i];
			currentEnding[observation.position] = observation.value;
		}
		possibleEndingList.push_back(currentEnding);
	}
	
	return possibleEndingList;

}

vector<int> searchPath(){
	cout << endl << "[SEARCH PATH]" << endl;
	
	vector<vector<int>> bestStateList = getBestStateList();
	
	vector<int> bestState = bestStateList[0];
	
	/////////////////////////////////////
	
	vector<int> currentState = line.getResult();
	
	createPath({0,3});
	
	exit(1);
	
	
	/*while(searchBestPath(currentState)==-1){
		createPath(bestStateList);
	}*/
	int bestPath = searchBestPath(currentState);
	
	return {};
	//if(bestPath==-1) return {};
	//return pathList[bestPath].action;
	
}

