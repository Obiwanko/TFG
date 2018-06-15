#include "StateEngine.h"
#include "State.h"
#include "CreationModeState.h"
#include "OptionsMenuState.h"
#include "MapSelectorState.h"
#include "MainMenuState.h"
#include <Utilidades.h> // Biblioteca de Utilidades
#include "Globals.h"
#include <iostream> // Biblioteca de entrada salida
#include <fstream>
#include <sstream> // Biblioteca de manejo de strings
#include <cmath> // Biblioteca matematica de C

CreationModeState CreationModeState::_CreationModeState;
//Engine para poder realizar los cambios.
StateEngine* engineCreation;


/*
Variables que indican los diferentes directorios
*/
/*
**********************************************************************
Variables de usadas para el funcionamiento de la camara
**********************************************************************
*/
Camera camaraflotante;
GLfloat velocidadcamara=1.0;
GLfloat lastx, lasty;


/*

Variables de atencion al HUD en el modo de creacion de circuitos

*/
//si el tramo esta seleccionado o no(ya se ha pulsado enter)
GLboolean seleccionado = false;

// indica que tramo tenenemos enfocado, en caso de pulsar enter dibujaremos dicho tramo
GLint tramoactual = 0;

//control de la rotacion de las piezas en el HUD
GLfloat rotacion_pieza = 0;

GLboolean pausa = false;


/*

Variables de atencion a la lista de tramos que tenenemos actualmente en memoria

*/

//total de piezas diferentes disponibles
GLint const totalTramos = 7;


//Identificadores texturas
GLuint textura_carretera;
GLuint textura_fondo;
GLuint textura_nukacola;
GLuint textura_metal;
GLuint textura_slurm;
GLuint textura_sonic;
GLuint textura_hierba;
GLuint textura_panel_trasero;
GLuint textura_cielonoche;
GLuint textura_cielodia;
GLuint textura_victoria;
GLuint textura_velocimetro;
GLuint textura_aguja;
GLuint textura_bandera1;
GLuint textura_bandera2;
GLuint textura_bandera3;
GLuint textura_goal;
GLuint textura_start;
GLuint textura_coche;

//boleanos de click izquierdo/click derecho

BOOLEAN leftclick = false;
BOOLEAN rightclick = false;


//variables de uso durante la pausa
GLint ButtonPausa = 0;
GLuint textura_BotonSeleccionadoPausa;
GLuint textura_BotonSinSeleccionarPausa;

void init_de_Textura(GLuint &id, char* nombre)
{
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	loadImageFile(nombre);
}




void onIdleCreation()
// Funcion de atencion al evento idle
{

	static int antesc = 0;
	int ahorac, tiempo_transcurridoc;

	ahorac = glutGet(GLUT_ELAPSED_TIME); //Tiempo transcurrido desde el inicio
	tiempo_transcurridoc = ahorac - antesc; //Tiempo transcurrido desde antes en msg.

	rotacion_pieza += float((0.036f*float(tiempo_transcurridoc)));

	antesc = ahorac;
	glutPostRedisplay();
}


void iluminacion() {
	// Luces
	//Luz de la luna
	glEnable(GL_LIGHT0);
	GLfloat ambiental0[4] = { (0.05f),(0.05f),(0.05f) ,(1.0f) };
	GLfloat difusa0[4] = { (0.05f),(0.05f),(0.05f) ,(1.0f) };
	GLfloat especular0[4] = { (0.0f),(0.0f),(0.0f) ,(1.0f) };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambiental0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, difusa0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, especular0);
	GLfloat posicion0[] = { 0.0f, 10.0f, 0.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, posicion0);

	//Luz del coche
	glEnable(GL_LIGHT1);
	GLfloat ambiental1[4] = { (0.2f),(0.2f),(0.2f) ,(1.0f) };
	GLfloat difusa1[4] = { (1.0f),(1.0f),(1.0f) ,(1.0f) };
	GLfloat especular1[4] = { (0.3f),(0.3f),(0.3f) ,(0.3f) };

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambiental1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, difusa1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, especular1);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 25.0);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 20);

	//Farolas
	GLfloat ambiental2[] = { (0.0f),(0.0f),(0.0f) ,(1.0f) };
	GLfloat difusa2[] = { (0.5f),(0.5f),(0.2f) ,(1.0f) };
	GLfloat especular2[] = { (0.0f),(0.0f),(0.0f) ,(1.0f) };

	glEnable(GL_LIGHT2);
	glLightfv(GL_LIGHT2, GL_AMBIENT, ambiental2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, difusa2);
	glLightfv(GL_LIGHT2, GL_SPECULAR, especular2);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 45.0);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 10.0);

	glEnable(GL_LIGHT3);
	glLightfv(GL_LIGHT3, GL_AMBIENT, ambiental2);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, difusa2);
	glLightfv(GL_LIGHT3, GL_SPECULAR, especular2);
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 45.0);
	glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 10.0);

	glEnable(GL_LIGHT4);
	glLightfv(GL_LIGHT4, GL_AMBIENT, ambiental2);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, difusa2);
	glLightfv(GL_LIGHT4, GL_SPECULAR, especular2);
	glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, 45.0);
	glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, 10.0);

	glEnable(GL_LIGHT5);
	glLightfv(GL_LIGHT5, GL_AMBIENT, ambiental2);
	glLightfv(GL_LIGHT5, GL_DIFFUSE, difusa2);
	glLightfv(GL_LIGHT5, GL_SPECULAR, especular2);
	glLightf(GL_LIGHT5, GL_SPOT_CUTOFF, 45.0);
	glLightf(GL_LIGHT5, GL_SPOT_EXPONENT, 10.0);


	// Material carretera
	GLfloat difuso[4] = { (0.8f),(0.8f),(0.8f) ,(1.0f) };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, difuso);
	GLfloat specular[4] = { (0.3f),(0.3f),(0.3f) ,(1.0f) };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 3);

	// Sombreado
	glShadeModel(GL_FLAT);

}

/*
Funcion para identificar el tipo de tramo y añadirlo al vector en memoria que se muestra.
*/
void addTramoToVector(int id, int posID, string line) {
	/*
	No tendremos mas de 10 parametros
	*/
	GLfloat parameters[10];

	// Nos saltamos el primer ; ya que sabemos que es el id
	int aux = 0;
	string parametros = line.substr(posID + 1);
	int found = parametros.find_first_of(";");
	//vamos añadiendo los parametros al array ya creado.
	int i = 0;
	while (found != std::string::npos)
	{
		parameters[i] = stof(parametros.substr(aux, found - aux));
		aux = found + 1;
		found = parametros.find_first_of(";", found + 1);
		i++;
	}
	parameters[i] = stof(parametros.substr(aux, parametros.length() - aux));
	switch (id) {
	case 1:
		vectorTramosEnMemoria.push_back(new Tramo(parameters[0], parameters[1], resolucion, repeticionTex));
		break;
	case 2:
		vectorTramosEnMemoria.push_back(new TramoCurvo(parameters[0], parameters[1], parameters[2], resolucion, repeticionTex));
		break;
	case 3:
		vectorTramosEnMemoria.push_back(new Rampa(parameters[0], parameters[1], parameters[2], resolucion, repeticionTex));
		break;
	case 4:
		vectorTramosEnMemoria.push_back(new RampaCurva(parameters[0], parameters[1], parameters[2], parameters[3], resolucion, repeticionTex));
		break;
	case 5:
		vectorTramosEnMemoria.push_back(new TramoSinuosoHorizontal(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], resolucion, repeticionTex));
		break;
	case 6:
		vectorTramosEnMemoria.push_back(new TramoSinuosoVertical(parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], resolucion, repeticionTex));
		break;
	case 7:
		vectorTramosEnMemoria.push_back(new Looping(parameters[0], parameters[1], parameters[2], resolucion, repeticionTex));
		break;
	default:
		break;
	}
}

/*
Funciones de atencion a la carga y guardado de los ficheros.
*/
void cargarCircuitoFromFile() {
	std::ifstream i(fileMap);
	
	if (!i.fail()) {
		auto line = std::string{};

		while (std::getline(i, line)) {
			auto pos = line.find_first_of(';');
			string id = line.substr(0, pos);
			addTramoToVector(std::stoi(id), pos, line);
			//vectorTramosEnMemoria.push_back(new Student{ std::string{ line, 0, pos },std::string{ line, pos + 1 } });
		}
	}

}

void guardarCircuitoToFile() {
	//TODO hacer que el fichero se introduzca por pantalla.
	std::ofstream file(fileMap);
	for (int i = 0; i < vectorTramosEnMemoria.size(); i++) {
		vectorTramosEnMemoria[i]->writeToFile(file);
	}
	file.close();
}

int hudNumeroSelector(int tramoaux) {

	if (tramoaux >= totalTramos) {
		return tramoaux - totalTramos;
	}
	else if (tramoaux < 0) {
		return tramoaux + totalTramos;
	}
	else {
		return tramoaux;
	}

}

void onSpecialKeyModoCreacion(int specialKey, int x, int y) {
	switch (specialKey) {
	case GLUT_KEY_LEFT:
		if (!seleccionado) {
			tramoactual = hudNumeroSelector(tramoactual - 1);
		}
		break;
	case GLUT_KEY_RIGHT:
		if (!seleccionado) {
			tramoactual = hudNumeroSelector(tramoactual + 1);
		}
		break;
	case GLUT_KEY_UP:

		break;
	case GLUT_KEY_DOWN:

		break;
	}

}



void mouseMovement(int x, int y) {
	//Comprobar diferencia entre la 'x' de y la 'y' de la posicion actual del raton con la anterior
	int diffx = x - lastx;
	int diffy = y - lasty;
	//Actualizamos la posicion anterior para poder continuar con el desplazamiento
	lastx = x;
	lasty = y;
	//Actualizamos los valores de la rotacion de la camara
	camaraflotante.ProcessMouseMovement(diffx, diffy);

}

void mouse(int button, int state, int x, int y)
{
	//evento de pulsar el click izquierdo
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
	{
		lastx = x;
		lasty = y;
		glutMotionFunc(mouseMovement);
		leftclick = true;
	}

	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP))
	{
		//desactivamos la escucha
		glutMotionFunc(NULL);
		leftclick = false;
	}

	if ((button == GLUT_RIGHT_BUTTON) && (state == GLUT_DOWN))
	{
		rightclick = true;
	}

	if ((button == GLUT_RIGHT_BUTTON) && (state == GLUT_UP))
	{
		rightclick = false;
	}
}


/*
Funciones de escucha durante la puasa
*/
/*
Sencillo metodo para seguir la logica de la actualizacion del menu.
*/
GLint actualizarButtonPausa(GLint button) {
	if (button<0) {
		button = 3;
	}
	else if (button == 4) {
		button = 0;
	}
	return button;
}

void onSpecialKeyPausa(int specialKey, int x, int y) {
	switch (specialKey) {
	case GLUT_KEY_LEFT:
		break;
	case GLUT_KEY_RIGHT:
		break;
	case GLUT_KEY_UP:
		ButtonPausa = actualizarButtonPausa(ButtonPausa - 1);
		break;
	case GLUT_KEY_DOWN:
		ButtonPausa = actualizarButtonPausa(ButtonPausa + 1);
		break;
	}
	cout << " specialkey " << ButtonPausa << "\n";
}


void vaciarTramosEnMemoria() {
	//while (!vectorTramosEnMemoria.empty())
		vectorTramosEnMemoria.clear();
}

void onKeyPausa(unsigned char tecla, int x, int y)
// Funcion de atencion al teclado
{	
	cout << " key " << ButtonPausa << "\n";
	//float xrotrad, yrotrad;
	switch (tecla) {
	case 13://se pulsa enter
		switch (ButtonPausa)
		{
		case 0: //Reanudamos el modo creacion
			CreationModeState::Instance()->Resume();
			break;
		case 1: // Seleccionamos otro mapa
			pausa = false;
			vaciarTramosEnMemoria();
			engineCreation->PushState(MapSelectorState::Instance());
			break;
		case 2: // entramos a las opciones
			pausa = false;
			engineCreation->PushState(OptionsMenuState::Instance());
			break;
		case 3: //guardamos y salimos
			guardarCircuitoToFile();
			engineCreation->Cleanup();
			engineCreation->PushState(MainMenuState::Instance());
			break;
		default:
			break;
		}
		break;
	case 8: //se pulsa retroceso.
		break;
	case 27: // Pulso escape
		CreationModeState::Instance()->Resume();
		
	}
}



void añade_tramo(GLint identificador) {
	// sumamos +1 ya que los arrays empiezan por 0
	//TODO añadir la variables que utilizaré en los globals
	switch (identificador + 1) {
	case 1:
		vectorTramosEnMemoria.push_back(new Tramo(2, 10, resolucion, repeticionTex));
		break;
	case 2:
		vectorTramosEnMemoria.push_back(new TramoCurvo(2, 5, 90, resolucion, repeticionTex));
		break;
	case 3:
		vectorTramosEnMemoria.push_back(new Rampa(2, 5, 1, resolucion, repeticionTex));
		break;
	case 4:
		vectorTramosEnMemoria.push_back(new RampaCurva(2, 5, 90, 10, resolucion, repeticionTex));
		break;
	case 5:
		vectorTramosEnMemoria.push_back(new TramoSinuosoHorizontal(2, 10, 2, 1, 1, resolucion, repeticionTex));
		break;
	case 6:
		vectorTramosEnMemoria.push_back(new TramoSinuosoVertical(2, 10, 2, 1, 1, resolucion, repeticionTex));
		break;
	case 7:
		vectorTramosEnMemoria.push_back(new Looping(2, 5, 4.5, resolucion, repeticionTex));
		break;
	}
}

void onKeyCreacion(unsigned char tecla, int x, int y)
// Funcion de atencion al teclado
{
	//float xrotrad, yrotrad;
	switch (tecla) {
	case 'W':
	case 'w':
		camaraflotante.ProcessKeyboard(FORWARD, velocidadcamara);
		break;

	case 'S':
	case 's':
		camaraflotante.ProcessKeyboard(BACKWARD, velocidadcamara);
		break;

	case 'D':
	case 'd':
		camaraflotante.ProcessKeyboard(RIGHT, velocidadcamara);
		break;
	case 'A':
	case 'a':
		camaraflotante.ProcessKeyboard(LEFT, velocidadcamara);
		break;
	case ' ':
		//enfocamos a la ultima pieza colocada
		//camaraflotante.LookLastPiece(posicionUltimoTramo.x, posicionUltimoTramo.y+10, posicionUltimoTramo.z);
		break;
	case 13:
		if (seleccionado)
		{
			//si ya se habia seleccionado una pieza la dibujamos
			añade_tramo(tramoactual);
		}
		else
		{
			// si no, simplemente marcamos la pieza como seleccionada
		}
		seleccionado = !seleccionado;
		break;
	case 8: //se pulsa retroceso.
		if (seleccionado) // si una pieza esta seleccionada la deseleccionamos
			seleccionado = !seleccionado;
		else {//si no tenenmos pieza seleccionada la eliminamos, TODO añadir mensaje de confirmacion.
			if (!vectorTramosEnMemoria.empty()) {
				vectorTramosEnMemoria.pop_back();
			}
		}
		break;
	case 27: // Pulso escape
		ButtonPausa = 0;
		CreationModeState::Instance()->Pause();
		//damos de alta a las funciones de escucha del menu
		glutSpecialFunc(onSpecialKeyPausa);
		glutKeyboardFunc(onKeyPausa);
		
		//guardarCircuitoToFile();
		//exit(0);
	}
}


void CreationModeState::Init(StateEngine* engine) {
	cargarCircuitoFromFile();

	engineCreation = engine;
	glutSpecialFunc(onSpecialKeyModoCreacion);// Alta de la funcion de atencion al teclado especial
	glutKeyboardFunc(onKeyCreacion);// Alta de la funcion de atencion al teclado 
	glutIdleFunc(onIdleCreation); // Alta de la funcion de atencion a idle
	glutMouseFunc(mouse);//Alta de la funcion de atencion a los botones del raton

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glutSetWindowTitle("Creador de Circuitos: Modo Creacion");

	//inicializamos la camara con los valores por defecto.
	camaraflotante = Camera();

	pausa = false;
	iluminacion();

	//Texturas
	init_de_Textura(textura_carretera, "./textures/carretera.jpg");
	init_de_Textura(textura_fondo, "./textures/fondo.jpg");
	init_de_Textura(textura_nukacola, "./textures/nukacola.jpg");
	init_de_Textura(textura_metal, "./textures/metal.jpg");
	init_de_Textura(textura_slurm, "./textures/slurm.jpg");
	init_de_Textura(textura_sonic, "./textures/sonic.jpg");
	init_de_Textura(textura_hierba, "./textures/hierba.jpg");
	init_de_Textura(textura_panel_trasero, "./textures/paneltrasero.jpg");
	init_de_Textura(textura_cielonoche, "./textures/cielonoche.jpg");
	init_de_Textura(textura_cielodia, "./textures/cielodia.jpg");
	init_de_Textura(textura_victoria, "./textures/victory.PNG");
	init_de_Textura(textura_velocimetro, "./textures/velocimetro.PNG");
	init_de_Textura(textura_aguja, "./textures/aguja.jpg");
	init_de_Textura(textura_bandera1, "./textures/bandera1.PNG");
	init_de_Textura(textura_bandera2, "./textures/bandera2.PNG");
	init_de_Textura(textura_bandera3, "./textures/bandera3.PNG");
	init_de_Textura(textura_goal, "./textures/goal.PNG");
	init_de_Textura(textura_start, "./textures/start.PNG");
	init_de_Textura(textura_coche, "./textures/coche.PNG");
	init_de_Textura(textura_BotonSeleccionadoPausa, "./textures/ButtonSelected.jpg");
	init_de_Textura(textura_BotonSinSeleccionarPausa, "./textures/ButtonNotSelected.jpg");
}

//Limpiar texturas etc
void CreationModeState::Cleanup() {
	glutSpecialFunc(NULL);
	glutKeyboardFunc(NULL);
	//glutIdleFunc(NULL);
	glutMouseFunc(NULL);
}

void updateRes() {
	for (int i = 0; i < vectorTramosEnMemoria.size(); i++) {
		vectorTramosEnMemoria[i]->setRes(resolucion);
	}
}

void CreationModeState::Resume() {
	pausa = false;
	glutSpecialFunc(onSpecialKeyModoCreacion);// Alta de la funcion de atencion al teclado especial
	glutKeyboardFunc(onKeyCreacion);// Alta de la funcion de atencion al teclado 
	glutIdleFunc(onIdleCreation); // Alta de la funcion de atencion a idle
	glutMouseFunc(mouse);//Alta de la funcion de atencion a los botones del raton
	updateRes();
}

void CreationModeState::Pause() {
	pausa = true;
	//desactivamos las funciones de escucha
	glutSpecialFunc(NULL);
	glutKeyboardFunc(NULL);
	//glutIdleFunc(NULL); 
	glutMouseFunc(NULL);
}


void CreationModeState::HandleEvents(StateEngine* game) {

}


void CreationModeState::Update(StateEngine* game) {

}



void dibujarTramosEnLista() {

	for (int i = 0; i < vectorTramosEnMemoria.size(); i++) {
		vectorTramosEnMemoria[i]->drawing();
	}
}


void dibujarCircuitoEnMemoria()
{



	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D); //habilitamos textura
							 //glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR); //brillos por separado

							 //Uso de las texturas
	glBindTexture(GL_TEXTURE_2D, textura_carretera);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (iluminacion) {
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	}
	else {
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	}


	glPushMatrix();
	{
		dibujarTramosEnLista();
	}
	glPopMatrix();
	glPopAttrib();
}

void dibuja_tramo_HUD(GLint identificador) {
	// sumamos +1 ya que los arrays empiezan por 0
	switch (identificador + 1) {
	case 1:
		glRotatef(90, 1, 0, 0);
		glRotatef(90, 0, 1, 0);
		glRotatef(rotacion_pieza, 1, 0, 0);
		Tramo(0.05, 0.15, resolucion, repeticionTex).draw();
		break;
	case 2:
		glRotatef(90, 1, 0, 0);
		glRotatef(90, 0, 1, 0);
		glRotatef(rotacion_pieza, 1, 0, 0);
		TramoCurvo(0.05, 0.15, 90, resolucion, repeticionTex).draw();
		break;
	case 3:
		glRotatef(90, 1, 0, 0);
		glRotatef(90, 0, 1, 0);
		glRotatef(rotacion_pieza, 1, 0, 0);
		Rampa(0.05, 0.10, 0.5, resolucion, repeticionTex).draw();
		break;
	case 4:
		glRotatef(90, 1, 0, 0);
		glRotatef(90, 0, 1, 0);
		glRotatef(rotacion_pieza, 1, 0, 0);
		RampaCurva(0.05, 0.08, 90, 10, resolucion, repeticionTex).draw();
		break;
	case 5:
		glRotatef(90, 1, 0, 0);
		glRotatef(90, 0, 1, 0);
		glRotatef(rotacion_pieza, 1, 0, 0);
		glScalef(0.01, 0.01, 0.01);
		TramoSinuosoHorizontal(5, 15, 2, 2, true, resolucion, repeticionTex).draw();
		break;
	case 6:
		glRotatef(90, 1, 0, 0);
		glRotatef(90, 0, 1, 0);
		glRotatef(rotacion_pieza, 1, 0, 0);
		glScalef(0.01, 0.01, 0.01);
		TramoSinuosoVertical(5, 15, 2, 2, true, resolucion, repeticionTex).draw();
		break;
	case 7:
		//glRotatef(-180, 1, 0, 0);
		glRotatef(90, 0, 1, 0);
		glRotatef(rotacion_pieza, 0, 1, 0);
		Looping(0.05, 0.06, 0.08, resolucion, repeticionTex).draw();
		break;
		//TODO añadir nuevas piezas conforme las vaya haciendo
	}
}


void hudElementBaseSelectorPiezas() {

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	// Habilitamos blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Z-Buffer
	glDepthMask(GL_FALSE);
	// Dibujar traslucidos

	//la tira donde se colocaran las piezas
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	//rgb(50,205,50)
	glColor4f(0.196, 0.8, 0.196, 0.7);
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

	//ahora se crean los cuadrados blancos que servirán para resaltar las piezas seleccionadas

	// mas a la izquierda
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	//rgb(50,205,50)
	glColor4f(1, 1, 1, 0.5);
	glBegin(GL_QUAD_STRIP);
	glTexCoord2f(0, 0);
	glVertex3f(-0.7, 0.925, 0.9);
	glTexCoord2f(0, 1);
	glVertex3f(-0.7, 0.725, 0.9);
	glTexCoord2f(1, 0);
	glVertex3f(-0.5, 0.925, 0.9);
	glTexCoord2f(1, 1);
	glVertex3f(-0.5, 0.725, 0.9);
	glEnd();
	glPopAttrib();

	//izquierda - central
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	//rgb(50,205,50)
	glColor4f(1, 1, 1, 0.5);
	glBegin(GL_QUAD_STRIP);
	glTexCoord2f(0, 0);
	glVertex3f(-0.4, 0.925, 0.9);
	glTexCoord2f(0, 1);
	glVertex3f(-0.4, 0.725, 0.9);
	glTexCoord2f(1, 0);
	glVertex3f(-0.2, 0.925, 0.9);
	glTexCoord2f(1, 1);
	glVertex3f(-0.2, 0.725, 0.9);
	glEnd();
	glPopAttrib();

	//central
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	//rgb(50,205,50)
	if (seleccionado) {
		glColor4f(1, 1, 0, 0.8);
	}
	else {
		glColor4f(1, 1, 1, 0.8);
	}

	glBegin(GL_QUAD_STRIP);
	glTexCoord2f(0, 0);
	glVertex3f(-0.1, 0.925, 0.9);
	glTexCoord2f(0, 1);
	glVertex3f(-0.1, 0.725, 0.9);
	glTexCoord2f(1, 0);
	glVertex3f(0.1, 0.925, 0.9);
	glTexCoord2f(1, 1);
	glVertex3f(0.1, 0.725, 0.9);
	glEnd();
	glPopAttrib();

	//derecho central
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	//rgb(50,205,50)
	glColor4f(1, 1, 1, 0.5);
	glBegin(GL_QUAD_STRIP);
	glTexCoord2f(0, 0);
	glVertex3f(0.2, 0.925, 0.9);
	glTexCoord2f(0, 1);
	glVertex3f(0.2, 0.725, 0.9);
	glTexCoord2f(1, 0);
	glVertex3f(0.4, 0.925, 0.9);
	glTexCoord2f(1, 1);
	glVertex3f(0.4, 0.725, 0.9);
	glEnd();
	glPopAttrib();

	//derecho
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	//rgb(50,205,50)
	glColor4f(1, 1, 1, 0.5);
	glBegin(GL_QUAD_STRIP);
	glTexCoord2f(0, 0);
	glVertex3f(0.5, 0.925, 0.9);
	glTexCoord2f(0, 1);
	glVertex3f(0.5, 0.725, 0.9);
	glTexCoord2f(1, 0);
	glVertex3f(0.7, 0.925, 0.9);
	glTexCoord2f(1, 1);
	glVertex3f(0.7, 0.725, 0.9);
	glEnd();
	glPopAttrib();


	// Z-Buffer a estado normal
	glDepthMask(GL_TRUE);
	glPopAttrib();

}


//TODO continuar con el selector de piezas, ver como hacer las animaciones mas automatizables y la colocacion de las piezas mas automatizables.
void hudElementPiezasVisibles() {
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	// Habilitamos blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Z-Buffer
	glDepthMask(GL_FALSE);

	//derecho
	glPushAttrib(GL_ALL_ATTRIB_BITS);


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//Posible problema con la luz
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D); //habilitamos textura
							 //glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR); //brillos por separado

							 //Uso de las texturas
	glBindTexture(GL_TEXTURE_2D, textura_carretera);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);


	//posicion izquierda central
	glPushMatrix();
	glTranslatef(-0.6, 0.75, 0.8);
	//rotacion a aplicar.
	dibuja_tramo_HUD(hudNumeroSelector(tramoactual - 2));
	glPopMatrix();



	//posicion izquierda central
	glPushMatrix();
	glTranslatef(-0.3, 0.75, 0.8);
	//rotacion a aplicar.
	dibuja_tramo_HUD(hudNumeroSelector(tramoactual - 1));
	glPopMatrix();

	//posicion central
	glPushMatrix();
	glTranslatef(0.0, 0.75, 0.8);
	//rotacion a aplicar.
	dibuja_tramo_HUD(hudNumeroSelector(tramoactual));
	glPopMatrix();

	//posicion derecha central
	glPushMatrix();
	glTranslatef(0.3, 0.75, 0.8);
	//rotacion a aplicar.
	dibuja_tramo_HUD(hudNumeroSelector(tramoactual + 1));
	glPopMatrix();

	//posicion derecha
	glPushMatrix();
	glTranslatef(0.6, 0.75, 0.8);
	//rotacion a aplicar.
	dibuja_tramo_HUD(hudNumeroSelector(tramoactual + 2));
	glPopMatrix();

	glPopAttrib();
	// Z-Buffer a estado normal
	glDepthMask(GL_TRUE);
	glPopAttrib();
}


void botonesPausa() {

	if (ButtonPausa == 0) {
		glBindTexture(GL_TEXTURE_2D, textura_BotonSeleccionadoPausa);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, textura_BotonSinSeleccionarPausa);
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

	if (ButtonPausa == 1) {
		glBindTexture(GL_TEXTURE_2D, textura_BotonSeleccionadoPausa);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, textura_BotonSinSeleccionarPausa);
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


	if (ButtonPausa == 2) {
		glBindTexture(GL_TEXTURE_2D, textura_BotonSeleccionadoPausa);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, textura_BotonSinSeleccionarPausa);
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

	if (ButtonPausa == 3) {
		glBindTexture(GL_TEXTURE_2D, textura_BotonSeleccionadoPausa);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, textura_BotonSinSeleccionarPausa);
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


void textosBotonesPausa() {
	if (ButtonPausa == 0)
		textoStroke(-0.18, 0.07, 0.2, "Reanudar", 0.08, 0.08, 0.08, AMARILLO, GLUT_STROKE_ROMAN);
	else
		textoStroke(-0.18, 0.07, 0.2, "Reanudar", 0.08, 0.08, 0.08, BLANCO, GLUT_STROKE_ROMAN);

	if (ButtonPausa == 1)
		textoStroke(-0.27, -0.23, 0.2, "Cargar Circuito", 0.08, 0.08, 0.08, AMARILLO, GLUT_STROKE_ROMAN);
	else
		textoStroke(-0.27, -0.23, 0.2, "Cargar Circuito", 0.08, 0.08, 0.08, BLANCO, GLUT_STROKE_ROMAN);

	if (ButtonPausa == 2)
		textoStroke(-0.18, -0.53, 0.2, "Opciones", 0.08, 0.08, 0.08, AMARILLO, GLUT_STROKE_ROMAN);
	else
		textoStroke(-0.18, -0.53, 0.2, "Opciones", 0.08, 0.08, 0.08, BLANCO, GLUT_STROKE_ROMAN);

	if (ButtonPausa == 3)
		textoStroke(-0.27, -0.83, 0.2, "Salir y Guardar", 0.08, 0.08, 0.08, AMARILLO, GLUT_STROKE_ROMAN);
	else
		textoStroke(-0.27, -0.83, 0.2, "Salir y Guardar", 0.08, 0.08, 0.08, BLANCO, GLUT_STROKE_ROMAN);

}

void menuPausa() {
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Z-Buffer
	glDepthMask(GL_FALSE);
	// Dibujar traslucidos
	//la tira donde se colocaran las piezas
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D); //habilitamos textura
	
	botonesPausa();
	textosBotonesPausa();
	glPopAttrib();
}

void hudModoCreacion() {


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


	if (!pausa) {
		hudElementBaseSelectorPiezas();
		hudElementPiezasVisibles();
	}
	else {
		menuPausa();
	}


	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

}

void dibujoCircuito() {
	// Borra buffers y selecciona modelview
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	camaraflotante.SetGluLookUp();

	glPolygonMode(GL_FRONT, GL_LINE);

	dibujarCircuitoEnMemoria();


}



//TODO completar esta mision
void CreationModeState::Draw(StateEngine* game) {
	

	glPushMatrix();
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camaraflotante.SetGluLookUp();
	dibujoCircuito();
	hudModoCreacion();

	
	glPopMatrix();


}


