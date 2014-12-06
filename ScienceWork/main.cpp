#include <stdio.h>
#include <iostream>
#include <set>
#include <stack>
#include <fstream>
using namespace std;

#include "States.h"

extern CrossroadState crossroadStates1[];
extern CrossroadState crossroadStates2[];

void AddStates(set<globalState>& stateSet, stack<globalState>& taskQueue, set<CrossroadState>& system1, set<CrossroadState>& system2, int remainingTime, int finishedSystem, ofstream& oNodeFile, ofstream& oEdgeFile, globalState currentState)
{
    globalState newState;
    newState.remainingTime = remainingTime;
    newState.finishedSystem = finishedSystem;
    for (set<CrossroadState>::iterator it1 = system1.begin(); it1 != system1.end(); it1++ )
    {
        for (set<CrossroadState>::iterator it2 = system2.begin(); it2 != system2.end(); it2++ )
        {
            newState.crossroadState1 = *it1;
            newState.crossroadState2 = *it2;
            set<globalState>::iterator it = stateSet.find(newState);
            if (stateSet.end() == it)
            {
                newState.id = stateSet.size() + 1;
#if OUTPUT
                cout<<"inserting: ";newState.Print(cout);
#endif
                newState.Print(oNodeFile);
                PrintStatesTransitionEdge(oEdgeFile, currentState, newState);
                stateSet.insert(newState);
                taskQueue.push(newState);
            }
            else
            {
                PrintStatesTransitionEdge(oEdgeFile, currentState, *it);
            }
        }
    }
}

int main()
{
    set<globalState> stateSet;
    stack<globalState> taskQueue;

    globalState startState;
    startState.crossroadState1=crossroadStates1[0];
    startState.crossroadState2=crossroadStates2[0];
    startState.finishedSystem = 0;
    startState.remainingTime = startState.crossroadState1.time;
    startState.id = 1;

    taskQueue.push(startState);
    stateSet.insert(startState);
    ofstream oEdgeFile("Edges.csv",ofstream::out);
    oEdgeFile <<"Source,Target,Type"<<endl;

    ofstream oNodeFile("Nodes.csv",ofstream::out);
    oNodeFile <<"Id,Label"<<endl;
    startState.Print(oNodeFile);
    while (!taskQueue.empty())
    {
        globalState currentState = taskQueue.top();
        currentState.Print(cout);
        taskQueue.pop();
        bool firstFinished = currentState.finishedSystem == 0 || currentState.finishedSystem == 1;
        bool secondFinished = currentState.finishedSystem == 0 || currentState.finishedSystem == 2;
        int firstTime = firstFinished ? currentState.crossroadState1.time : currentState.remainingTime;
        int secondTime = secondFinished ? currentState.crossroadState2.time : currentState.remainingTime;
        int willFinishSystem = 
            firstTime < secondTime ? 1 :
            (firstTime > secondTime ? 2 : 0);
        int timeToSpend = currentState.remainingTime ? currentState.remainingTime : (willFinishSystem == 1 ? firstTime : secondTime);
        globalState newState;
        switch (willFinishSystem)
	{
	case 1: 
        {
	    set<CrossroadState> newSystems1 = GetNextStates1(currentState.crossroadState1);
	    set<CrossroadState> newSystems2;
	    newSystems2.insert(currentState.crossroadState2);
            AddStates(stateSet, taskQueue, newSystems1, newSystems2, secondTime - timeToSpend, willFinishSystem, oNodeFile, oEdgeFile, currentState);
	    break;
        }
	case 2:
        {
	    set<CrossroadState> newSystems1;
	    set<CrossroadState> newSystems2 = GetNextStates2(currentState.crossroadState2);
	    newSystems1.insert(currentState.crossroadState1);
            AddStates(stateSet, taskQueue, newSystems1, newSystems2, firstTime - timeToSpend, willFinishSystem, oNodeFile, oEdgeFile, currentState);
	    break;
        }
	case 0:
        {
	    set<CrossroadState> newSystems1 = GetNextStates1(currentState.crossroadState1);
	    set<CrossroadState> newSystems2 = GetNextStates2(currentState.crossroadState2);
            AddStates(stateSet, taskQueue, newSystems1, newSystems2, 0, willFinishSystem, oNodeFile, oEdgeFile, currentState);
	    break;
        }
	}
    }
    printf("Hello, world!\n");
    return 0;
}
