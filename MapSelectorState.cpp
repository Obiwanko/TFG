#include "StateEngine.h"
#include "State.h"
#include "MapSelectorState.h"
#include "CreationModeState.h"
#include <Utilidades.h> // Biblioteca de Utilidades
#include "Globals.h"
#include <string>
#include <filesystem>
#include <iostream>

namespace fs = std::experimental::filesystem;

MapSelectorState MapSelectorState::_MapSelectorState;
//Identificadores texturas
GLuint textura_fondoSelector;
GLuint textura_Soporte;
GLuint textura_Flecha;

std::vector<string> nombresFicheros;

StateEngine* engineSelector;
GLint SelectedMap = 0;

/*
Sencillo metodod para seguir la logica de la actualizacion del menu.
*/
GLint actualizarSelectedMap(GLint button) {
	if (button<0) {
		button = nombresFicheros.size()-1;
	}
	else if (button == nombresFicheros.size()) {
		button = 0;
	}
	return button;
}

void onSpecialKeySelector(int specialKey, int x, int y) {
	switch (specialKey) {
	case GLUT_KEY_LEFT:
		break;
	case GLUT_KEY_RIGHT:
		break;
	case GLUT_KEY_UP:
		SelectedMap = actualizarSelectedMap(SelectedMap - 1);
		break;
	case GLUT_KEY_DOWN:
		SelectedMap = actualizarSelectedMap(SelectedMap + 1);
		break;
	}

}

void init_de_TexturaSelector(GLuint &id, char* nombre)
{
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	loadImageFile(nombre);
}

void onKeyMainSelector(unsigned char tecla, int x, int y)
// Funcion de atencion al teclado
{
	switch (tecla) {
	case 13://se pulsa enter
		fileMap = saveFolder + nombresFicheros[SelectedMap];
		engineSelector->ChangeState(CreationModeState::Instance());
		break;
	case 8: //se pulsa retroceso.
		break;
	case 27: // Pulso escape
		engineSelector->PopState();
	}
}

void inicializarVectorFicheros() {

	for (auto & p : fs::directory_iterator(saveFolder)) {
		string file = p.path().filename().string();
		if (file.find(".txt") != std::string::npos) {
			nombresFicheros.push_back(file);
		}
	}
	
}

void MapSelectorState::Init(StateEngine* engine) {
	init_de_TexturaSelector(textura_fondoSelector, "./textures/SelectorMapa/Fondo.jpg");
	init_de_TexturaSelector(textura_Soporte, "./textures/SelectorMapa/Soporte.jpg");
	init_de_TexturaSelector(textura_Flecha, "./textures/SelectorMapa/Arrow.png");
	engineSelector = engine;

	inicializarVectorFicheros();

	glutSpecialFunc(onSpecialKeySelector);// Alta de la funcion de atencion al teclado especial
	glutKeyboardFunc(onKeyMainSelector);// Alta de la funcion de atencion al teclado 

}

//Limpiar texturas etc
void MapSelectorState::Cleanup() {
	glutSpecialFunc(NULL);
	glutKeyboardFunc(NULL);
}


void MapSelectorState::Resume() {
	//se vuelven a dar de alta las funciones de escucha
	glutSpecialFunc(onSpecialKeySelector);// Alta de la funcion de atencion al teclado especial
	glutKeyboardFunc(onKeyMainSelector);// Alta de la funcion de atencion al teclado 
}
//limpiamos funciones de escucha
void MapSelectorState::Pause() {
	glutSpecialFunc(NULL);
	glutKeyboardFunc(NULL);
}


void MapSelectorState::HandleEvents(StateEngine* game) {

}


void MapSelectorState::Update(StateEngine* game) {

}

void backgroundSelector() {

	//Uso de las texturas
	glBindTexture(GL_TEXTURE_2D, textura_fondoSelector);
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


void Soporte() {

	//Uso de las texturas
	glBindTexture(GL_TEXTURE_2D, textura_Soporte);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	GLfloat v0[3] = { -0.4,-0.8,0.0 };
	GLfloat v1[3] = { 0.4,-0.8,0.0 };
	GLfloat v3[3] = { -0.4,0.8,0.0 };
	GLfloat v2[3] = { 0.4,0.8,0.0 };

	quadtex((GLfloat*)v0, (GLfloat*)v1, (GLfloat*)v2, (GLfloat*)v3,
		0, 1, 0, 1, 1, 1);

}




void flechas() {

	glPushAttrib(GL_ALL_ATTRIB_BITS);


	glBindTexture(GL_TEXTURE_2D, textura_Flecha);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	GLfloat v0[3] = { -0.6,-0.3,0.0 };
	GLfloat v1[3] = { -0.4,-0.3,0.0 };
	GLfloat v3[3] = { -0.6,-0.5,0.0 };
	GLfloat v2[3] = { -0.4,-0.5,0.0 };

	quadtex((GLfloat*)v0, (GLfloat*)v1, (GLfloat*)v2, (GLfloat*)v3,
		0, 1, 0, 1, 1, 1);


	glPushMatrix();

	glTranslatef(-1.0, -1, 0);
	glRotatef(180, 0, 0, 1);
	quadtex((GLfloat*)v0, (GLfloat*)v1, (GLfloat*)v2, (GLfloat*)v3,
		0, 1, 0, 1, 1, 1);
	glPopMatrix();

	glPopAttrib();

}

/*
TODO dibujar la lista de mapas cargados, dibujando 8 mapas a la vez y una vez llegemos al octavo dibujar los siguientes 8, apoyandose en la variable de seleccion.
*/
void mapasCargados() {

}


//TODO completar esta mision
void MapSelectorState::Draw(StateEngine* game) {

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
							 //glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR); //brillos por separado

	backgroundSelector();
	Soporte();
	flechas();

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


