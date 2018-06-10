
#ifndef STATE_H
#define STATE_H

#include "StateEngine.h"

class State
{
public:
	virtual void Init() = 0;
	virtual void Cleanup() = 0;

	virtual void Pause() = 0;
	virtual void Resume() = 0;

	virtual void HandleEvents() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	void ChangeState(StateEngine* game, State* state) {
		game->ChangeState(state);
	}

protected:
	State() { }
};

#endif

