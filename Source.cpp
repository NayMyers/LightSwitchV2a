#include <vector>
#include <iostream>

using namespace std;

class StateContext;

enum state {ON, OFF};

class State
{
protected:
	StateContext* CurrentContext;
public:
	State(StateContext* Context) { CurrentContext = Context; }
	virtual ~State(void){}
};

class StateContext
{
protected:
	State* CurrentState = nullptr;
	int stateIndex = 0;
	vector<State*> availableStates;
public:
	virtual ~StateContext(void);
	virtual void setState(state newState);
	virtual int getStateIndex(void);
};
StateContext::~StateContext(void)
{
	for (int count = 0; count < this->availableStates.size(); count++) delete this->availableStates[count]; // code that deletes all pointers to all of the prevoiusly saved states 
}
void StateContext::setState(state newState)
{
	this->CurrentState = availableStates[newState];
	this->stateIndex = newState;
}
int StateContext::getStateIndex(void)
{
	return this->stateIndex;
}

class Transition
{
public:
	virtual void pressButton(void) { cout << "Error!" << endl; }
};

class LightSwitchState : public State, public Transition
{
public:
	LightSwitchState(StateContext* CurrentContext) :State(CurrentContext) {}
};

class LightSwitchOn : public LightSwitchState
{
public:
	LightSwitchOn(StateContext* CurrentContext) :LightSwitchState(CurrentContext) {}
	void pressButton(void);
};
void LightSwitchOn::pressButton(void)
{
	cout << "Light Going Off" << endl;
	this->CurrentContext->setState(OFF);
}

class LightSwitchOff :public LightSwitchState
{
	LightSwitchOff(StateContext* CurrentContext) :LightSwitchState(CurrentContext) {}
	void pressButton(void);
};
void LightSwitchOff::pressButton(void)
{
	cout << "Light Going On" << endl;
	this->CurrentContext->setState(ON);
}

class LightSwitch : public StateContext , public Transition
{
protected:
	LightSwitchState* currentLightSwitchState = nullptr;
public:
	LightSwitch(void);
	void pressButton(void);
};
LightSwitch::LightSwitch(void)
{
	this->availableStates.push_back(new LightSwitchOn(this));
	this->availableStates.push_back(new LightSwitchOff(this));
	this->setState(OFF);
}
void LightSwitch::pressButton(void)
{
	currentLightSwitchState = (LightSwitchState*)CurrentState;
	currentLightSwitchState->pressButton();
}

int main(void)
{
	LightSwitch MyLightSwitch;
	for (int count = 0; count < 5; count++) MyLightSwitch.pressButton();

	return 0;
}
