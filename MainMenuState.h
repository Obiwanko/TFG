
#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

#include "State.h"


class MainMenuState : public State
{
public:
	void Init();
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents();
	void Update();
	void Draw();
	
	void ChangeState(StateEngine* game, State* state) {
		game->ChangeState(state);
	}


	static MainMenuState* Instance() {
		return &_MainMenuState;
	};

protected:
	MainMenuState() { }

private:
	static MainMenuState _MainMenuState;
};

#endif