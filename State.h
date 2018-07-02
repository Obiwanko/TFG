/*
Clase generica de estado dependencia inyectada por metodo de la clase StateEngine
*/

#ifndef STATE_H
#define STATE_H

#include "StateEngine.h"

class State
{
public:
	//inicializa el estado
	virtual void Init(StateEngine* engine) = 0;
	//funciones para controlar el estado actual
	virtual void Cleanup() = 0;
	virtual void Pause() = 0;
	virtual void Resume() = 0;
	//funcion para controlar eventos
	virtual void HandleEvents(StateEngine* engine) = 0;
	//funcion de actualizacion
	virtual void Update(StateEngine* engine) = 0;
	//funcion de dibujado propia del estado
	virtual void Draw(StateEngine* engine) = 0;

	//funcion para cambiar a un estado diferente
	void ChangeState(StateEngine* engine, State* state) {
		engine->ChangeState(state);
	}

protected:
	State() { }
};

#endif

