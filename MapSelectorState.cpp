#include "StateEngine.h"
#include "State.h"
#include "MapSelectorState.h"
#include "CreationModeState.h"
#include "Utilidades.h" // Biblioteca de Utilidades
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
/*
Variables para el dibujo de la lista de ficheros
*/
GLint SelectedMap = 0;
GLint Pagina = 0;

/*
Sencillo metodod para seguir la logica de la actualizacion del menu.
*/
GLint actualizarSelectedMap(GLint button) {
	if (button<0) {
	
		if (Pagina != 0) {
			Pagina -= 1;
			button = 7;
		}
		else {
			button = (nombresFicheros.size() % 8) - 1;
			Pagina = (int)nombresFicheros.size() / 8;
		}
	}
	else if (button==8) {
		button = 0;
		Pagina += 1;
	}
	else if (button+(Pagina*8) == nombresFicheros.size()) {
		button = 0;
		Pagina = 0;
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
	cout << SelectedMap << "\n";
}


void onKeyMainSelector(unsigned char tecla, int x, int y)
// Funcion de atencion al teclado
{
	switch (tecla) {
	case 13://se pulsa enter
		fileMap = saveFolder + nombresFicheros[SelectedMap+(Pagina*8)];
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
	inicializarTextura(textura_fondoSelector, "./textures/SelectorMapa/Fondo.jpg");
	inicializarTextura(textura_Soporte, "./textures/SelectorMapa/Soporte.jpg");
	inicializarTextura(textura_Flecha, "./textures/SelectorMapa/Arrow.png");
	engineSelector = engine;

	inicializarVectorFicheros();

	glutSpecialFunc(onSpecialKeySelector);// Alta de la funcion de atencion al teclado especial
	glutKeyboardFunc(onKeyMainSelector);// Alta de la funcion de atencion al teclado 

}

//Limpiar texturas etc
void MapSelectorState::Cleanup() {
	glutSpecialFunc(NULL);
	glutKeyboardFunc(NULL);
	nombresFicheros.clear();
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

	Point3D v0( -1.0,-1.0,0.0 );
	Point3D v1( 1.0,-1.0,0.0 );
	Point3D v3( -1.0,1.0,0.0 );
	Point3D v2( 1.0,1.0,0.0 );

	quadtex(v0, v1, v2, v3,
		0, 1, 0, 1, 1, 1);

}


void Soporte() {

	//Uso de las texturas
	glBindTexture(GL_TEXTURE_2D, textura_Soporte);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	Point3D v0( -0.4,-0.8,0.0 );
	Point3D v1( 0.4,-0.8,0.0 );
	Point3D v3( -0.4,0.8,0.0 );
	Point3D v2( 0.4,0.8,0.0 );

	quadtex(v0, v1, v2, v3,
		0, 1, 0, 1, 1, 1);

}




void flechas() {

	glPushAttrib(GL_ALL_ATTRIB_BITS);


	glBindTexture(GL_TEXTURE_2D, textura_Flecha);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	Point3D v0( -0.6,0.8,0.0 );
	Point3D v1( -0.4,0.8,0.0 );
	Point3D v3( -0.6,0.6,0.0 );
	Point3D v2( -0.4,0.6,0.0 );

	quadtex(v0, v1, v2, v3,
		0, 1, 0, 1, 1, 1);


	glPushMatrix();

	glTranslatef(-1.0, 0, 0);
	glRotatef(180, 0, 0, 1);
	quadtex(v0, v1, v2, v3,
		0, 1, 0, 1, 1, 1);
	glPopMatrix();

	
	glPopAttrib();

}

/*
dibuja la lista de mapas cargados, dibujando 8 mapas a la vez y una vez llegemos al octavo dibujar los siguientes 8, apoyandose en la variable de seleccion.
*/
void mapasCargados() {

	if (nombresFicheros.size() == 0) {
		textoStroke(-0.39, 0 , 0.1, "No hay ningun mapa guardado", 0.06, 0.06, 0.05, ROJO, GLUT_STROKE_ROMAN);
	}
	else {

		for (int i = Pagina*8; i < nombresFicheros.size() && i<(Pagina+1)*8; i++) {
			string str = nombresFicheros[i].c_str();
			std::vector<char> nombre(str.begin(), str.end());
			nombre.push_back('\0');
			if(i == SelectedMap+Pagina*8)
			textoStroke(-0.38, 0.65-(0.17*(i-Pagina*8)), 0.1,&nombre[0], 0.06, 0.06, 0.06, AMARILLO, GLUT_STROKE_ROMAN);
			else
			textoStroke(-0.38, 0.65 - (0.17*(i - Pagina * 8)), 0.1, &nombre[0], 0.06, 0.06, 0.06, BLANCO, GLUT_STROKE_ROMAN);
		}
	}
	
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
	mapasCargados();

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


