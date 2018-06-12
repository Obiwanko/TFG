#include "StateEngine.h"
#include "State.h"


void StateEngine::Init(const char* title, int width, int heigth,  bool fullscreen) {
	
		glutInitWindowSize(width, heigth); // Tamanyo inicial de la ventana
	glutCreateWindow(title);	// Pone el titulo para que se vaya actualizando luego en onkey
	if (fullscreen) {
		glutFullScreen();
	}
	running = true;
	
	

}

void StateEngine::Cleanup()
{
	// cleanup the all states
	while (!states.empty()) {
		states.back()->Cleanup();
		states.pop_back();
	}
	exit(0);
}

void StateEngine::ChangeState(State* state)
{
	// cleanup the current state
	if (!states.empty()) {
		states.back()->Cleanup();
		states.pop_back();
	}
	// store and init the new state
	states.push_back(state);
	states.back()->Init(this);
}

void StateEngine::PushState(State* state)
{
	// pause current state
	if (!states.empty()) {
		states.back()->Pause();
	}

	// store and init the new state
	states.push_back(state);
	states.back()->Init(this);
}

void StateEngine::PopState()
{
	// cleanup the current state
	if (!states.empty()) {
		states.back()->Cleanup();
		states.pop_back();
	}

	// resume previous state
	if (!states.empty()) {
		states.back()->Resume();
	}

}

void StateEngine::HandleEvents()
{
	if (!states.empty())
	states.back()->HandleEvents(this);
}

void StateEngine::Update()
{
	if (!states.empty())
	states.back()->Update(this);
}

void StateEngine::Draw() {
	if(!states.empty())
	states.back()->Draw(this);
}