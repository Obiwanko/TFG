#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <vector>
using namespace std;

class CGameState;

class CGameEngine
{
public:

	void Init(const char* title, int width = 640, int height = 480,
		int bpp = 0, bool fullscreen = false);
	void Cleanup();

	void ChangeState(CGameState* state);
	void PushState(CGameState* state);
	void PopState();

	void HandleEvents();
	void Update();
	void Draw();

	bool Running() { return m_running; }
	void Quit() { m_running = false; }

	// screen;

private:
	// the stack of states
	vector<CGameState*> states;

	bool m_running;
	bool m_fullscreen;
};

#endif
