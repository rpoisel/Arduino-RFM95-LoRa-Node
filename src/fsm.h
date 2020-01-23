#ifndef FSM_H_
#define FSM_H_

namespace LoRaNode
{
class FSM;
class State
{
  public:
  virtual ~State(){};

  virtual void execute(FSM* fsm) = 0;
  virtual void enter() = 0;
};

class NullState : public State
{
  public:
  static NullState* instance()
  {
    static NullState state;
    return &state;
  }
  void execute(FSM* fsm)
  {
  }
  void enter()
  {
  }

  private:
  NullState()
  {
  }
  NullState& operator=(const NullState&) = delete;
  NullState(const NullState&) = delete;
};

class FSM
{
  public:
  FSM() : curState{NullState::instance()}
  {
  }
  void loop()
  {
    curState->execute(this);
  }

  void nextState(State* next)
  {
    curState = next;
    curState->enter();
  }

  private:
  State* curState;
};
} // namespace LoRaNode

#endif /* FSM_H_ */
