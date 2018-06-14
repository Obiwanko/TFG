#ifndef OPTIONSMENUSTATE_H
#define OPTIONSMENUSTATE_H

#include "State.h"


class OptionsMenuState : public State
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


	static OptionsMenuState* Instance() {
		return &_OptionsMenuState;
	};

protected:
	OptionsMenuState() { }

private:
	static OptionsMenuState _OptionsMenuState;
};

#endif