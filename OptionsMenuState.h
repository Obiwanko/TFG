#ifndef OPTIONSMENUSTATE_H
#define OPTIONSMENUSTATE_H

#include "State.h"

//clase del menu de opciones contiene las mismas funciones que el estado base
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

	// implementacion del patron singleton
	static OptionsMenuState* Instance() {
		return &_OptionsMenuState;
	};

protected:
	OptionsMenuState() { }

private:
	static OptionsMenuState _OptionsMenuState;
};

#endif