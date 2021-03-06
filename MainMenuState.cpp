#include "StateEngine.h"
#include "State.h"
#include "MainMenuState.h"
#include "CreationModeState.h"
#include "OptionsMenuState.h"
#include "MapSelectorState.h"
#include "Utilidades.h" // Biblioteca de Utilidades
#include "Globals.h"
#include <ctime>
#include <string>

MainMenuState MainMenuState::_MainMenuState;
//Identificadores texturas
GLuint textura_fondoMainMenu;
GLuint textura_BotonSeleccionado;
GLuint textura_BotonSinSeleccionar;
GLuint textura_Titulo;
StateEngine* engineMenu;
GLint Button=0;
string fileMap ="";

	/*
	Sencillo metodo para seguir la logica de la actualizacion del menu.
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


	void onKeyMainMenu(unsigned char tecla, int x, int y)
		// Funcion de atencion al teclado
	{
		std::time_t t = std::time(0);
		std::tm* now = std::localtime(&t);
		int dia = (now->tm_mday);
		int mes = (now->tm_mon + 1);
		int a�o = (now->tm_year + 1900);
		int seg = (now->tm_sec);
		int min = (now->tm_min);
		int hora = (now->tm_hour);
		//float xrotrad, yrotrad;
		switch (tecla) {
		case 13://se pulsa enter
			switch (Button)
			{
			case 0: //entramos al creador
				fileMap = saveFolder+"Circuito_" +to_string(hora)+ to_string(min) + to_string(seg)+ to_string(dia) + to_string(mes) + to_string(a�o) + ".txt";
				engineMenu->ChangeState(CreationModeState::Instance());
				break;
			case 1: //entramos a seleccionar mapa
					engineMenu->PushState(MapSelectorState::Instance());
				break;
			case 2: // entramos a las opciones
					engineMenu->PushState(OptionsMenuState::Instance());
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


	void MainMenuState::Init(StateEngine* engine) {
		inicializarTextura(textura_BotonSeleccionado, "./textures/MainMenu/ButtonSelected.jpg");
		inicializarTextura(textura_fondoMainMenu, "./textures/MainMenu/Background.jpg");
		inicializarTextura(textura_BotonSinSeleccionar, "./textures/MainMenu/ButtonNotSelected.jpg");
		inicializarTextura(textura_Titulo, "./textures/MainMenu/titulo.png");
		engineMenu = engine;
		glutSpecialFunc(onSpecialKeyMainMenu);// Alta de la funcion de atencion al teclado especial
		glutKeyboardFunc(onKeyMainMenu);// Alta de la funcion de atencion al teclado 
	}

	//Limpiar texturas etc
	void MainMenuState::Cleanup() {
		glutSpecialFunc(NULL);
		glutKeyboardFunc(NULL);
	}


	void MainMenuState::Resume() {
		//se vuelven a dar de alta las funciones de escucha

		glutSpecialFunc(onSpecialKeyMainMenu);// Alta de la funcion de atencion al teclado especial
		glutKeyboardFunc(onKeyMainMenu);// Alta de la funcion de atencion al teclado 
	}

	void MainMenuState::Pause() {

	}


	void MainMenuState::HandleEvents(StateEngine* game) {

	}


	void MainMenuState::Update(StateEngine* game) {

	}

	void background() {

								 //Uso de las texturas
		glBindTexture(GL_TEXTURE_2D, textura_fondoMainMenu);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		Point3D v0(-1.0, -1.0, 0.0);
		Point3D v1(1.0, -1.0, 0.0);
		Point3D v3(-1.0, 1.0, 0.0);
		Point3D v2(1.0, 1.0, 0.0);

		quadtex(v0, v1, v2, v3,
			0, 1, 0, 1, 1, 1);

	}
	

	void titulo() {

		//Uso de las texturas
		glBindTexture(GL_TEXTURE_2D, textura_Titulo);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		Point3D v0( -0.35,0.2,0.0 );
		Point3D v1( 0.35,0.2,0.0 );
		Point3D v3(-0.35,0.9,0.0);
		Point3D v2(0.35,0.9,0.0 );

		quadtex(v0, v1, v2, v3,
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

		Point3D v0( -0.6,0.2,0.0 );
		Point3D v1( 0.6,0.2,0.0 );
		Point3D v3( -0.6,0.0,0.0 );
		Point3D v2( 0.6,0.0,0.0);
		
		
		quadtex(v0, v1, v2, v3,
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

		quadtex(v0, v1, v2, v3,
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

		quadtex(v0, v1, v2, v3,
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

		quadtex(v0, v1, v2, v3,
			0, 1, 0, 1, 1, 1);


	}


	void textosBotones() {
		if(Button==0)
		textoStroke(-0.27, 0.07, 0.2, "Nuevo Circuito", 0.08, 0.08, 0.08, AMARILLO, GLUT_STROKE_ROMAN);
		else
		textoStroke(-0.27, 0.07, 0.2, "Nuevo Circuito", 0.08, 0.08, 0.08, BLANCO, GLUT_STROKE_ROMAN);

		if (Button == 1)
			textoStroke(-0.27, -0.23, 0.2, "Cargar Circuito", 0.08, 0.08, 0.08, AMARILLO, GLUT_STROKE_ROMAN);
		else
			textoStroke(-0.27, -0.23, 0.2, "Cargar Circuito", 0.08, 0.08, 0.08, BLANCO, GLUT_STROKE_ROMAN);

		if (Button == 2)
			textoStroke(-0.18, -0.53, 0.2, "Opciones", 0.08, 0.08, 0.08, AMARILLO, GLUT_STROKE_ROMAN);
		else
			textoStroke(-0.18, -0.53, 0.2, "Opciones", 0.08, 0.08, 0.08, BLANCO, GLUT_STROKE_ROMAN);

		if (Button == 3)
			textoStroke(-0.1, -0.83, 0.2, "Salir", 0.08, 0.08, 0.08, AMARILLO, GLUT_STROKE_ROMAN);
		else
			textoStroke(-0.1, -0.83, 0.2, "Salir", 0.08, 0.08, 0.08, BLANCO, GLUT_STROKE_ROMAN);

	}

	//TODO completar esta mision
	void MainMenuState::Draw(StateEngine* game) {

		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPushMatrix();
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

		background();
		botones();
		titulo();
		textosBotones();

		glPopAttrib();
		// Z-Buffer a estado normal
		glDepthMask(GL_TRUE);

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		glPopMatrix();
		glPopAttrib();
	}


