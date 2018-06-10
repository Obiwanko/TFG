#include "StateEngine.h"
#include "State.h"
#include "MainMenuState.h"

//#include <Utilidades.h> // Biblioteca de Utilidades

	void MainMenuState::Draw() {
	/*
			//HUD
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		//Preparamos camara orthografica para poder dibujar el Hud
		glOrtho(-1, 1, -1, 1, -1.0, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glDisable(GL_CULL_FACE);

		glClear(GL_DEPTH_BUFFER_BIT);

		//Nos disponemos a añadir los elementos al hud, de esta forma, podremos tener elementos añadidos de forma constante delante de la camara y crear una interfaz para el usuario.

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		//rgb(50,205,50)
		glColor4f(0, 0, 1, 0.7);
		glBegin(GL_QUAD_STRIP);
		glTexCoord2f(0, 0);
		glVertex3f(-0.85, 0.95, 1);
		glTexCoord2f(0, 1);
		glVertex3f(-0.85, 0.7, 1);
		glTexCoord2f(1, 0);
		glVertex3f(0.85, 0.95, 1);
		glTexCoord2f(1, 1);
		glVertex3f(0.85, 0.7, 1);
		glEnd();
		glPopAttrib();


		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	*/


	}

