#include "search.h"

// Display path
void printPath(Path path){
	cout << "  -Initial: ";
	for(int val : path.transition.initialValue) cout << val << " ";
	cout << endl;
	cout << "  -Final: ";
	for(int val : path.finalValue) cout << val << " ";
	cout << endl;
	cout << "  -Action: " << path.transition.action << endl;
	cout << endl;
}

// Search if any path is linked to initial value
int searchBestPath(vector<int> currentState){

	int bestReward = 0;
	int bestPath = -1;
	
	for(int i=0; i<pathList.size(); i++){
		Path path = pathList[i];
		if(path.transition.initialValue != currentState) continue;
		
		cout << "PATH FOUND!!" << endl;
		exit(1);
		if(path.reward > bestReward){
			bestReward = path.reward;
			bestPath = i;
		}
	}
	
	return bestPath;
}

// Search for the best reward within reward logic list and send back state to reach list
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

// Recursive function to locate each valid combination of states
void searchFunctionalPath(PotentialPath potentialPath, vector<StepPath> currentPathList, vector<vector<int>> &functionalPathList, int startPos, vector<int> requirement, vector<int> pathList){
	vector<int> set = potentialPath.initialValueList[startPos];
	
	for(int ID : set){
		StepPath stepPath = currentPathList[ID];
		
		if(startPos==0){
			requirement = stepPath.transition.initialValue;
			pathList.clear();
		}
		else if(requirement!=stepPath.transition.initialValue) continue;
		pathList.push_back(ID);
		
		if(startPos>potentialPath.initialValueList.size()-2){
			functionalPathList.push_back(pathList);
			continue;
		}
		else searchFunctionalPath(potentialPath, currentPathList, functionalPathList, startPos+1, requirement, pathList);
	}
}


vector<Transition> deduceAllTransition(Transition originalTransition){
	
	vector<Transition> transitionList;
	if(originalTransition.action==-1){
		for(int action : actionList){
			originalTransition.action = action;
			transitionList.push_back(originalTransition);
		}
	}
	else transitionList.push_back(originalTransition);
	
	for(int i=0; i<originalTransition.initialValue.size(); i++){
		vector<Transition> newTransitionList;
		for(Transition transition : transitionList){
			if(transition.initialValue[i] == -1){
				for(int value : valueList){
					Transition newTransition = transition;
					newTransition.initialValue[i] = value;
					newTransitionList.push_back(newTransition);
				}
			}
			else newTransitionList.push_back(transition);
		}
		transitionList = newTransitionList;
	}
	
	return transitionList;

}

// Generate all possible values from a logic
vector<StepPath> getStepPathList(Condition condition, int finalPos, vector<Transition> bannedTransitionList){

	//printObservationList(condition.observationList);
	
	StepPath originalStepPath;
	vector<int> initialValue = {-1,-1};
	int originalAction = -1;
	
	for(Observation observation : condition.observationList){
		if(observation.set==0) originalAction = observation.value;
		if(observation.set==1) initialValue[observation.position] = observation.value;
	}
	originalStepPath.transition.initialValue = initialValue;
	originalStepPath.transition.action = originalAction;
	originalStepPath.finalValuePos = finalPos;
	
	vector<Transition> transitionList = deduceAllTransition(originalStepPath.transition);
	vector<StepPath> stepPathList;
	
	for(Transition transition : transitionList){
	
		bool bannedFound = false;
		for(Transition bannedTransition : bannedTransitionList){
			if(transition.action == bannedTransition.action
			&& transition.initialValue == bannedTransition.initialValue){
				bannedFound = true;
				break;
			}
		}
		
		if(bannedFound) continue;
		
		StepPath newStepPath;
		newStepPath.transition = transition;
		newStepPath.finalValuePos = finalPos;
		stepPathList.push_back(newStepPath);
	}
	
	/*for(StepPath stepPath : stepPathList){
		cout << "--> action: " << stepPath.transition.action << endl;
		cout << "--> final: " << stepPath.finalValuePos << endl;
		cout << "   => ";
		for(int val : stepPath.transition.initialValue) cout << val << " ";
		cout << endl << endl;
	}*/
	
	return stepPathList;
	
}

// Acquire all the invalid values from conditions above
vector<Transition> getBannedTransitionList(Logic logic){

	//cout << "Studying opposite " << logic.outcome << "..." << endl;

	vector<Transition> bannedTransitionList;

	for(Condition condition : logic.conditionList){
		vector<int> bannedState = {-1, -1};
		int bannedAction = -1;
		for(Observation observation : condition.observationList){
			if(observation.set==0) bannedAction = observation.value;
			if(observation.set==1) bannedState[observation.position] = observation.value;
		}
		Transition bannedTransition;
		bannedTransition.action = bannedAction;
		bannedTransition.initialValue = bannedState;
		bannedTransitionList.push_back(bannedTransition);
	}
	
	vector<Transition> finalBannedTransitionList;
	for(Transition transition : bannedTransitionList){
		vector<Transition> expandedTransitionList = deduceAllTransition(transition);
		for(Transition expandedTransition : expandedTransitionList){
			finalBannedTransitionList.push_back(expandedTransition);
		}
	}
	
	/*cout << "Banned values:" << endl;
	for(auto bannedTransition : finalBannedTransitionList){
		cout << bannedTransition.action << " - ";
		for(auto value : bannedTransition.initialValue) cout << value << " ";
		cout << endl;
	}*/
	
	return finalBannedTransitionList;
}

bool filterPath(){
	return false;
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

void createPath(vector<int> endState){

	pathList.clear();
	
	vector<StepPath> currentPathList;	
	
	for(int i=0; i<endState.size(); i++){
		for(int j=0; j<linkSuperList[1][i].logicList.size(); j++){
			Logic logic = linkSuperList[1][i].logicList[j];
			if(logic.outcome!=endState[i]) continue;
			
			// Look for banned states
			vector<Transition> bannedTransitionList;
			for(int k=j+1; k<linkSuperList[1][i].logicList.size(); k++){
				vector<Transition> tempBannedTransitionList = getBannedTransitionList(linkSuperList[1][i].logicList[k]);
				for(Transition bannedTransition : tempBannedTransitionList){
					bannedTransitionList.push_back(bannedTransition);
				}
			}
			
			// Look for paths that can be directly reached
			for(Condition condition : logic.conditionList){
				vector<StepPath> newStepPathList = getStepPathList(condition,i,bannedTransitionList);
				for(StepPath stepPath : newStepPathList) currentPathList.push_back(stepPath);
			}
			
			// No path (core value)
			if(logic.conditionList.size()==0){
				vector<StepPath> newStepPathList = getStepPathList(Condition(),i,bannedTransitionList);
				for(StepPath stepPath : newStepPathList) currentPathList.push_back(stepPath);
			}
			
		}
		
	}
	
	vector<PotentialPath> potentialPathList;
	
	for(int i=0; i<currentPathList.size(); i++){
	
		/*cout << "=== " << i << " ===" << endl;
		cout << "  -Initial: ";
		for(int val : path.transition.initialValue) cout << val << " ";
		cout << endl;
		cout << "  -Final: " << path.finalValuePos << endl;
		cout << "  -Action: " << path.transition.action << endl;
		cout << endl;*/		
	
		StepPath path = currentPathList[i];
		bool pathFound = false;
		for(auto &potentialPath : potentialPathList){
			if(potentialPath.action == path.transition.action){
				potentialPath.initialValueList[path.finalValuePos].push_back(i);
				pathFound = true;
				break;
			}
		}
		if(!pathFound){
			PotentialPath potentialPath;
			potentialPath.action = path.transition.action;
			potentialPath.initialValueList[path.finalValuePos].push_back(i);
			potentialPathList.push_back(potentialPath);
		}
	}
	
	/*for(PotentialPath potentialPath : potentialPathList){
		cout << potentialPath.action << endl;
		for(vector<int> initialValue : potentialPath.initialValueList){
			for(int val : initialValue) cout << val << " ";
			cout << endl;
		}
	}*/
	
	for(PotentialPath potentialPath : potentialPathList){
	
		vector<vector<int>> functionalPathList;
		searchFunctionalPath(potentialPath, currentPathList, functionalPathList, 0, {-1,-1}, vector<int>());
		
		for(vector<int> functionalPath : functionalPathList){
			
			Path newPath;
			
			newPath.transition.action = potentialPath.action;
			newPath.transition.initialValue = currentPathList[functionalPath[0]].transition.initialValue;
			newPath.finalValue = endState;
			
			pathList.push_back(newPath);
		}
	}
	
	cout << endl << "=== FINAL SUM UP ===" << endl;
	for(auto path : pathList){
		cout << "Action " << path.transition.action << ": ";
		for(int val : path.transition.initialValue) cout << val << " ";
		cout << " -> ";
		for(int val : path.finalValue) cout << val << " ";
		cout << endl;
	}	
}


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

vector<int> searchPath(){
	cout << endl << "[SEARCH PATH]" << endl;
	
	vector<vector<int>> bestStateList = getBestStateList();
	vector<int> bestState = bestStateList[0];
	
	/////////////////////////////////////
	
	createPath({3,3});
	createPath({2,3});
	createPath({1,3});
	createPath({0,3});
	createPath({3,0});
	createPath({2,0});
	createPath({1,0});
	
	exit(1);
	
	return {};
	
}

