#include "line.h"
    
Line::Line(int commandSize, int resultSize, int rewardSize) : m_commandSize(commandSize), m_resultSize(resultSize), m_rewardSize(rewardSize){
    m_command.resize(m_commandSize);
    m_result.resize(m_resultSize);
    m_reward.resize(m_rewardSize);
	initLine();
}

void Line::initLine(){
	for(int i=0; i<m_result.size(); i++) m_result[i]=0;
}

void Line::addVal(){
	if(m_result[0]<3) m_result[0]++;
	else m_result[0] = 0;
}

void Line::pushVal(){
	for(int i=m_result.size()-1; i>0; i--) m_result[i]=m_result[i-1];
	m_result[0]=0;
}

void Line::condTest(){
	if(m_command[0]==1) addVal();
	if(m_command[0]==2) pushVal();
	
	m_reward[0] = 0;
	for(const int element : m_result) m_reward[0]+=element;
}

void Line::setCommand(vector<int> newCommand){
	for(int i=0; i<newCommand.size(); i++) m_command[i] = newCommand[i];
}

void Line::display(){
	cout << "[";
	for(int command : m_command) cout << command << " ";
	cout << "/ ";
	for(int result : m_result) cout << result << " ";
	cout << "]" << endl;
}

vector<int> Line::getCommand(){
	return m_command;
}

vector<int> Line::getResult(){
	return m_result;
}

vector<int> Line::getReward(){
	return m_reward;
}
