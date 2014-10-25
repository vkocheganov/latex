#include <stdio.h>
#include <iostream>
#include <fstream>
#include <set>
#include <stack>
using namespace std;

#include "States.h"


CrossroadState crossroadStates1[] = {{0, 30}, {1, 30}};
CrossroadState crossroadStates2[] = {{0, 30}, {1, 30}, {2, 15}};

bool CrossroadState::operator<(const CrossroadState& s) const
{
#if OUTPUT
  cout <<"comparing:";
  this->Print();
  s.Print();
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

void CrossroadState::Print() const
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
  this->Print();
  s.Print();
#endif

  bool less = (crossroadState1 < s.crossroadState1) ||
    (crossroadState1 == s.crossroadState1 && crossroadState2 < s.crossroadState2) ||
    (crossroadState1 == s.crossroadState1 && crossroadState2 == s.crossroadState2 && remainingTime < s.remainingTime) ||
    (crossroadState1 == s.crossroadState1 && crossroadState2 == s.crossroadState2 && remainingTime == s.remainingTime && finishedSystem < s.finishedSystem);

#if OUTPUT
  cout<<"less = "<< less<< endl;
#endif

  return less;
}


// void globalState::Print()const
// {
  
//   cout<< id<<" ("<<crossroadState1.id<<","<<crossroadState1.time<<"),"<<
//     "("<<crossroadState2.id<<","<<crossroadState2.time<<"),"<<remainingTime<<","
//       <<finishedSystem<<endl;
// }

void globalState::Print(ostream& f)const
{
  
//   f<< id<<",|"<<crossroadState1.id<<"_"<<crossroadState1.time<<"|"
// <<crossroadState2.id<<"_"<<crossroadState2.time<<"|"<<remainingTime<<"_"
//       <<finishedSystem<<endl;
  f<< id<<","<<id<<endl;
}

#include <sstream>
void PrintStatesTransitionEdge(ofstream& file, globalState& source, globalState& target)
{
  file << source.id<<","<<target.id << ",Directed"<<endl;
}
