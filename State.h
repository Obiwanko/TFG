#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "StateEngine.h"

class State
{
public:
	virtual void Init() = 0;
	virtual void Cleanup() = 0;

	virtual void Pause() = 0;
	virtual void Resume() = 0;

	virtual void HandleEvents(CGameEngine* game) = 0;
	virtual void Update(CGameEngine* game) = 0;
	virtual void Draw(CGameEngine* game) = 0;

	void ChangeState(CGameEngine* game, CGameState* state) {
		game->ChangeState(state);
	}

protected:
	State() { }
};

class MainMenuState
{
public:
	virtual void Init() = 0;
	virtual void Cleanup() = 0;

	virtual void Pause() = 0;
	virtual void Resume() = 0;

	virtual void HandleEvents(CGameEngine* game) = 0;
	virtual void Update(CGameEngine* game) = 0;
	virtual void Draw(CGameEngine* game) = 0;

	void ChangeState(CGameEngine* game, CGameState* state) {
		game->ChangeState(state);
	}

protected:
	MainMenuState() { }
};

class MenuState
{
public:
	virtual void Init() = 0;
	virtual void Cleanup() = 0;

	virtual void Pause() = 0;
	virtual void Resume() = 0;

	virtual void HandleEvents(CGameEngine* game) = 0;
	virtual void Update(CGameEngine* game) = 0;
	virtual void Draw(CGameEngine* game) = 0;

	void ChangeState(CGameEngine* game, CGameState* state) {
		game->ChangeState(state);
	}

protected:
	MenuState() { }
};

class MapSelectionState
{
public:
	virtual void Init() = 0;
	virtual void Cleanup() = 0;

	virtual void Pause() = 0;
	virtual void Resume() = 0;

	virtual void HandleEvents(CGameEngine* game) = 0;
	virtual void Update(CGameEngine* game) = 0;
	virtual void Draw(CGameEngine* game) = 0;

	void ChangeState(CGameEngine* game, CGameState* state) {
		game->ChangeState(state);
	}

protected:
	MapSelectionState() { }
};

#endif