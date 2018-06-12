
#ifndef MAPSELECTOR_H
#define MAPSELECTOR_H

#include "State.h"


class MapSelectorState : public State
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


	static MapSelectorState* Instance() {
		return &_MapSelectorState;
	};

protected:
	MapSelectorState() { }

private:
	static MapSelectorState _MapSelectorState;
};

#endif