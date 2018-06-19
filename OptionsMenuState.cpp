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
#include <iostream> // Biblioteca de entrada salida
#include <fstream>
#include <sstream> // Biblioteca de manejo de strings

OptionsMenuState OptionsMenuState::_OptionsMenuState;
//Identificadores texturas
GLuint textura_fondoOpciones;
GLuint textura_botonSeleccionadoResolucion;
GLuint textura_botonSinSeleccionarResolucion;
GLuint textura_noCheck;
GLuint textura_Check;
GLuint textura_noCheckSelected;
GLuint textura_CheckSelected;
GLuint textura_flechita;
StateEngine* engineOptions;
GLint HorizontalButton = 0;
GLint VerticalButton = 0;

string fileoptions = saveFolder + "options/options.txt";
/*
Sencillo metodod para seguir la logica de la actualizacion del menu.
*/
GLint actualizarVerticalButton(GLint button) {
	if (button<0) {
		button = 2;

	}
	else if (button == 3) {
		button = 0;
	}
	HorizontalButton = 0;
	return button;
}

GLint actualizarHorizontalButton(GLint button) {
	if (VerticalButton == 0) {//estamos en la altura de las 3 opciones de graficos
		if (button<0) {
			button = 2;

		}
		else if (button == 3) {
			button = 0;
		}
	}
	else if (VerticalButton == 1) { //estamos en la seleccion de fullscreen o no
		if (button<0) {
			button = 1;

		}
		else if (button == 2) {
			button = 0;
		}
	}
	else {
		//estamos en la altura de salir del menu
		button = 0;
	}
	return button;
}

void onSpecialKeyOptionsMenu(int specialKey, int x, int y) {
	switch (specialKey) {
	case GLUT_KEY_LEFT:
		HorizontalButton = actualizarHorizontalButton(HorizontalButton - 1);
		break;
	case GLUT_KEY_RIGHT:
		HorizontalButton = actualizarHorizontalButton(HorizontalButton + 1);
		break;
	case GLUT_KEY_UP:
		VerticalButton = actualizarVerticalButton(VerticalButton - 1);
		break;
	case GLUT_KEY_DOWN:
		VerticalButton = actualizarVerticalButton(VerticalButton + 1);
		break;
	}

}


void guardarOpciones() {
	std::ofstream file;
	file.open(fileoptions);
	int aux = fullscreen ? 1 : 0;
	file << graficos << ';' << '\n' << aux << ";";
	file.close();
}

void onKeyOptionsMenu(unsigned char tecla, int x, int y)
// Funcion de atencion al teclado
{
	//float xrotrad, yrotrad;
	switch (tecla) {
	case 13://se pulsa enter
		switch (VerticalButton)
		{
		case 0: //set up de los graficos.
			if (HorizontalButton == 0) {
				resolucion = 10;
				graficos = 1;
			}
			else if (HorizontalButton == 1) {
				resolucion = 20;
				graficos = 2;
			}
			else {
				resolucion = 30;
				graficos = 3;
			}
			//repeticionTex
			//resolucion
			break;
		case 1: // activamos o desactivamos la pantalla completa
			//fullscreen
			if (HorizontalButton == 0) {
		
				if (fullscreen == true) {
					glutReshapeWindow(800, 499);
					glutPositionWindow(0, 0);
					fullscreen = false;
				}
				
			}
			else {
				if (fullscreen == false) {
					glutFullScreen();
					fullscreen = true;
				}

			}
			break;
		case 2: // salimos de opciones y volvemos al estado anterior

			guardarOpciones();
			engineOptions->PopState();
			break;
		default:
			break;
		}
		break;
	case 8: //se pulsa retroceso.
		break;
	case 27: // Pulso escape
		engineOptions->PopState();
	}
}


void OptionsMenuState::Init(StateEngine* engine) {
	inicializarTextura(textura_botonSeleccionadoResolucion, "./textures/Options/ButtonSelected.png");
	inicializarTextura(textura_botonSinSeleccionarResolucion, "./textures/Options/Button.png");
	inicializarTextura(textura_noCheck, "./textures/Options/noCheck.png");
	inicializarTextura(textura_Check, "./textures/Options/Check.png");
	inicializarTextura(textura_noCheckSelected, "./textures/Options/noCheckSelected.png");
	inicializarTextura(textura_CheckSelected, "./textures/Options/CheckSelected.png");
	inicializarTextura(textura_fondoOpciones, "./textures/Options/Background.jpg");
	inicializarTextura(textura_flechita, "./textures/Options/Flecha.png");
	engineOptions = engine;
	glutSpecialFunc(onSpecialKeyOptionsMenu);// Alta de la funcion de atencion al teclado especial
	glutKeyboardFunc(onKeyOptionsMenu);// Alta de la funcion de atencion al teclado 
}

//Limpiar texturas etc
void OptionsMenuState::Cleanup() {
	glutSpecialFunc(NULL);
	glutKeyboardFunc(NULL);
}


void OptionsMenuState::Resume() {
	//se vuelven a dar de alta las funciones de escucha

	glutSpecialFunc(onSpecialKeyOptionsMenu);// Alta de la funcion de atencion al teclado especial
	glutKeyboardFunc(onKeyOptionsMenu);// Alta de la funcion de atencion al teclado 
}

void OptionsMenuState::Pause() {

}


void OptionsMenuState::HandleEvents(StateEngine* game) {

}


void OptionsMenuState::Update(StateEngine* game) {

}

void backgroundOpciones() {

	//Uso de las texturas
	glBindTexture(GL_TEXTURE_2D, textura_fondoOpciones);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	Point3D v0( -1.0,-1.0,0.0 );
	Point3D v1( 1.0,-1.0,0.0 );
	Point3D v3( -1.0,1.0,0.0 );
	Point3D v2( 1.0,1.0,0.0 );

	quadtex(v0, v1, v2, v3,
		0, 1, 0, 1, 1, 1);

}




void botonesOpciones() {

	if ( HorizontalButton == 0 && VerticalButton == 0) {
		glBindTexture(GL_TEXTURE_2D, textura_botonSeleccionadoResolucion);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, textura_botonSinSeleccionarResolucion);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	Point3D v0( -0.7,0.4,0.0 );
	Point3D v1( -0.3,0.4,0.0 );
	Point3D v3( -0.7,0.2,0.0 );
	Point3D v2( -0.3,0.2,0.0 );


	quadtex(v0, v1, v2, v3,
		0, 1, 0, 1, 1, 1);

	if (HorizontalButton == 1 && VerticalButton == 0) {
		glBindTexture(GL_TEXTURE_2D, textura_botonSeleccionadoResolucion);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, textura_botonSinSeleccionarResolucion);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	v0[0] = -0.2;
	v1[0] = 0.2;
	v3[0] = -0.2;
	v2[0] = 0.2;

	quadtex(v0, v1, v2, v3,
		0, 1, 0, 1, 1, 1);


	if (HorizontalButton == 2 && VerticalButton == 0) {
		glBindTexture(GL_TEXTURE_2D, textura_botonSeleccionadoResolucion);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, textura_botonSinSeleccionarResolucion);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	v0[0] = 0.3;
	v1[0] = 0.7;
	v3[0] = 0.3;
	v2[0] = 0.7;

	quadtex(v0, v1, v2, v3,
		0, 1, 0, 1, 1, 1);

}

void checks() {

	if (fullscreen == true) {

		if (HorizontalButton == 0 && VerticalButton == 1) {
			glBindTexture(GL_TEXTURE_2D, textura_noCheckSelected);
		}
		else {
			glBindTexture(GL_TEXTURE_2D, textura_noCheck);
		}
	
	}
	else {

		if (HorizontalButton == 0 && VerticalButton == 1) {
			glBindTexture(GL_TEXTURE_2D, textura_CheckSelected);
		}
		else {
			glBindTexture(GL_TEXTURE_2D, textura_Check);
		}

	}



	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	Point3D v0( -0.4,-0.1,0.0 );
	Point3D v1( -0.2,-0.1,0.0 );
	Point3D v3( -0.4,-0.3,0.0 );
	Point3D v2( -0.2,-0.3,0.0 );


	quadtex(v0, v1, v2, v3,
		0, 1, 0, 1, 1, 1);


	if (fullscreen == false) {

		if (HorizontalButton == 1 && VerticalButton == 1) {
			glBindTexture(GL_TEXTURE_2D, textura_noCheckSelected);
		}
		else {
			glBindTexture(GL_TEXTURE_2D, textura_noCheck);
		}

	}
	else {

		if (HorizontalButton == 1 && VerticalButton == 1) {
			glBindTexture(GL_TEXTURE_2D, textura_CheckSelected);
		}
		else {
			glBindTexture(GL_TEXTURE_2D, textura_Check);
		}

	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	v0[0] = 0.2;
	v1[0] = 0.4;
	v3[0] = 0.2;
	v2[0] = 0.4;

	quadtex(v0, v1, v2, v3,
		0, 1, 0, 1, 1, 1);
}

void textosOpciones() {

		//titulos
		textoStroke(-0.2, 0.5, 0.2, "Graficos", 0.12, 0.12, 0.12, NEGRO, GLUT_STROKE_ROMAN);
		textoStroke(-0.4, 0.00, 0.2, "Pantalla Completa", 0.12, 0.12, 0.12, NEGRO, GLUT_STROKE_ROMAN);
		
		//resolucion
		textoStroke(-0.58, 0.27, 0.2, "Bajos", 0.08, 0.08, 0.08, NEGRO, GLUT_STROKE_ROMAN);
		textoStroke(-0.1, 0.27, 0.2, "Medios", 0.08, 0.08, 0.08, NEGRO, GLUT_STROKE_ROMAN);
		textoStroke(0.425, 0.27, 0.2, "Altos", 0.08, 0.08, 0.08, NEGRO, GLUT_STROKE_ROMAN);

		//checks
		textoStroke(-0.7, -0.225, 0.2, "Desactivar", 0.08, 0.08, 0.08, NEGRO, GLUT_STROKE_ROMAN);
		textoStroke(0.0, -0.225, 0.2, "Activar", 0.08, 0.08, 0.08, NEGRO, GLUT_STROKE_ROMAN);

		//salir
		textoStroke(-0.15, -0.525, 0.4, "Guardar", 0.1, 0.1, 0.1, BLANCO, GLUT_STROKE_ROMAN);

}

void salir() {

	if (VerticalButton == 2) {
		glBindTexture(GL_TEXTURE_2D, textura_botonSeleccionadoResolucion);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, textura_botonSinSeleccionarResolucion);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	Point3D v0( -0.3,-0.4,0.0 );
	Point3D v1( 0.3,-0.4,0.0 );
	Point3D v3( -0.3,-0.6,0.0 );
	Point3D v2( 0.3,-0.6,0.0 );


	quadtex(v0, v1, v2, v3,
		0, 1, 0, 1, 1, 1);
}

void flecha() {
	
	glBindTexture(GL_TEXTURE_2D, textura_flechita);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	Point3D v0( -0.55,0.4,0.0 );
	Point3D v1( -0.45,0.4,0.0 );
	Point3D v3( -0.55,0.5,0.0 );
	Point3D v2( -0.45,0.5,0.0 );

	if (graficos == 1) {
		quadtex(v0, v1, v2, v3,
			0, 1, 0, 1, 1, 1);
	}
	else if (graficos == 2) {
		v0[0] = -0.05;
		v1[0] = 0.05;
		v3[0] = -0.05;
		v2[0] = 0.05;
		quadtex(v0, v1, v2, v3,
			0, 1, 0, 1, 1, 1);
	}
	else {
		v0[0] = 0.45;
		v1[0] = 0.55;
		v3[0] = 0.45;
		v2[0] = 0.55;
		quadtex(v0, v1, v2, v3,
			0, 1, 0, 1, 1, 1);
	}




}

//TODO completar esta mision
void OptionsMenuState::Draw(StateEngine* game) {

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


	backgroundOpciones();
	botonesOpciones();
	checks();
	salir();
	flecha();
	textosOpciones();

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


