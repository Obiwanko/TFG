#include "StateEngine.h"
#include "State.h"
#include "MainMenuState.h"
#include <Utilidades.h> // Biblioteca de Utilidades

MainMenuState MainMenuState::_MainMenuState;
//Identificadores texturas
GLuint textura_fondoMainMenu;
GLuint textura_BotonSeleccionado;
GLuint textura_BotonSinSeleccionar;

GLint Button=0;

	/*
	Sencillo metodod para seguir la logica de la actualizacion del menu.
	*/
	GLint actualizarButton(GLint button) {
		if (button<0) {
			button = 3;
		}
		else if(button==4){
			button = 0;
		}
		return button;
	}

	void onSpecialKeyMainMenu(int specialKey, int x, int y) {
		switch (specialKey) {
		case GLUT_KEY_LEFT:
			break;
		case GLUT_KEY_RIGHT:
			break;
		case GLUT_KEY_UP:
			Button = actualizarButton(Button - 1);
			break;
		case GLUT_KEY_DOWN:
			Button = actualizarButton(Button + 1);
			break;
		}

	}

	void init_de_TexturaMainMenu(GLuint &id, char* nombre)
	{
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		loadImageFile(nombre);
	}

	void onKeyMainMenu(unsigned char tecla, int x, int y)
		// Funcion de atencion al teclado
	{
		//float xrotrad, yrotrad;
		switch (tecla) {

		case 13://se pulsa enter

			switch (Button)
			{

			case 0: //entramos al creador
				break;
			case 1: //entramos a seleccionar mapa
				break;
			case 2: // entramos a las opciones
				break;
			case 3: //salimos
				exit(0);
				break;
			default:
				break;
			}

			break;
		case 8: //se pulsa retroceso.

			break;
		case 27: // Pulso escape
			exit(0);
		}
	}


	void MainMenuState::Init() {
		init_de_TexturaMainMenu(textura_BotonSeleccionado, "./textures/MainMenu/ButtonSelected.jpg");
		init_de_TexturaMainMenu(textura_fondoMainMenu, "./textures/MainMenu/Background.jpg");
		init_de_TexturaMainMenu(textura_BotonSinSeleccionar, "./textures/MainMenu/ButtonNotSelected.jpg");
		glutSpecialFunc(onSpecialKeyMainMenu);// Alta de la funcion de atencion al teclado especial
		glutKeyboardFunc(onKeyMainMenu);// Alta de la funcion de atencion al teclado 
	}

	//Limpiar texturas etc
	void MainMenuState::Cleanup() {

	}


	void MainMenuState::Resume() {

	}

	void MainMenuState::Pause() {

	}


	void MainMenuState::HandleEvents() {

	}


	void MainMenuState::Update() {

	}

	void background() {

								 //Uso de las texturas
		glBindTexture(GL_TEXTURE_2D, textura_fondoMainMenu);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		GLfloat v0[3] = { -1.0,-1.0,0.0 };
		GLfloat v1[3] = { 1.0,-1.0,0.0 };
		GLfloat v3[3] = { -1.0,1.0,0.0 };
		GLfloat v2[3] = { 1.0,1.0,0.0 };

		quadtex((GLfloat*)v0, (GLfloat*)v1, (GLfloat*)v2, (GLfloat*)v3,
			0, 1, 0, 1, 1, 1);




	}
	
	



	void botones() {

		if (Button == 0) {
			glBindTexture(GL_TEXTURE_2D, textura_BotonSeleccionado);
		}
		else {
			glBindTexture(GL_TEXTURE_2D, textura_BotonSinSeleccionar);
		}
	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		GLfloat v0[3] = { -0.6,0.2,0.0 };
		GLfloat v1[3] = { 0.6,0.2,0.0 };
		GLfloat v3[3] = { -0.6,0.0,0.0 };
		GLfloat v2[3] = { 0.6,0.0,0.0 };

		quadtex((GLfloat*)v0, (GLfloat*)v1, (GLfloat*)v2, (GLfloat*)v3,
			0, 1, 0, 1, 1, 1);

		if (Button == 1) {
			glBindTexture(GL_TEXTURE_2D, textura_BotonSeleccionado);
		}
		else {
			glBindTexture(GL_TEXTURE_2D, textura_BotonSinSeleccionar);
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		v0[1] = -0.1;
		v1[1] = -0.1;
		v2[1] = -0.3;
		v3[1] = -0.3;

		quadtex((GLfloat*)v0, (GLfloat*)v1, (GLfloat*)v2, (GLfloat*)v3,
			0, 1, 0, 1, 1, 1);


		if (Button == 2) {
			glBindTexture(GL_TEXTURE_2D, textura_BotonSeleccionado);
		}
		else {
			glBindTexture(GL_TEXTURE_2D, textura_BotonSinSeleccionar);
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		v0[1] = -0.4;
		v1[1] = -0.4;
		v2[1] = -0.6;
		v3[1] = -0.6;

		quadtex((GLfloat*)v0, (GLfloat*)v1, (GLfloat*)v2, (GLfloat*)v3,
			0, 1, 0, 1, 1, 1);

		if (Button == 3) {
			glBindTexture(GL_TEXTURE_2D, textura_BotonSeleccionado);
		}
		else {
			glBindTexture(GL_TEXTURE_2D, textura_BotonSinSeleccionar);
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		v0[1] = -0.7;
		v1[1] = -0.7;
		v2[1] = -0.9;
		v3[1] = -0.9;

		quadtex((GLfloat*)v0, (GLfloat*)v1, (GLfloat*)v2, (GLfloat*)v3,
			0, 1, 0, 1, 1, 1);


	}

	//TODO completar esta mision
	void MainMenuState::Draw() {

			
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		//Preparamos camara orthografica para poder dibujar el Hud
		glOrtho(-1, 1, -1, 1, -1.0, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glDisable(GL_CULL_FACE);

		glClear(GL_DEPTH_BUFFER_BIT);
		// Habilitamos blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// Z-Buffer
		glDepthMask(GL_FALSE);

		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//Posible problema con la luz
		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D); //habilitamos textura
								 //glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR); //brillos por separado

		background();
		botones();
		
		glPopAttrib();
		// Z-Buffer a estado normal
		glDepthMask(GL_TRUE);

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

	}


