#include <set>
using namespace std;

#define OUTPUT 0

class CrossroadState
{
public:
  int id;
  int time;
  bool operator<(const CrossroadState& s) const;
  bool operator==(const CrossroadState& s) const;
  void Print() const;
};

CrossroadState GetNextState1(CrossroadState s);
CrossroadState GetNextState2(CrossroadState s);
set<CrossroadState> GetNextStates1(CrossroadState s);
set<CrossroadState> GetNextStates2(CrossroadState s);


class globalState
{
public:
  CrossroadState crossroadState1;
  CrossroadState crossroadState2;
  int remainingTime;
  int finishedSystem;
  int id;
  bool operator<(const globalState& s) const;
  //  void Print() const;
  void Print(ostream&) const;
};

void PrintStatesTransitionEdge(ofstream& file, globalState& source, const globalState& target);
