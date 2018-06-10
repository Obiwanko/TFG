#ifndef ENGINESTATE_H
#define ENGINESTATE_H
#include <vector>
#include <GL/freeglut.h>
#include <GL/glext.h>
#include <freeimage/FreeImage.h>

using namespace std;

class State;

class StateEngine
{
public:

	void Init(const char* title, int width = 800, int height = 600, bool fullscreen = false);
	void Cleanup();
	void ChangeState(State* state);
	void PushState(State* state);
	void PopState();

	void HandleEvents();
	void Update();
	void Draw();

	bool Running() { return running; }
	void Quit() { running = false; }

private:
	// the stack of states
	vector<State*> states;

	bool running;
};

#endif
