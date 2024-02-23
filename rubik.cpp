#include "utils.h"

int main(){

	//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////
	// INSTRUCTION :
	// 1) Represent states using patterns only
	// 2) Try to reach step 1, then step 2, etc
	// 3) While trying to reach states, remember the useful operations
	// 4) Create logic to reach states from experimentation (hard)
	//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////

	cout << "Welcome to the 2x2 Rubik's Cube Solver!" << endl << endl;
	
	// CUBE INIT
	////////////////////////////////////////////////////

    srand(23);

	Cube c, solvedCube;
	inputSolvedCube(solvedCube);
	vector<vector<Cube>> testCubeSuperList;
		
	Set setFront{"front"}, setRight{"right"}, setBack{"back"}, setLeft{"left"}, setUp{"up"}, setDown{"down"};
	vector<Set*> setList = {&setFront, &setRight, &setBack, &setLeft, &setUp, &setDown};
	
	// MODIFY TO GET VECTOR OF STATE PER CUBE
	for(int i=0; i<100; i++){
	
		c = solvedCube;
		
		streambuf* old_cout = mute();
		
		generateTestCube(c, 5000);
		vector<Cube> testCubeList = beginnersMethod(c);
		
		activate(old_cout);
		
		for(int j=1; j<testCubeList.size(); j++){
			if(testCubeList[j] == testCubeList[j-1]){
        		testCubeList.erase(testCubeList.begin() + j);
				j--;
			}
		}
		
		if(testCubeList.size()==9) testCubeSuperList.push_back(testCubeList);
		else i--;
	}
	
	//return 1;
	
	// SEARCH SIMILARITIES
	////////////////////////////////////////////////////
	
	vector<SC> scStateList;
	
	// For each state of the resolution
	for(int j=0; j<testCubeSuperList[0].size(); j++){
	
		SC scState;
		
		loadSet(setList, testCubeSuperList[0][j]);
		vector<Set> initList;
		for(Set* ptr : setList) initList.push_back(*ptr);
			
		vector<pair<Set*,int>> similarList = getSimilarList(initList, setList);
		
		// For each cube
		for(int i=1; i<testCubeSuperList.size(); i++){
		
			//testCubeSuperList[0][1].displayCube();
			loadSet(setList, testCubeSuperList[i][j]);
			//testCubeSuperList[i][1].displayCube();
			
			vector<Set> setListPrev;
			for(Set* ptr : setList) setListPrev.push_back(*ptr);
			
			loadSet(setList, testCubeSuperList[i-1][j]);
			
			vector<pair<Set*,int>> newSimilarList = getSimilarList(setListPrev, setList);
			
			
			//cout << "==========" << endl;
			vector<pair<Set*,int>> correctedList;
			for(pair<Set*,int> similar : similarList){
				for(pair<Set*,int> newSimilar : newSimilarList){
					if(similar == newSimilar){
						correctedList.push_back(newSimilar);
						break;
					}
				}
			}
			
			similarList = correctedList;
		
		}
		
		//cout << "===============" << endl;
		if(similarList.size()==0); //cout << "No similarity" << endl;
		
		else{
			for(auto similar : similarList){
				//cout << "Similar " << j << ": " << similar.first->name << " -> " << similar.second << endl;
				
				loadSet(setList, testCubeSuperList[0][j]);
				
				PEV equalValue;
				equalValue.valueList.push_back(similar);
				equalValue.equalValueList.push_back(similar.first->valueList[similar.second]);
				
				scState.pevList.push_back(equalValue);
				
			}
		}
		scStateList.push_back(scState);
	}
	
	// States to reach
	cout << endl << "States to reach" << endl;
	cout << "=============" << endl;
	for(int i=1; i<scStateList.size()-1; i++){
		cout << "State " << i << ":" << endl;
		printSC(scStateList[i]);
	}
	
	cout << endl;
	generateTestCube(c, 20);
	
	//vector<Cube> testCubeList = beginnersMethod(c);
	
	for(int i=0; i<9; i++){
		cout << "=========" << endl;
		cout << "GOAL : STATE " << i << endl;
		cout << "=========" << endl;
	
		vector<vector<int>> actionSeriesList;
		
		int nbTrial = 5;
		for(int j=1; j<nbTrial; j++){
	
			if(!messWithCube(j, actionSeriesList, vector<int>(), setList, c, scStateList[i])){
				cout << "No solution found..." << endl;
				if(j==nbTrial-1){
					cout << "Trying randomly breaks at some point, time to apply logic" << endl;
					exit(1);	
				}
			}
			else{
				cout << "Solution found!" << endl;
				break;
			}
		
		}
		
		if(actionSeriesList.size()>0){
			for(int action : actionSeriesList.back()) directAction({0, action}, c);
		}
		cout << endl;		
		c.displayCube();		
		
	}
	
	return 1;
	
	// ALL VARIABLES / CONDITIONS
	////////////////////////////////////////////////////
	
	// SEARCH PATTERN
	
	PCV sUp, cF, cR1, cR2, cB, cL;
	sUp.valueList = {{&setUp,0},{&setUp,1},{&setUp,2},{&setUp,3}};
	cF.valueList = {{&setFront,0},{&setFront,1}};
	cR1.valueList = {{&setRight,0},{&setRight,1}};
	cR2.valueList = {{&setRight,2},{&setRight,3}};
	cB.valueList = {{&setBack,0},{&setBack,1}};
	cL.valueList = {{&setLeft,0},{&setLeft,1}};
	vector<PCV> crownState = {cF, cB, cR1, cL, sUp};
	
	PCV pcvA, pcvB, pcvC, pcvD, pcvE, pcvF;
	pcvA.valueList = {{&setFront,0},{&setFront,1},{&setFront,3},{&setDown,2}};
	pcvB.valueList = {{&setBack,0},{&setBack,1},{&setFront,2},{&setDown,3}};
	pcvC.valueList = {{&setRight,0},{&setRight,1},{&setBack,2},{&setBack,3}};
	pcvD.valueList = {{&setLeft,0},{&setLeft,1},{&setDown,0},{&setDown,1}};
	pcvE.valueList = {{&setUp,0},{&setUp,1},{&setUp,2},{&setUp,3}};
	pcvF.valueList = {{&setLeft,2},{&setLeft,3},{&setRight,2},{&setRight,3}};
	vector<PCV> testState = {pcvA, pcvB, pcvC, pcvD, pcvE, pcvF};
	
	//for(PCV pcv : crownState) printPCV(pcv);
	//cout << endl;
	
	// Compare every state
	for(int i=0; i<testCubeSuperList[0].size()*0+1; i++){
		
		vector<PCV> pCVL1 = stateComposition(setList, testCubeSuperList[0][i]);
		vector<PCV> pCorrected = pCVL1;
		
		//pCorrected = testState;
		pCorrected = crownState;
		
		for(int j=1; j<testCubeSuperList.size(); j++){
			
			int bestCorr(0);
			vector<PCV> bestPCVList;
			
			for(int k=0; k<testCubeSuperList[j].size()-2; k++){
			
				vector<PCV> pCVL2 = stateComposition(setList, testCubeSuperList[j][k]);
				//cout << "CHECKING" << endl;
				vector<PCV> pchecking = correctComposition(pCorrected, pCVL2);
				//cout << "FINISHED" << endl;
				
				//cout << "Check:" << endl;
				//for(PCV pcv : pchecking) printPCV(pcv);
				
				//testCubeSuperList[j][k].displayCube();
				
				int currentDataSize(0);
				for(PCV pcv : pchecking) currentDataSize += pcv.valueList.size();
				if(currentDataSize > bestCorr){
					//cout << "Better: " << k << endl;
					bestCorr = currentDataSize;
					bestPCVList = pchecking;
				}
			}
			pCorrected = bestPCVList;
			
		}
		cout << "Best:" << endl;
		for(PCV pcv : pCorrected) printPCV(pcv);
		
	}
	
	return 1;

	// ALL PATTERN WE SHOULD FIND
	////////////////////////////////////////////////////
	
	// White side correct
	PEV sUWhite, sDWhite;
	sUWhite.valueList = {{&setUp,0},{&setUp,1},{&setUp,2},{&setUp,3}};
	sUWhite.equalValueList = {0};
	sDWhite.valueList = {{&setDown,0},{&setDown,1},{&setDown,2},{&setDown,3}};
	sDWhite.equalValueList = {0};
	
	// Crown correct
	PCV sFUp, sFDown, sRUp, sRDown, sBUp, sBDown, sLUp, sLDown;
	sFUp.valueList = {{&setFront,0},{&setFront,1}};
	sFDown.valueList = {{&setFront,2},{&setFront,3}};
	sRUp.valueList = {{&setRight,0},{&setRight,1}};
	sRDown.valueList = {{&setRight,2},{&setRight,3}};
	sBUp.valueList = {{&setBack,0},{&setBack,1}};
	sBDown.valueList = {{&setBack,2},{&setBack,3}};
	sLUp.valueList = {{&setLeft,0},{&setLeft,1}};
	sLDown.valueList = {{&setLeft,2},{&setLeft,3}};
		
	// VERIFY CROWN STATE
	////////////////////////////////////////////////////
	
	// Requirement : all 6 faces set, comparition of all crown results

	SC crownCompleteUp;
	crownCompleteUp.pcvList = {sFUp, sRUp, sBUp, sLUp};
	crownCompleteUp.pevList = {sUWhite};
	
	SC crownCompleteDown;
	crownCompleteDown.pcvList = {sFDown, sRDown, sBDown, sLDown};
	crownCompleteDown.pevList = {sDWhite};

	TC crownComplete{"crownComplete"};
	crownComplete.conditionList = {{{},{crownCompleteDown}},
								   {{},{crownCompleteUp}}};
	
	printTC(crownComplete);					   
	bool crownCompleteChecked = checkTC(crownComplete);
	
	return 1;
	
	// PROCESSING
	////////////////////////////////////////////////////
	
	
	// 1) I want to see how to achieve my goal
	
	// Check if face1 complete
	
	/*systemAction({1,0,2,0}, probeValue, trashCond, c);
	systemAction({0,0,0,1}, probeValue, t2ok, c);
	systemAction({0,0,3,0}, probeValue, trashCond, c);
	systemAction({0,0,0,2}, probeValue, t3ok, c);
	systemAction({0,0,4,0}, probeValue, trashCond, c);
	systemAction({0,0,0,3}, probeValue, t4ok, c);*/
	
	// 2) Direct way (solved) not functionnal, checking way (4-correct) to solved
	
	// 3) Way to solved (4-correct) not functionnal, checking way (crown) to 4-correct
	
	// 4) Way to 4-correct (crown) FUNCTIONNAL, applying protocol crown->4-correct
	
	
	
	// check if one face is white
	// check if crown correct
	
	
	
	
	c.displayCube();
	
	return 1;
	
	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	
	
	//Scond face1SolvedCond;
	//Scond rubikSolvedCond = {{}};
	
	bool face1Complete(false), face2Complete(false), face3Complete(false),
	face4Complete(false), face5Complete(false), face6Complete(false);
	
	// rubik's cube solved
	bool rubikSolved = face1Complete && face2Complete && face3Complete
						&& face4Complete && face5Complete && face6Complete;
	
	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	
	//return 1;
	
	// Orientate it like in the video
	c.moveSide(1);
	c.moveSide(1);
	c.moveSide(2);
	c.moveSide(2);
	
	c.displayCube();
	
	/////////////////////////////////////
	// SEARCH WHITE CROWN
	/////////////////////////////////////
	
	
	bool fullWhite(false);
	bool crown1(false), crown2(false), crown3(false), crown4(false);
	bool noDoubleCrown(false);
	
	vector<int> seriesOfMove = {1,1,1,2,2,2,-1};
	vector<int> crownColors;
	
	// Check white side
	for(int move : seriesOfMove){
		if(c.front.getupper().getleft()==0
		&& c.front.getupper().getright()==0
		&& c.front.getlower().getleft()==0
		&& c.front.getlower().getright()==0){
			cout << "Found you" << endl;
			fullWhite = true;
			break;
		}
		cout << "Move " << move << endl;
		c.moveSide(move);
	}
	
	c.displayCube();
	
	// Check crown 1
	c.moveSide(2);
	c.displayCube();
	if(c.front.getlower().getleft() == c.front.getlower().getright()){
		crown1 = true;
		crownColors.push_back(c.front.getlower().getleft());
	}
	
	// Check crown 2
	c.moveSide(1);
	c.displayCube();
	if(c.front.getlower().getleft() == c.front.getlower().getright()){
		crown2 = true;
		crownColors.push_back(c.front.getlower().getleft());
	}
	
	// Check crown 3
	c.moveSide(1);
	c.displayCube();
	if(c.front.getlower().getleft() == c.front.getlower().getright()){
		crown3 = true;
		crownColors.push_back(c.front.getlower().getleft());
	}
	
	// Check crown 4
	c.moveSide(1);
	c.displayCube();
	if(c.front.getlower().getleft() == c.front.getlower().getright()){
		crown4 = true;
		crownColors.push_back(c.front.getlower().getleft());
	}
	
	// Check no double crown
	if(crownColors[0] != crownColors[1] && crownColors[0] != crownColors[2] && crownColors[0] != crownColors[3]
	&& crownColors[1] != crownColors[2] && crownColors[1] != crownColors[3] && crownColors[2] != crownColors[3]){
		noDoubleCrown = true;
	}
	
	if(fullWhite && crown1 && crown2 && crown3 && crown4 && noDoubleCrown){
		cout << "FUNCTIONNAL" << endl;
	}
	
	// PROTOCOL CROWN -> 4-CORRECT
	
	bool solved1(false), solved2(false), solved3(false), solved4(false);
	
	c.moveSide(1);
	c.moveSide(1);
	c.displayCube();
	
	int val1, val2, val3, val4, val5;
	bool val1Valid, val2Valid, val3Valid;
	
	// Check solved 1
	val1 = c.front.getupper().getright(), val2 = c.up.getlower().getright(), val3 = c.right.getupper().getleft();
	val4 = c.front.getlower().getright(), val5 = c.right.getlower().getleft();
	val1Valid = false, val2Valid = false, val3Valid = false;
	if(val1 == val4 || val1 == val5) val1Valid = true;
	if(val2 == val4 || val2 == val5) val2Valid = true;
	if(val3 == val4 || val3 == val5) val3Valid = true;
	if(val1Valid && val2Valid || val1Valid && val3Valid || val2Valid && val3Valid) solved1 = true;
	
	// Check solved 2
	c.moveSide(1);
	c.displayCube();
	
	val1 = c.front.getupper().getright(), val2 = c.up.getlower().getright(), val3 = c.right.getupper().getleft();
	val4 = c.front.getlower().getright(), val5 = c.right.getlower().getleft();
	val1Valid = false, val2Valid = false, val3Valid = false;
	if(val1 == val4 || val1 == val5) val1Valid = true;
	if(val2 == val4 || val2 == val5) val2Valid = true;
	if(val3 == val4 || val3 == val5) val3Valid = true;
	if(val1Valid && val2Valid || val1Valid && val3Valid || val2Valid && val3Valid) solved2 = true;
	
	// Check solved 3
	c.moveSide(1);
	c.displayCube();
	
	val1 = c.front.getupper().getright(), val2 = c.up.getlower().getright(), val3 = c.right.getupper().getleft();
	val4 = c.front.getlower().getright(), val5 = c.right.getlower().getleft();
	val1Valid = false, val2Valid = false, val3Valid = false;
	if(val1 == val4 || val1 == val5) val1Valid = true;
	if(val2 == val4 || val2 == val5) val2Valid = true;
	if(val3 == val4 || val3 == val5) val3Valid = true;
	if(val1Valid && val2Valid || val1Valid && val3Valid || val2Valid && val3Valid) solved3 = true;
	
	
	// Check solved 4
	c.moveSide(1);
	c.displayCube();
	
	val1 = c.front.getupper().getright(), val2 = c.up.getlower().getright(), val3 = c.right.getupper().getleft();
	val4 = c.front.getlower().getright(), val5 = c.right.getlower().getleft();
	val1Valid = false, val2Valid = false, val3Valid = false;
	if(val1 == val4 || val1 == val5) val1Valid = true;
	if(val2 == val4 || val2 == val5) val2Valid = true;
	if(val3 == val4 || val3 == val5) val3Valid = true;
	if(val1Valid && val2Valid || val1Valid && val3Valid || val2Valid && val3Valid) solved4 = true;
	
	cout << solved1 << "/" << solved2 << "/" << solved3 << "/" << solved4 << endl;
	
	// PROTOCOL 4-CORRECT -> SOLVED
	
	// Search color section on top
	int lastColor;
	for(int i=1; i<6; i++){
		bool colorIn(false);
		for(int color : crownColors){
			if(color==i){
				colorIn = true;
				break;
			}
		}
		if(!colorIn){
			lastColor = i;
			break;
		}
	}
	
	cout << "Last color: " << lastColor << endl;
	
	// Orientate it like in the video
	c.moveSide(1);
	c.moveSide(1);
	c.moveSide(2);
	c.moveSide(2);
	
	while(c.front.getlower().getright() != lastColor && c.front.getlower().getleft() != lastColor){
		c.moveSide(1);
	}	
	
	while(!c.isSolved()){
	//for(int i=0; i<2; i++){
		
		//if(c.isSolved()) break;
		while(c.front.getlower().getright() != lastColor && c.front.getlower().getleft() != lastColor && !c.isSolved()){
			c.Dp();
		}
		
		if(c.isSolved()) break;
		c.displayCube();
		
		bool leftPart(false);
		if(c.front.getlower().getleft() == lastColor) leftPart = true;
		
		bool lastColorShifted(false);
		while(!lastColorShifted){
		
			cout << "Turn..." << endl;
		
			c.R();
			c.U();
			c.Rp();
			c.Up();
		
			if(leftPart && c.down.getupper().getleft() == lastColor) lastColorShifted = true;
			else if (c.down.getupper().getright() == lastColor)	lastColorShifted = true;
		}
		
		cout << "Ok" << endl;
	}
	
	cout << "Congrats." << endl;
	
	
	c.displayCube();
	
	
	
	
	
	
	
	
	
	
	// Crown is complete
	
	/*cout << ">> " << c.up.getupper().getleft() << endl;
	cout << ">> " << c.up.getupper().getright() << endl;
	cout << ">> " << c.up.getlower().getleft() << endl;
	cout << ">> " << c.up.getlower().getright() << endl;
	
	cout << ">> " << c.front.getupper().getleft() << endl;
	cout << ">> " << c.front.getupper().getright() << endl;
	
	cout << ">> " << c.back.getupper().getleft() << endl;
	cout << ">> " << c.back.getupper().getright() << endl;
	
	cout << ">> " << c.right.getupper().getleft() << endl;
	cout << ">> " << c.right.getupper().getright() << endl;
	
	cout << ">> " << c.left.getupper().getleft() << endl;
	cout << ">> " << c.left.getupper().getright() << endl;*/
	
	/*c.R();
	c.Rp();
	c.L();
	c.Lp();
	c.F();
	c.Fp();
	c.B();
	c.Bp();
	c.D();
	c.Dp();*/
	
	//beginnersMethod(c);
	
	return 0;
}









