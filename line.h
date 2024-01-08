#ifndef MEM_LINE_H
#define MEM_LINE_H

#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <cstring>
#include <algorithm>

using namespace std;

class Line {
private:
    vector<int> m_command, m_result, m_reward;
    int m_commandSize, m_resultSize, m_rewardSize;
    
public:
	Line(int commandSize, int resultSize, int rewardSize);
    void initLine();
    void addVal();
    void pushVal();
    void condTest();
    void setCommand(vector<int> newCommand);
    void display();
    vector<int> getCommand();
    vector<int> getResult();
    vector<int> getReward();
};

#endif
