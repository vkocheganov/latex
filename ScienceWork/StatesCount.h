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
  void Out() const;
};

CrossroadState GetNextState1(CrossroadState s);
CrossroadState GetNextState2(CrossroadState s);
set<CrossroadState> GetNextStates1(CrossroadState s);
set<CrossroadState> GetNextStates2(CrossroadState s);


class globalState
{
public:
  CrossroadState crossroad1;
  CrossroadState crossroad2;
  int remainingTime;
  int finishedSystem;
  bool operator<(const globalState& s) const;
  void StateOut() const;
};


