
#ifndef STATE_H
#define STATE_H

#include "StateEngine.h"

class State
{
public:
	virtual void Init(StateEngine* engine) = 0;
	virtual void Cleanup() = 0;

	virtual void Pause() = 0;
	virtual void Resume() = 0;

	virtual void HandleEvents(StateEngine* engine) = 0;
	virtual void Update(StateEngine* engine) = 0;
	virtual void Draw(StateEngine* engine) = 0;

	void ChangeState(StateEngine* engine, State* state) {
		engine->ChangeState(state);
	}

protected:
	State() { }
};

#endif

