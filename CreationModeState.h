#ifndef CREATIONMODESTATE_H
#define CREATIONMODESTATE_H

#include "State.h"


class CreationModeState : public State
{
public:
	void Init(StateEngine* game);
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents(StateEngine* game);
	void Update(StateEngine* game);
	void Draw(StateEngine* game);

	void ChangeState(StateEngine* game, State* state) {
		game->ChangeState(state);
	}


	static CreationModeState* Instance() {
		return &_CreationModeState;
	};

protected:
	CreationModeState() { }

private:
	static CreationModeState _CreationModeState;
};

#endif