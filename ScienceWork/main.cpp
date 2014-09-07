#include <stdio.h>
#include <iostream>
#include <set>
#include <stack>
using namespace std;

#include "States.h"

extern CrossroadState crossroadStates1[];
extern CrossroadState crossroadStates2[];

int main()
{
  set<globalState> stateSet;
  stack<globalState> taskQueue;

  globalState startState;
  startState.crossroadState1=crossroadStates1[0];
  startState.crossroadState2=crossroadStates2[0];
  startState.finishedSystem = 2;
  startState.remainingTime = startState.crossroadState1.time;

  taskQueue.push(startState);
  stateSet.insert(startState);

  while (!taskQueue.empty())
    {
      globalState currentState = taskQueue.top();
      currentState.Print();
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
	    newState.remainingTime = currentState.crossroadState2.time - timeToSpend;
	    newState.finishedSystem = willFinishSystem;
	    for (set<CrossroadState>::iterator it1 = newSystems1.begin(); it1 != newSystems1.end(); it1++ )
	      {
		for (set<CrossroadState>::iterator it2 = newSystems2.begin(); it2 != newSystems2.end(); it2++ )
		  {
		    newState.crossroadState1 = *it1;
		    newState.crossroadState2 = *it2;
		    if (stateSet.end() == stateSet.find(newState))
		      {
#if OUTPUT
			cout<<"inserting: ";newState.Print();
#endif
			stateSet.insert(newState);
			taskQueue.push(newState);
		      }
		  }
	      }
	    break;
	  }
	case 2:
	  {
	    set<CrossroadState> newSystems1;
	    set<CrossroadState> newSystems2 = GetNextStates2(currentState.crossroadState2);
	    newSystems1.insert(currentState.crossroadState1);
	    newState.remainingTime = currentState.crossroadState1.time - timeToSpend;
	    newState.finishedSystem = willFinishSystem;
	    for (set<CrossroadState>::iterator it1 = newSystems1.begin(); it1 != newSystems1.end(); it1++ )
	      {
		for (set<CrossroadState>::iterator it2 = newSystems2.begin(); it2 != newSystems2.end(); it2++ )
		  {
		    newState.crossroadState1 = *it1;
		    newState.crossroadState2 = *it2;
		    if (stateSet.end() == stateSet.find(newState))
		      {
#if OUTPUT
			cout<<"inserting: ";newState.Print();
#endif
			stateSet.insert(newState);
			taskQueue.push(newState);
		      }
		  }
	      }
	    break;
	  }
	case 0:
	  {
	    set<CrossroadState> newSystems1 = GetNextStates1(currentState.crossroadState1);
	    set<CrossroadState> newSystems2 = GetNextStates2(currentState.crossroadState2);
	    newState.remainingTime = 0;
	    newState.finishedSystem = willFinishSystem;
	    for (set<CrossroadState>::iterator it1 = newSystems1.begin(); it1 != newSystems1.end(); it1++ )
	      {
		for (set<CrossroadState>::iterator it2 = newSystems2.begin(); it2 != newSystems2.end(); it2++ )
		  {
		    newState.crossroadState1 = *it1;
		    newState.crossroadState2 = *it2;
#if OUTPUT
		    cout<<"checking: ";newState.Print();
		    cout <<"size = "<<stateSet.size()<<endl;
#endif
		    if (stateSet.end() == stateSet.find(newState))
		      {
#if OUTPUT
			cout<<"inserting: ";newState.Print();
#endif
			stateSet.insert(newState);
			taskQueue.push(newState);
		      }
		  }
	      }
	    break;
	  }
	}
    }
  printf("Hello, world!\n");
  return 0;
}
