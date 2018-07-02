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
	//inicializacion de la ventana de dibujado
	void Init(const char* title, int width = 800, int height = 600, bool fullscreen = false);
	//funciones para el control de la pila de estados
	void Cleanup();
	void ChangeState(State* state);
	void PushState(State* state);
	void PopState();

	//funcion para el manejo de estados
	void HandleEvents();
	//evento de actualizacion
	void Update();
	//dibuja el estado que este en la cima de la pila
	void Draw();
	//puesta en marcha del engine
	bool Running() { return running; }
	//apagado del engine
	void Quit() { running = false; }

private:
	//pila de estados
	vector<State*> states;
	//booleano para saber si este engine esta en funcionamiento
	bool running;
};

#endif
