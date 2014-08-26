#include <stdio.h>
#include <iostream>
#include <set>
#include <stack>
using namespace std;

#define OUTPUT 0

class State
{
public:
  int name;
  int time;
  bool operator<(const State& s) const
  {
#if OUTPUT
    cout <<"comparing:";
    this->Out();
    s.Out();
#endif
    bool less = (name < s.name) ||
      (name == s.name && time < s.time);
#if OUTPUT
    cout <<"less= "<<less<<endl;
#endif
    return less;
  }
  bool operator==(const State& s) const
  {return name == s.name && time == s.time;}
  void Out() const
  {
    cout << "("<<name<<","<<time<<")"<<endl;
}
};

State system1[] = {{0, 3}, {1, 4}};
State system2[] = {{0, 4}, {1, 3}, {2, 2}};

State f1(State s)
{
  return s.name < 1 ? system1[1] : system1[0];
}

State f2(State s)
{
  return s.name < 1 ? system2[1] : system2[0];
}

set<State> NextStates1(State s)
{
  set<State> temp;
  temp.insert(s.name < 1 ? system1[1] : system1[0]);
  return temp;
}

set<State> NextStates2(State s)
{
  set<State> temp;
  switch(s.name)
    {
    case 0: 
      temp.insert(system2[1]);
      break;
    case 1: 
      temp.insert(system2[0]);
      temp.insert(system2[2]);
      break;
    case 2: 
      temp.insert(system2[0]);
      temp.insert(system2[2]);
      break;

}
  return temp;
}

class globalState
{
public:
  State system1;
  State system2;
  int remainingTime;
  int finishedSystem;
  bool operator<(const globalState& s) const
  {
#if OUTPUT
    cout <<"comparing:";
    this->StateOut();
    s.StateOut();
#endif
    bool less = (system1 < s.system1) ||
      (system1 == s.system1 && system2 < s.system2) ||
      (system1 == s.system1 && system2 == s.system2 && remainingTime < s.remainingTime) ||
      (system1 == s.system1 && system2 == s.system2 && remainingTime == s.remainingTime && finishedSystem < s.finishedSystem);
#if OUTPUT
    cout<<"less = "<< less<< endl;
#endif
    return less;
  }
  void StateOut()const
  {
    cout<<"("<<system1.name<<","<<system1.time<<"),"<<
      "("<<system2.name<<","<<system2.time<<"),"<<remainingTime<<","
	<<finishedSystem<<endl;
  }
};

bool compareFunc(State& s1, State& s2)
{
  return false;
}
int main()
{
  set<globalState> stateSet;
  stack<globalState> taskQueue;

  globalState startState;
  startState.system1=system1[0];
  startState.system2=system2[0];
  startState.finishedSystem = 2;
  startState.remainingTime = startState.system1.time;

  taskQueue.push(startState);
  stateSet.insert(startState);

  while (!taskQueue.empty())
    {
      globalState currentState = taskQueue.top();
      currentState.StateOut();
      taskQueue.pop();
      bool firstFinished = currentState.finishedSystem == 0 || currentState.finishedSystem == 1;
      bool secondFinished = currentState.finishedSystem == 0 || currentState.finishedSystem == 2;
      int firstTime = firstFinished ? currentState.system1.time : currentState.remainingTime;
      int secondTime = secondFinished ? currentState.system2.time : currentState.remainingTime;
      int willFinishSystem = 
	firstTime < secondTime ? 1 :
	(firstTime > secondTime ? 2 : 0);
      int timeToSpend = currentState.remainingTime ? currentState.remainingTime : (willFinishSystem == 1 ? firstTime : secondTime);
      globalState newState;
      switch (willFinishSystem)
	{
	case 1: 
	  {
	    set<State> newSystems1 = NextStates1(currentState.system1);
	    set<State> newSystems2;
	    newSystems2.insert(currentState.system2);
	    newState.remainingTime = currentState.system2.time - timeToSpend;
	    newState.finishedSystem = willFinishSystem;
	    for (set<State>::iterator it1 = newSystems1.begin(); it1 != newSystems1.end(); it1++ )
	      {
		for (set<State>::iterator it2 = newSystems2.begin(); it2 != newSystems2.end(); it2++ )
		  {
		    newState.system1 = *it1;
		    newState.system2 = *it2;
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
	    set<State> newSystems1;
	    set<State> newSystems2 = NextStates2(currentState.system2);
	    newSystems1.insert(currentState.system1);
	    newState.remainingTime = currentState.system1.time - timeToSpend;
	    newState.finishedSystem = willFinishSystem;
	    for (set<State>::iterator it1 = newSystems1.begin(); it1 != newSystems1.end(); it1++ )
	      {
		for (set<State>::iterator it2 = newSystems2.begin(); it2 != newSystems2.end(); it2++ )
		  {
		    newState.system1 = *it1;
		    newState.system2 = *it2;
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
	    set<State> newSystems1 = NextStates1(currentState.system1);
	    set<State> newSystems2 = NextStates2(currentState.system2);
	    newState.remainingTime = 0;
	    newState.finishedSystem = willFinishSystem;
	    for (set<State>::iterator it1 = newSystems1.begin(); it1 != newSystems1.end(); it1++ )
	      {
		for (set<State>::iterator it2 = newSystems2.begin(); it2 != newSystems2.end(); it2++ )
		  {
		    newState.system1 = *it1;
		    newState.system2 = *it2;
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
