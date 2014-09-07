#include <stdio.h>
#include <iostream>
#include <set>
#include <stack>
using namespace std;

#include "StatesCount.h"

CrossroadState crossroadStates1[] = {{0, 3}, {1, 4}};
CrossroadState crossroadStates2[] = {{0, 4}, {1, 3}, {2, 2}};

bool CrossroadState::operator<(const CrossroadState& s) const
{

#if OUTPUT
  cout <<"comparing:";
  this->Out();
  s.Out();
#endif

  bool less = (id < s.id) ||
    (id == s.id && time < s.time);

#if OUTPUT
  cout <<"less= "<<less<<endl;
#endif

  return less;
}
bool CrossroadState::operator==(const CrossroadState& s) const
{
  return id == s.id && time == s.time;
}

void CrossroadState::Out() const
{
  cout << "("<<id<<","<<time<<")"<<endl;
}

CrossroadState GetNextState1(CrossroadState s)
{
  return s.id == 0 ? crossroadStates1[1] : crossroadStates1[0];
}

CrossroadState GetNextState2(CrossroadState s)
{
  return s.id == 0 ? crossroadStates2[1] : crossroadStates2[0];
}

set<CrossroadState> GetNextStates1(CrossroadState s)
{
  set<CrossroadState> temp;
  temp.insert(s.id < 1 ? crossroadStates1[1] : crossroadStates1[0]);
  return temp;
}

set<CrossroadState> GetNextStates2(CrossroadState s)
{
  set<CrossroadState> tempSet;
  switch(s.id)
    {
    case 0: 
      tempSet.insert(crossroadStates2[1]);
      break;
    case 1: 
      tempSet.insert(crossroadStates2[0]);
      tempSet.insert(crossroadStates2[2]);
      break;
    case 2: 
      tempSet.insert(crossroadStates2[0]);
      tempSet.insert(crossroadStates2[2]);
      break;

    }
  return tempSet;
}

bool globalState::operator<(const globalState& s) const
{

#if OUTPUT
  cout <<"comparing:";
  this->StateOut();
  s.StateOut();
#endif

  bool less = (crossroad1 < s.crossroad1) ||
    (crossroad1 == s.crossroad1 && crossroad2 < s.crossroad2) ||
    (crossroad1 == s.crossroad1 && crossroad2 == s.crossroad2 && remainingTime < s.remainingTime) ||
    (crossroad1 == s.crossroad1 && crossroad2 == s.crossroad2 && remainingTime == s.remainingTime && finishedSystem < s.finishedSystem);

#if OUTPUT
  cout<<"less = "<< less<< endl;
#endif

  return less;
}

void globalState::StateOut()const
{
  cout<<"("<<crossroad1.id<<","<<crossroad1.time<<"),"<<
    "("<<crossroad2.id<<","<<crossroad2.time<<"),"<<remainingTime<<","
      <<finishedSystem<<endl;
}

int main()
{
  set<globalState> stateSet;
  stack<globalState> taskQueue;

  globalState startState;
  startState.crossroad1=crossroadStates1[0];
  startState.crossroad2=crossroadStates2[0];
  startState.finishedSystem = 2;
  startState.remainingTime = startState.crossroad1.time;

  taskQueue.push(startState);
  stateSet.insert(startState);

  while (!taskQueue.empty())
    {
      globalState currentState = taskQueue.top();
      currentState.StateOut();
      taskQueue.pop();
      bool firstFinished = currentState.finishedSystem == 0 || currentState.finishedSystem == 1;
      bool secondFinished = currentState.finishedSystem == 0 || currentState.finishedSystem == 2;
      int firstTime = firstFinished ? currentState.crossroad1.time : currentState.remainingTime;
      int secondTime = secondFinished ? currentState.crossroad2.time : currentState.remainingTime;
      int willFinishSystem = 
	firstTime < secondTime ? 1 :
	(firstTime > secondTime ? 2 : 0);
      int timeToSpend = currentState.remainingTime ? currentState.remainingTime : (willFinishSystem == 1 ? firstTime : secondTime);
      globalState newState;
      switch (willFinishSystem)
	{
	case 1: 
	  {
	    set<CrossroadState> newSystems1 = GetNextStates1(currentState.crossroad1);
	    set<CrossroadState> newSystems2;
	    newSystems2.insert(currentState.crossroad2);
	    newState.remainingTime = currentState.crossroad2.time - timeToSpend;
	    newState.finishedSystem = willFinishSystem;
	    for (set<CrossroadState>::iterator it1 = newSystems1.begin(); it1 != newSystems1.end(); it1++ )
	      {
		for (set<CrossroadState>::iterator it2 = newSystems2.begin(); it2 != newSystems2.end(); it2++ )
		  {
		    newState.crossroad1 = *it1;
		    newState.crossroad2 = *it2;
		    if (stateSet.end() == stateSet.find(newState))
		      {
#if OUTPUT
			cout<<"inserting: ";newState.StateOut();
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
	    set<CrossroadState> newSystems2 = GetNextStates2(currentState.crossroad2);
	    newSystems1.insert(currentState.crossroad1);
	    newState.remainingTime = currentState.crossroad1.time - timeToSpend;
	    newState.finishedSystem = willFinishSystem;
	    for (set<CrossroadState>::iterator it1 = newSystems1.begin(); it1 != newSystems1.end(); it1++ )
	      {
		for (set<CrossroadState>::iterator it2 = newSystems2.begin(); it2 != newSystems2.end(); it2++ )
		  {
		    newState.crossroad1 = *it1;
		    newState.crossroad2 = *it2;
		    if (stateSet.end() == stateSet.find(newState))
		      {
#if OUTPUT
			cout<<"inserting: ";newState.StateOut();
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
	    set<CrossroadState> newSystems1 = GetNextStates1(currentState.crossroad1);
	    set<CrossroadState> newSystems2 = GetNextStates2(currentState.crossroad2);
	    newState.remainingTime = 0;
	    newState.finishedSystem = willFinishSystem;
	    for (set<CrossroadState>::iterator it1 = newSystems1.begin(); it1 != newSystems1.end(); it1++ )
	      {
		for (set<CrossroadState>::iterator it2 = newSystems2.begin(); it2 != newSystems2.end(); it2++ )
		  {
		    newState.crossroad1 = *it1;
		    newState.crossroad2 = *it2;
#if OUTPUT
		    cout<<"checking: ";newState.StateOut();
		    cout <<"size = "<<stateSet.size()<<endl;
#endif
		    if (stateSet.end() == stateSet.find(newState))
		      {
#if OUTPUT
			cout<<"inserting: ";newState.StateOut();
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
