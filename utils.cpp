#include "utils.h"

void loadSet(vector<Set*>& setList, Cube &c){
    setList[0]->valueList = {c.front.getupper().getleft(), c.front.getupper().getright(), c.front.getlower().getleft(), c.front.getlower().getright()};
    setList[1]->valueList = {c.right.getupper().getleft(), c.right.getupper().getright(), c.right.getlower().getleft(), c.right.getlower().getright()};
    setList[2]->valueList = {c.back.getupper().getleft(), c.back.getupper().getright(), c.back.getlower().getleft(), c.back.getlower().getright()};
    setList[3]->valueList = {c.left.getupper().getleft(), c.left.getupper().getright(), c.left.getlower().getleft(), c.left.getlower().getright()};
    setList[4]->valueList = {c.up.getupper().getleft(), c.up.getupper().getright(), c.up.getlower().getleft(), c.up.getlower().getright()};
    setList[5]->valueList = {c.down.getupper().getleft(), c.down.getupper().getright(), c.down.getlower().getleft(), c.down.getlower().getright()};
}


// Swap (0:4), Turn (0:18)
void directAction(vector<int> actionList, Cube &c){
	// Change side
	if(actionList[0] != 0) c.moveSide(actionList[0]-1);
	// Rotate cube	
	switch(actionList[1]){
	case 0: c.U();   break;
	case 1: c.Up();  break;
	case 2: c.U2();  break;
	case 3: c.R();   break;
	case 4: c.Rp();  break;
	case 5: c.R2();  break;
	case 6: c.L();   break;
	case 7: c.Lp();  break;
	case 8: c.L2();  break;
	case 9: c.F();   break;
	case 10: c.Fp(); break;
	case 11: c.F2(); break;
	case 12: c.B();  break;
	case 13: c.Bp(); break;
	case 14: c.B2(); break;
	case 15: c.D();  break;
	case 16: c.Dp(); break;
	case 17: c.D2(); break;
	}
}

void printPCV(PCV pcv){
	if(pcv.reversed) cout << "  Diff value : ";
	else cout << "  Same value : ";
	for(pair<Set*,int> value : pcv.valueList) cout << value.first->name << "/" << value.second << " ";
	cout << endl;
}

void printPEV(PEV pev){
	if(pev.reversed) cout << "  Not equal : ";
	else cout << "  Equal : ";
	for(pair<Set*,int> value : pev.valueList) cout << value.first->name << "/" << value.second << " ";
	cout << "with {";
	for(int equalValue : pev.equalValueList) cout << equalValue << " ";
	cout << "}" << endl;
}

void printSC(SC sc){
	for(PCV pcv : sc.pcvList) printPCV(pcv);
	for(PEV pev : sc.pevList) printPEV(pev);
}

void printTC(TC tcOriginal){
	for(int i=0; i<tcOriginal.conditionList.size(); i++){
		auto subCond = tcOriginal.conditionList[i];
		cout << "Sub condition " << i << ";" << endl;
		
		if(subCond.first.size()>0){
			cout << "  Other TC : ";
			for(TC tc : subCond.first) cout << tc.name << " ";
			cout << endl;
		}
		
		for(SC sc : subCond.second) printSC(sc);
	}
}

bool checkSC(SC scOriginal){
	for(PCV pcv : scOriginal.pcvList){
		vector<int> valueList;
		for(pair<Set*,int> value : pcv.valueList) valueList.push_back(value.first->valueList[value.second]);
		
		/*cout << "Check equal (" << pcv.reversed << "): ";
		for(int element : valueList) cout << element << " ";
		cout << endl;*/
		
		if(pcv.reversed){
			set<int> uniqueValues(valueList.begin(), valueList.end());
			if(uniqueValues.size()!=valueList.size()) return false;
		}
		else{			
			int firstVal = valueList[0];
			for(int i=1; i<valueList.size(); i++){
				if(valueList[i]!=firstVal) return false;
			}
		}
	}
	for(PEV pev : scOriginal.pevList){
	
		/*cout << "Check in list (" << pev.reversed << "): ";
		for(pair<Set,int> element : pev.valueList) cout << element.first.valueList[element.second] << " ";
		cout << "{";
		for(int element : pev.equalValueList) cout << element << " ";
		cout << "}" << endl;*/
		
		if(pev.reversed){
			for(pair<Set*,int> value : pev.valueList){
				int currentVal = value.first->valueList[value.second];
				if(find(pev.equalValueList.begin(), pev.equalValueList.end(), currentVal)!=pev.equalValueList.end()) return false;
			}
		}
		else{
			for(pair<Set*,int> value : pev.valueList){
				int currentVal = value.first->valueList[value.second];
				if(find(pev.equalValueList.begin(), pev.equalValueList.end(), currentVal)==pev.equalValueList.end()) return false;
			}
		}
	}
	return true;
}

bool checkTC(TC tcOriginal){
	for(int i=0; i<tcOriginal.conditionList.size(); i++){
		auto subCond = tcOriginal.conditionList[i];
		cout << "==== SUBCOND " << i << " ====" << endl;
		
		// Check top-condition
		bool checkedTC(true);
		for(TC tc : subCond.first){
			if(!checkTC(tc)){
				checkedTC=false;
				break;
			}
		}
		if(!checkedTC){
			cout << "Failure" << endl;
			continue;
		}
		
		// Check sub-condition
		for(SC sc : subCond.second){
			if(!checkSC(sc)) break;
			cout << "Success" << endl;
			return true;
		}
		
		cout << "Failure" << endl;
	}
	return false;
}

vector<PCV> stateComposition(vector<Set*>& setList, Cube &c){
	loadSet(setList, c);
	//c.displayCube();
	
	vector<PCV> patternCommonValueList;
	
	for(int i=0; i<setList.size(); i++){
		Set *firstSet = setList[i];
		for(int j=0; j<firstSet->valueList.size(); j++){
		
			int firstVal(firstSet->valueList[j]);
		
			bool valueAlreadyStudied(false);
			for(PCV patternCommonValue : patternCommonValueList){
			
				int previousValue(patternCommonValue.valueList[0].first->valueList[patternCommonValue.valueList[0].second]);
				
				if(previousValue == firstVal){
					valueAlreadyStudied=true;
					break;
				}
			}
			
			if(valueAlreadyStudied) continue;
		
			PCV patternCommonValue;
			patternCommonValue.valueList.push_back({firstSet,j});
			for(int k=i; k<setList.size(); k++){
				Set *secondSet = setList[k];
				for(int l=0; l<secondSet->valueList.size(); l++){
					if(i==k && l<=j) continue;
					int secondVal(secondSet->valueList[l]);
					if(firstVal == secondVal) patternCommonValue.valueList.push_back({secondSet,l});
				}
			}
			
			patternCommonValueList.push_back(patternCommonValue);
		}
	}
	
	//for(PCV pcv : patternCommonValueList) printPCV(pcv);
	//cout << endl;
	return patternCommonValueList;
	
}

vector<PCV> correctComposition(vector<PCV> pCVL1, vector<PCV> pCVL2){
	
	vector<PCV> correctedPCVList;
	
	for(PCV pcv1 : pCVL1){
		PCV retainedPCV;
		int maxSize(1);
		for(PCV pcv2 : pCVL2){
			
			PCV correctedPCV;
			
			for(pair<Set*,int> value1 : pcv1.valueList){
				for(pair<Set*,int> value2 : pcv2.valueList){
					
					if(value1.first->name == value2.first->name && value1.second == value2.second){
						correctedPCV.valueList.push_back(value1);
					}
				}
			}
			
			//cout << "========" << endl;
			//printPCV(pcv1);
			//printPCV(pcv2);
			if(correctedPCV.valueList.size()>maxSize){
				//cout << "  Retained " << correctedPCV.valueList.size() << endl;
				maxSize = correctedPCV.valueList.size();
				retainedPCV = correctedPCV;
			}
		}
		if(retainedPCV.valueList.size()>0) correctedPCVList.push_back(retainedPCV);
		//exit(1);
	}
	
	//for(PCV pcv : correctedPCVList) printPCV(pcv);
	//cout << endl;
	return correctedPCVList;
}


vector<pair<Set*,int>> getSimilarList(vector<Set> setListPrev, vector<Set*> &setListNew){
	vector<pair<Set*,int>> similarList;
	
	for(int i=0; i<setListPrev.size(); i++){
		Set setPrev = setListPrev[i];
		Set *setNew = setListNew[i];
		
		for(int j=0; j<setPrev.valueList.size(); j++){
			if(setPrev.valueList[j]==setNew->valueList[j]) similarList.push_back({setNew,j});
		}
		
	}
	
	return similarList;
	
}

// messWithCube(1, similarList, actionSeriesList, vector<int>(), c, allStateOriginal);
bool messWithCube(int remainingMove, vector<vector<int>> &actionSeriesList, vector<int> currentActionSeries, vector<Set*>& setList, Cube c, SC scCondition){
	
	loadSet(setList, c);
	if(checkSC(scCondition)) return true;
	
	if(remainingMove==0) return false;
		
	Cube copy = c;
	
	currentActionSeries.push_back(-1);

	for(int i=0; i<18; i++){
	
		streambuf* old_cout = mute();
	
		// Try each possible move
		switch(i){
		case 0: c.U();   break;
		case 1: c.Up();  break;
		case 2: c.U2();  break;
		case 3: c.R();   break;
		case 4: c.Rp();  break;
		case 5: c.R2();  break;
		case 6: c.L();   break;
		case 7: c.Lp();  break;
		case 8: c.L2();  break;
		case 9: c.F();   break;
		case 10: c.Fp(); break;
		case 11: c.F2(); break;
		case 12: c.B();  break;
		case 13: c.Bp(); break;
		case 14: c.B2(); break;
		case 15: c.D();  break;
		case 16: c.Dp(); break;
		case 17: c.D2(); break;
		}
		
		activate(old_cout);
		
		// 15, 11, 7
		
		//cout << remainingMove << " -> " << i << "/18" << endl;
		
		currentActionSeries[currentActionSeries.size()-1] = i;
		if(remainingMove==1) actionSeriesList.push_back(currentActionSeries);
		
		loadSet(setList, c);
		if(checkSC(scCondition)) return true;
		
		if(messWithCube(remainingMove-1, actionSeriesList, currentActionSeries, setList, c, scCondition)) return true;
		
		c = copy;
	}
	return false;
}


void inputSolvedCube(Cube &c) {
	c.set(0, 0, 0, 0, 0); // up
	c.set(3, 1, 1, 1, 1); // front
	c.set(1, 2, 2, 2, 2); // right
	c.set(4, 3, 3, 3, 3); // back
	c.set(2, 4, 4, 4, 4); // left
	c.set(5, 5, 5, 5, 5); // down
}

void cubeManip(Cube &c){
	c.Up(); c.B(); c.Up(); c.Bp();
	for(int i=0; i<3; i++){c.U(); c.B(); c.Up(); c.Bp();}
	c.D();
	for(int i=0; i<4; i++){c.U(); c.B(); c.Up(); c.Bp();}
	c.D();
	for(int i=0; i<4; i++){c.U(); c.B(); c.Up(); c.Bp();}
}

void generateTestCube(Cube &c, int iteration) {

	cout << "Generating a test cube..." << endl;
	inputSolvedCube(c);
	/* Perform 20 random turns to scramble the cube */
	int turn;
	cout << "Turns from solved cube: ";
	for (int i = 0; i < iteration; i++) {
		turn = rand() % 12; // 12 possible turns
		switch (turn) {
		case 0: c.U(); break;
		case 1: c.Up(); break;
		case 2: c.R(); break;
		case 3: c.Rp(); break;
		case 4: c.L(); break;
		case 5: c.Lp(); break;
		case 6: c.F(); break;
		case 7: c.Fp(); break;
		case 8: c.B(); break;
		case 9: c.Bp(); break;
		case 10: c.D(); break;
		case 11: c.Dp(); break;
		}
	}

	c.displayCube();
}

streambuf* mute(){
	ofstream null_stream("/dev/null");
	streambuf* old_cout = cout.rdbuf();
	cout.rdbuf(null_stream.rdbuf());
	return old_cout;
}

void activate(streambuf* old_cout){
	cout.rdbuf(old_cout);
}
