#include "StateEngine.h"
#include "State.h"
#include "CreationModeState.h"
#include "Camera.h"
#include "Tramos.h"
#include "OptionsMenuState.h"
#include "MapSelectorState.h"
#include "MainMenuState.h"
#include "Utilidades.h" // Biblioteca de Utilidades
#include "Globals.h"
#include "Entorno.h"
#include <iostream> // Biblioteca de entrada salida
#include <fstream>
#include <sstream> // Biblioteca de manejo de strings
#include <cmath> // Biblioteca matematica de C
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

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

//total de piezas diferentes disponibles
GLint const totalTramos = 7;


//variable para el control del pausado de este estado
GLboolean pausa = false;






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
GLuint textura_carreteraLado;
GLuint textura_mesa;
GLuint texturaSuelo;
GLuint texturaTecho;
GLuint texturaParedposX;
GLuint texturaParednegX;
GLuint texturaParedposZ;
GLuint texturaParednegZ;

//boleanos de click izquierdo/click derecho

BOOLEAN leftclick = false;
BOOLEAN rightclick = false;


//variables de uso durante la pausa
GLint ButtonPausa = 0;
GLuint textura_BotonSeleccionadoPausa;
GLuint textura_BotonSinSeleccionarPausa;

//variables de uso durante la construccion de las piezas
GLfloat LongitudoRadio=5;
GLfloat AnchoCarretera=2;
GLfloat AngulosGrados=10;
GLfloat Inclinacion=1;
GLfloat Separacion=AnchoCarretera;
GLfloat Ondulacion = 2;
GLfloat Potencia = 0.5;
BOOLEAN direccion=true;

/*

Variables de atencion a la lista de tramos que tenenemos actualmente en memoria

*/
glm::mat4 myMatrix(1);
std::vector<Tramo*> vectorTramosEnMemoria;

//variables de uso para la simulacion
std::vector<Point3D> pathSimulacion;
BOOLEAN startSimulacion = false;
BOOLEAN mostrarPath = false;
Point3D posicionActualCoche;
int delay = 250;
int velocidad = 1;
//tiempo en el que inicia la simulacion
int tiempoInicio = 0;

Point3D getPositionAt(int currentTime) {
	Point3D before, after, result,yaw;
	if (!pathSimulacion.empty()) {
		int currentIndex = (currentTime / delay) % pathSimulacion.size();
		before = pathSimulacion[currentIndex];
		after = pathSimulacion[(currentIndex + 1) % pathSimulacion.size()];

		double progress = fmod(((
			(double)currentTime) / (double)delay),
			(double)pathSimulacion.size())
			
			- currentIndex;

		result.x = before.x + progress*(after.x - before.x);
		result.y = before.y + progress*(after.y - before.y);
		result.z = before.z + progress*(after.z - before.z);

	}


	return result;
}


void lookAtLastPiece() {
	Point3D posicion(glm::vec3(myMatrix[3]).x, glm::vec3(myMatrix[3]).y, glm::vec3(myMatrix[3]).z);
	Point3D mirando(glm::vec3(myMatrix[0]).x, glm::vec3(myMatrix[0]).y, glm::vec3(myMatrix[0]).z);
	camaraflotante.LookAtPoint(posicion, 5);
}

void onIdleCreation(){
// Funcion de atencion al evento idle


	posicionActualCoche = getPositionAt(glutGet(GLUT_ELAPSED_TIME)- tiempoInicio);
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
		vectorTramosEnMemoria.push_back(new Tramo(parameters[0], parameters[1], myMatrix, resolucion, repeticionTex));
		myMatrix = vectorTramosEnMemoria.back()->getMatFinal();
		break;
	case 2:
		vectorTramosEnMemoria.push_back(new TramoCurvo(parameters[0], parameters[1], parameters[2], myMatrix, resolucion, repeticionTex));
		myMatrix = vectorTramosEnMemoria.back()->getMatFinal();
		break;
	case 3:
		vectorTramosEnMemoria.push_back(new Rampa(parameters[0], parameters[1], parameters[2], myMatrix,resolucion, repeticionTex));
		myMatrix = vectorTramosEnMemoria.back()->getMatFinal();
		break;
	case 4:
		vectorTramosEnMemoria.push_back(new RampaCurva(parameters[0], parameters[1], parameters[2], parameters[3], myMatrix, resolucion, repeticionTex));
		myMatrix = vectorTramosEnMemoria.back()->getMatFinal();
		break;
	case 5:
		vectorTramosEnMemoria.push_back(new TramoSinuosoHorizontal(parameters[0], parameters[1], parameters[2], myMatrix, parameters[3], parameters[4], resolucion, repeticionTex));
		myMatrix = vectorTramosEnMemoria.back()->getMatFinal();
		break;
	case 6:
		vectorTramosEnMemoria.push_back(new TramoSinuosoVertical(parameters[0], parameters[1], parameters[2], myMatrix,parameters[3], parameters[4], resolucion, repeticionTex));
		myMatrix = vectorTramosEnMemoria.back()->getMatFinal();
		break;
	case 7:
		vectorTramosEnMemoria.push_back(new Looping(parameters[0], parameters[1], parameters[2], myMatrix, resolucion, repeticionTex));
		myMatrix = vectorTramosEnMemoria.back()->getMatFinal();
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
		button = 4;
	}
	else if (button == 5) {
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

}


void vaciarTramosEnMemoria() {
		vectorTramosEnMemoria.clear();
}

/*
Metodos de uso para simular el coche recorriendo el circuito
*/

void rellenaPath() {

	for (int i = 0; i < vectorTramosEnMemoria.size(); i++) {
		
		vectorTramosEnMemoria[i]->calcularPathPoints(pathSimulacion);
	
	}
	pathSimulacion.push_back(Point3D(myMatrix[3].x, myMatrix[3].y, myMatrix[3].z));
}

void vaciaPath() {
	pathSimulacion.clear();
}

void printPath() {

	for (int i = 0; i < pathSimulacion.size(); i++) {

		cout << pathSimulacion[i].x << " , " << pathSimulacion[i].y << " , " << pathSimulacion[i].z << " \n ";

	}
}

void resetVariables() {

	LongitudoRadio = 5;
	// No actualizamos AnchoCarretera para que sea mas sencillo para el usuario continua con el mismo ancho
	AngulosGrados = 90;
	Inclinacion = 1;
	Potencia = 0.5;
	Ondulacion = 2;
	Separacion = AnchoCarretera;
	direccion = true;
}


void añade_tramo(GLint identificador) {
	// sumamos +1 ya que los arrays empiezan por 0
	GLfloat angulo = 0;
	GLfloat onda = 0;
	if (direccion) {
		angulo = -AngulosGrados;
		onda = -Ondulacion;
	}
	else {
		angulo = AngulosGrados;
		onda = Ondulacion;
	}

	//TODO añadir la variables que utilizaré en los globals
	switch (identificador + 1) {
	case 1:

		vectorTramosEnMemoria.push_back(new Tramo(AnchoCarretera, LongitudoRadio, myMatrix,resolucion, repeticionTex));
		myMatrix = vectorTramosEnMemoria.back()->getMatFinal();
		break;
	case 2:
		vectorTramosEnMemoria.push_back(new TramoCurvo(AnchoCarretera, LongitudoRadio, angulo, myMatrix, resolucion, repeticionTex));
		myMatrix = vectorTramosEnMemoria.back()->getMatFinal();
		break;
	case 3:
		vectorTramosEnMemoria.push_back(new Rampa(AnchoCarretera, LongitudoRadio, Inclinacion, myMatrix,resolucion, repeticionTex));
		myMatrix = vectorTramosEnMemoria.back()->getMatFinal();
		break;
	case 4:
		vectorTramosEnMemoria.push_back(new RampaCurva(AnchoCarretera, LongitudoRadio,angulo, Inclinacion, myMatrix,resolucion, repeticionTex));
		myMatrix = vectorTramosEnMemoria.back()->getMatFinal();
		break;
	case 5:
		vectorTramosEnMemoria.push_back(new TramoSinuosoHorizontal(AnchoCarretera, LongitudoRadio, onda, myMatrix, Potencia, direccion, resolucion, repeticionTex));
		myMatrix = vectorTramosEnMemoria.back()->getMatFinal();
		break;
	case 6:
		vectorTramosEnMemoria.push_back(new TramoSinuosoVertical(AnchoCarretera, LongitudoRadio, onda, myMatrix, Potencia, direccion, resolucion, repeticionTex));
		myMatrix = vectorTramosEnMemoria.back()->getMatFinal();
		break;
	case 7:
		GLfloat separacion;
		if (direccion) {
			separacion = -Separacion;
		}
		else {
			separacion = Separacion;
		}
		vectorTramosEnMemoria.push_back(new Looping(AnchoCarretera, separacion, LongitudoRadio, myMatrix,resolucion, repeticionTex));
		myMatrix = vectorTramosEnMemoria.back()->getMatFinal();
		break;
	}
	resetVariables();
}

void dibuja_tramo_actual(GLint identificador) {
	GLfloat angulo = 0;
	GLfloat onda = 0;
	if (direccion) {
		angulo = -AngulosGrados;
		onda = -Ondulacion;
	}
	else {
		angulo = AngulosGrados;
		onda = Ondulacion;
	}

	
	// sumamos +1 ya que los arrays empiezan por 0
	switch (identificador + 1) {
	case 1:
		Tramo(AnchoCarretera, LongitudoRadio, myMatrix, resolucion, repeticionTex).draw(textura_carretera,textura_carreteraLado);
		break;
	case 2:

		TramoCurvo(AnchoCarretera, LongitudoRadio, angulo,myMatrix, resolucion, repeticionTex).draw(textura_carretera, textura_carreteraLado);
		break;
	case 3:
		Rampa(AnchoCarretera, LongitudoRadio, Inclinacion, myMatrix,resolucion, repeticionTex).draw(textura_carretera, textura_carreteraLado);
		break;
	case 4:
		RampaCurva(AnchoCarretera, LongitudoRadio, angulo, Inclinacion, myMatrix,resolucion, repeticionTex).draw(textura_carretera, textura_carreteraLado);
		break;
	case 5:
		TramoSinuosoHorizontal(AnchoCarretera, LongitudoRadio, onda, myMatrix, Potencia, direccion, resolucion, repeticionTex).draw(textura_carretera, textura_carreteraLado);
		break;
	case 6:
		TramoSinuosoVertical(AnchoCarretera, LongitudoRadio, onda, myMatrix, Potencia, direccion, resolucion, repeticionTex).draw(textura_carretera, textura_carreteraLado);
		break;
	case 7:
		GLfloat separacion;
		if (direccion) {
			separacion = -Separacion;
		}
		else {
			separacion = Separacion;
		}
		Looping(AnchoCarretera, separacion, LongitudoRadio, myMatrix, resolucion, repeticionTex).draw(textura_carretera, textura_carreteraLado);
		break;
	}
}



void onKeySimulacion(unsigned char tecla, int x, int y)
// Funcion de atencion al teclado
{

	if (startSimulacion) {
		switch (tecla) {
		case 'W':
		case 'w':
			delay -= 10;
			if (delay < 1) {
				delay = 1;
			}

			break;

		case 'S':
		case 's':
			delay += 10;
			if (delay > 1000) {
				delay = 1000;
			}
			break;
		case 'M':
		case 'm':
			mostrarPath = !mostrarPath;
			break;
		case 27: // Pulso escape
			ButtonPausa = 0;
			CreationModeState::Instance()->Pause();
			break;

		}
	}
}

void InitSimulation() {
	rellenaPath();
	printPath();
	glutSpecialFunc(NULL);// Alta de la funcion de atencion al teclado especial
	glutKeyboardFunc(onKeySimulacion);// Alta de la funcion de atencion al teclado 
	startSimulacion = true;
}

void ResumeSimulation() {
	glutSpecialFunc(NULL);// Alta de la funcion de atencion al teclado especial
	glutKeyboardFunc(onKeySimulacion);// Alta de la funcion de atencion al teclado 
	pausa = false;
}

void onKeyPausa(unsigned char tecla, int x, int y)
// Funcion de atencion al teclado
{

	switch (tecla) {
	case 13://se pulsa enter
		switch (ButtonPausa)
		{
		case 0: //comenzamos la simulacion
			if (!startSimulacion) {
				InitSimulation();
				pausa = false;
			}

			else {
				pausa = false;
				glutSpecialFunc(NULL);// Alta de la funcion de atencion al teclado especial
				glutKeyboardFunc(onKeySimulacion);// Alta de la funcion de atencion al teclado 
			}
			tiempoInicio = glutGet(GLUT_ELAPSED_TIME);
			break;
		case 1: //Reanudamos el modo creacion
			CreationModeState::Instance()->Resume();
			vaciaPath();
			break;
		case 2: // Seleccionamos otro mapa
			pausa = false;
			vaciaPath();
			startSimulacion = false;
			engineCreation->PushState(MapSelectorState::Instance());
			break;
		case 3: // entramos a las opciones
			pausa = false;
			engineCreation->PushState(OptionsMenuState::Instance());
			break;
		case 4: //guardamos y salimos
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
		if (!startSimulacion) {
			CreationModeState::Instance()->Resume();
		}
		else {
			ResumeSimulation();
		}
	
		break;
	}
}

void onKeyCreacion(unsigned char tecla, int x, int y)
// Funcion de atencion al teclado
{

	if (startSimulacion) {
		switch (tecla) {
		case 'W':
		case 'w':
			delay -= 1;
			if (delay == 1) {
				delay = 1;
			}

			break;

		case 'S':
		case 's':
			delay += 1;
			if (delay == 1000) {
				delay = 1000;
			}
			break;
		case 'M':
		case 'm':
			mostrarPath = !mostrarPath;
			break;
		case 27: // Pulso escape
			ButtonPausa = 0;
			CreationModeState::Instance()->Pause();
			//damos de alta a las funciones de escucha del menu
			glutSpecialFunc(onSpecialKeyPausa);
			glutKeyboardFunc(onKeyPausa);
			break;

		}
	}
	else {

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
			//enfocamos a la ultima pieza colocad

			lookAtLastPiece();
			break;
		case 13:
			if (seleccionado)
			{
				//si ya se habia seleccionado una pieza la dibujamos
				añade_tramo(tramoactual);

			}
			else
			{

			}
			seleccionado = !seleccionado;
			break;
		case 8: //se pulsa retroceso.
			if (seleccionado) // si una pieza esta seleccionada la deseleccionamos
				seleccionado = !seleccionado;
			else {//si no tenenmos pieza seleccionada la eliminamos, TODO añadir mensaje de confirmacion.
				if (!vectorTramosEnMemoria.empty()) {
					vectorTramosEnMemoria.pop_back();
					if (!vectorTramosEnMemoria.empty())
						myMatrix = vectorTramosEnMemoria.back()->getMatFinal();
				}
				else {
					// si el stack de piezas esta vacio reseteamos la matriz de transformaciones a la matriz unidad
					myMatrix = glm::mat4(1);
				}
			}
			break;
		case 27: // Pulso escape
			ButtonPausa = 0;
			CreationModeState::Instance()->Pause();
			//damos de alta a las funciones de escucha del menu
			glutSpecialFunc(onSpecialKeyPausa);
			glutKeyboardFunc(onKeyPausa);
			break;
		}



		if (seleccionado) {
			switch (tecla) {
			case 't':
			case 'T':
				LongitudoRadio += 0.1;
				if (LongitudoRadio > 10) {
					LongitudoRadio = 10;
				}
				break;
			case 'g':
			case 'G':
				LongitudoRadio -= 0.1;
				if (LongitudoRadio < 1) {
					LongitudoRadio = 1;
				}
				break;
			case 'r':
			case 'R':
				direccion = !direccion;
				break;
			case 'f':
			case 'F':
				AnchoCarretera -= 0.1;
				if (AnchoCarretera < 1) {
					AnchoCarretera = 1;
				}

				break;
			case 'h':
			case 'H':
				AnchoCarretera += 0.1;
				if (AnchoCarretera > 10)
					AnchoCarretera = 10;
				break;
			case 'j':
			case 'J':
				AngulosGrados -= 0.5;
				if (AngulosGrados < 0) {
					AngulosGrados = 0;
				}

				Ondulacion -= 0.2;
				if (Ondulacion < 0) {
					Ondulacion = 0;
				}
				break;
			case 'k':
			case 'K':
				AngulosGrados += 0.5;
				// no es valido que el angulo sea mayor de 359
				if (AngulosGrados > 359) {
					AngulosGrados = 359;
				}

				Ondulacion += 0.2;
				if (Ondulacion > 30) {
					Ondulacion = 30;
				}
				break;
			case 'u':
			case 'U':
				Inclinacion -= 0.1;
				if (Inclinacion < -1.5) {
					Inclinacion = -1.5;
				}
				break;
			case 'i':
			case 'I':
				Inclinacion += 0.1;
				if (Inclinacion > 1.5) {
					Inclinacion = 1.5;
				}
				break;
			case 'o':
			case 'O':
				Separacion -= 0.1;
				if (Separacion < AnchoCarretera)
					Separacion = AnchoCarretera;

				Potencia -= 0.1;
				if (Potencia < 0) {
					Potencia = 0;
				}

				break;
			case 'p':
			case 'P':
				Separacion += 0.1;
				if (Separacion > 20)
					Separacion = 20;

				Potencia -= 0.1;
				if (Potencia >3) {
					Potencia = 3;
				}
				break;
			}
		}
	}

}


void CreationModeState::Init(StateEngine* engine) {
	myMatrix = glm::mat4(1);
	vaciarTramosEnMemoria();
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

	glm::vec3 posicion_inicial = { 0,5,10 };
	camaraflotante = Camera(posicion_inicial);

	pausa = false;
	iluminacion();

	//Texturas
	inicializarTextura(textura_carretera, "./textures/carretera.jpg");
	inicializarTextura(textura_carreteraLado, "./textures/carreteralado.jpg");
	inicializarTextura(textura_fondo, "./textures/fondo.jpg");
	inicializarTextura(textura_nukacola, "./textures/nukacola.jpg");
	inicializarTextura(textura_metal, "./textures/metal.jpg");
	inicializarTextura(textura_slurm, "./textures/slurm.jpg");
	inicializarTextura(textura_sonic, "./textures/sonic.jpg");
	inicializarTextura(textura_hierba, "./textures/hierba.jpg");
	inicializarTextura(textura_panel_trasero, "./textures/paneltrasero.jpg");
	inicializarTextura(textura_cielonoche, "./textures/cielonoche.jpg");
	inicializarTextura(textura_cielodia, "./textures/cielodia.jpg");
	inicializarTextura(textura_victoria, "./textures/victory.PNG");
	inicializarTextura(textura_velocimetro, "./textures/velocimetro.PNG");
	inicializarTextura(textura_aguja, "./textures/aguja.jpg");
	inicializarTextura(textura_bandera1, "./textures/bandera1.PNG");
	inicializarTextura(textura_bandera2, "./textures/bandera2.PNG");
	inicializarTextura(textura_bandera3, "./textures/bandera3.PNG");
	inicializarTextura(textura_goal, "./textures/goal.PNG");
	inicializarTextura(textura_start, "./textures/start.PNG");
	inicializarTextura(textura_coche, "./textures/coche.PNG");
	inicializarTextura(textura_BotonSeleccionadoPausa, "./textures/ButtonSelected.jpg");
	inicializarTextura(textura_BotonSinSeleccionarPausa, "./textures/ButtonNotSelected.jpg");
	inicializarTextura(textura_mesa, "./textures/madera.png");
	inicializarTextura(texturaSuelo, "./textures/room/negy.dds");
	inicializarTextura(texturaTecho, "./textures/room/posy.dds");
	inicializarTextura(texturaParedposX, "./textures/room/posx.dds");
	inicializarTextura(texturaParednegX, "./textures/room/negx.dds");
	inicializarTextura(texturaParedposZ, "./textures/room/posz.dds");
	inicializarTextura(texturaParednegZ, "./textures/room/negz.dds");
	
}

//Limpiar texturas etc
void CreationModeState::Cleanup() {
	glutSpecialFunc(NULL);
	glutKeyboardFunc(NULL);
	//glutIdleFunc(NULL);
	glutMouseFunc(NULL);
	vaciaPath();
	vaciarTramosEnMemoria();
}

void updateRes() {
	for (int i = 0; i < vectorTramosEnMemoria.size(); i++) {
		vectorTramosEnMemoria[i]->setRes(resolucion);
	}
}

void CreationModeState::Resume() {
	pausa = false;
	startSimulacion = false;
	glutSpecialFunc(onSpecialKeyModoCreacion);// Alta de la funcion de atencion al teclado especial
	glutKeyboardFunc(onKeyCreacion);// Alta de la funcion de atencion al teclado 
	glutIdleFunc(onIdleCreation); // Alta de la funcion de atencion a idle
	glutMouseFunc(mouse);//Alta de la funcion de atencion a los botones del raton
	if (!vectorTramosEnMemoria.empty()) {
		myMatrix = vectorTramosEnMemoria.back()->getMatFinal();
	}
	else {
		myMatrix = glm::mat4(1);
	}
	
	updateRes();
	cout << glm::to_string(myMatrix);
}

void CreationModeState::Pause() {
	pausa = true;
	//desactivamos las funciones de escucha
	glutSpecialFunc(NULL);
	glutKeyboardFunc(NULL);
	//glutIdleFunc(NULL); 
	glutMouseFunc(NULL);

	glutSpecialFunc(onSpecialKeyPausa);
	glutKeyboardFunc(onKeyPausa);
}


void CreationModeState::HandleEvents(StateEngine* game) {

}


void CreationModeState::Update(StateEngine* game) {

}

void texto_tramo_actual(GLint tramo) {

	switch (tramo + 1) {
	case 1:
		textoStroke(-1, 0.05, 0.2, "f/h: aumentar/disminuir ancho", 0.04, 0.04, 0.04, ROJO, GLUT_STROKE_ROMAN);
		textoStroke(-1, 0.0, 0.2, "g/t: aumentar/disminuir longitud", 0.04, 0.04, 0.04, ROJO, GLUT_STROKE_ROMAN);
		break;
	case 2:
		textoStroke(-1, 0.1, 0.2, "f/h: aumentar/disminuir ancho", 0.04, 0.04, 0.04, ROJO, GLUT_STROKE_ROMAN);
		textoStroke(-1, 0.05, 0.2, "g/t: aumentar/disminuir longitud", 0.04, 0.04, 0.04, ROJO, GLUT_STROKE_ROMAN);
		textoStroke(-1, 0.0, 0.2, "j/k: aumentar/disminuir curvatura", 0.04, 0.04, 0.04, ROJO, GLUT_STROKE_ROMAN);
		textoStroke(-1, -0.05, 0.2, "r: rotar", 0.04, 0.04, 0.04, ROJO, GLUT_STROKE_ROMAN);
		break;
	case 3:
		textoStroke(-1, 0.1, 0.2, "f/h: aumentar/disminuir ancho", 0.04, 0.04, 0.04, ROJO, GLUT_STROKE_ROMAN);
		textoStroke(-1, 0.05, 0.2, "g/t: aumentar/disminuir longitud", 0.04, 0.04, 0.04, ROJO, GLUT_STROKE_ROMAN);
		textoStroke(-1, 0.0, 0.2, "u/i: aumentar/disminuir inclinacion", 0.04, 0.04, 0.04, ROJO, GLUT_STROKE_ROMAN);
		break;
	case 4:
		textoStroke(-1, 0.1, 0.2, "f/h: aumentar/disminuir ancho", 0.04, 0.04, 0.04, ROJO, GLUT_STROKE_ROMAN);
		textoStroke(-1, 0.05, 0.2, "g/t: aumentar/disminuir longitud", 0.04, 0.04, 0.04, ROJO, GLUT_STROKE_ROMAN);
		textoStroke(-1, 0.0, 0.2, "j/k: aumentar/disminuir curvatura", 0.04, 0.04, 0.04, ROJO, GLUT_STROKE_ROMAN);
		textoStroke(-1, -0.05, 0.2, "r: rotar", 0.04, 0.04, 0.04, ROJO, GLUT_STROKE_ROMAN);
		textoStroke(-1, -0.1, 0.2, "u/i: aumentar/disminuir inclinacion", 0.04, 0.04, 0.04, ROJO, GLUT_STROKE_ROMAN);
		break;
	case 5:
		textoStroke(-1, 0.1, 0.2, "f/h: aumentar/disminuir ancho", 0.04, 0.04, 0.04, ROJO, GLUT_STROKE_ROMAN);
		textoStroke(-1, 0.05, 0.2, "g/t: aumentar/disminuir longitud", 0.04, 0.04, 0.04, ROJO, GLUT_STROKE_ROMAN);
		textoStroke(-1, 0.0, 0.2, "j/k: aumentar/disminuir ondulacion", 0.04, 0.04, 0.04, ROJO, GLUT_STROKE_ROMAN);
		textoStroke(-1, -0.05, 0.2, "r: rotar", 0.04, 0.04, 0.04, ROJO, GLUT_STROKE_ROMAN);
		textoStroke(-1, -0.1, 0.2, "o/p: aumentar/disminuir numero ondas", 0.04, 0.04, 0.04, ROJO, GLUT_STROKE_ROMAN);
		break;
	case 6:
		textoStroke(-1, 0.1, 0.2, "f/h: aumentar/disminuir ancho", 0.04, 0.04, 0.04, ROJO, GLUT_STROKE_ROMAN);
		textoStroke(-1, 0.05, 0.2, "g/t: aumentar/disminuir longitud", 0.04, 0.04, 0.04, ROJO, GLUT_STROKE_ROMAN);
		textoStroke(-1, 0.0, 0.2, "j/k: aumentar/disminuir ondulacion", 0.04, 0.04, 0.04, ROJO, GLUT_STROKE_ROMAN);
		textoStroke(-1, -0.05, 0.2, "r: rotar", 0.04, 0.04, 0.04, ROJO, GLUT_STROKE_ROMAN);
		textoStroke(-1, -0.1, 0.2, "o/p: aumentar/disminuir numero ondas", 0.04, 0.04, 0.04, ROJO, GLUT_STROKE_ROMAN);
		break;
	case 7:
		textoStroke(-1, 0.1, 0.2, "f/h: aumentar/disminuir ancho", 0.04, 0.04, 0.04, ROJO, GLUT_STROKE_ROMAN);
		textoStroke(-1, 0.05, 0.2, "g/t: aumentar/disminuir radio", 0.04, 0.04, 0.04, ROJO, GLUT_STROKE_ROMAN);
		textoStroke(-1, 0.0, 0.2, "o/p: aumentar/disminuir separacion", 0.04, 0.04, 0.04, ROJO, GLUT_STROKE_ROMAN);
		
		break;
		//TODO añadir nuevas piezas conforme las vaya haciendo
	}
}


void dibujarTramosEnLista() {

	for (int i = 0; i < vectorTramosEnMemoria.size(); i++) {
		vectorTramosEnMemoria[i]->drawing(textura_carretera,textura_carreteraLado);
	}
}


void dibujarCircuitoEnMemoria()
{

	glPushMatrix();
	{
		dibujarTramosEnLista();

		if (seleccionado) {
			dibuja_tramo_actual(tramoactual);
		}


	}
	glPopMatrix();

}

void dibuja_tramo_HUD(GLint identificador) {
	// sumamos +1 ya que los arrays empiezan por 0
	switch (identificador + 1) {
	case 1:
		glRotatef(90, 1, 0, 0);
		glRotatef(90, 0, 1, 0);
		glRotatef(rotacion_pieza, 1, 0, 0);
		Tramo(0.05, 0.15,glm::mat4(1), resolucion, repeticionTex).draw(textura_carretera, textura_carreteraLado);
		break;
	case 2:
		glRotatef(90, 1, 0, 0);
		glRotatef(90, 0, 1, 0);
		glRotatef(rotacion_pieza, 1, 0, 0);
		TramoCurvo(0.05, 0.15, 90, glm::mat4(1), resolucion, repeticionTex).draw(textura_carretera, textura_carreteraLado);
		break;
	case 3:
		glRotatef(90, 1, 0, 0);
		glRotatef(90, 0, 1, 0);
		glRotatef(rotacion_pieza, 1, 0, 0);
		Rampa(0.05, 0.10, 0.5, glm::mat4(1), resolucion, repeticionTex).draw(textura_carretera, textura_carreteraLado);
		break;
	case 4:
		glRotatef(90, 1, 0, 0);
		glRotatef(90, 0, 1, 0);
		glRotatef(rotacion_pieza, 1, 0, 0);
		RampaCurva(0.05, 0.08, 90, 10, glm::mat4(1), resolucion, repeticionTex).draw(textura_carretera, textura_carreteraLado);
		break;
	case 5:
		glRotatef(90, 1, 0, 0);
		glRotatef(90, 0, 1, 0);
		glRotatef(rotacion_pieza, 1, 0, 0);
		glScalef(0.01, 0.01, 0.01);
		TramoSinuosoHorizontal(5, 15, 2, glm::mat4(1), 2, true, resolucion, repeticionTex).draw(textura_carretera, textura_carreteraLado);
		break;
	case 6:
		glRotatef(90, 1, 0, 0);
		glRotatef(90, 0, 1, 0);
		glRotatef(rotacion_pieza, 1, 0, 0);
		glScalef(0.01, 0.01, 0.01);
		TramoSinuosoVertical(5, 15, 2, glm::mat4(1), 2, true, resolucion, repeticionTex).draw(textura_carretera, textura_carreteraLado);
		break;
	case 7:
		//glRotatef(-180, 1, 0, 0);
		glRotatef(90, 0, 1, 0);
		glRotatef(rotacion_pieza, 0, 1, 0);
		Looping(0.05, 0.06, 0.08, glm::mat4(1), resolucion, repeticionTex).draw(textura_carretera, textura_carreteraLado);
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

	Point3D v0(-0.6, 0.5, 0.0);
	Point3D v1(0.6, 0.5, 0.0);
	Point3D v3(-0.6, 0.3, 0.0);
	Point3D v2(0.6, 0.3, 0.0);


	quadtex(v0, v1, v2, v3,
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
	
	v0[1] = 0.2;
	v1[1] = 0.2;
	v2[1] = 0.0;
	v3[1] = 0.0;

	quadtex(v0, v1, v2, v3,
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

	v0[1] = -0.1;
	v1[1] = -0.1;
	v2[1] = -0.3;
	v3[1] = -0.3;

	quadtex(v0, v1, v2, v3,
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

	v0[1] = -0.4;
	v1[1] = -0.4;
	v2[1] = -0.6;
	v3[1] = -0.6;

	quadtex(v0, v1, v2, v3,
		0, 1, 0, 1, 1, 1);

	if (ButtonPausa == 4) {
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

	quadtex(v0, v1, v2, v3,
		0, 1, 0, 1, 1, 1);


}


void textosBotonesPausa() {

	if (ButtonPausa == 0)
		textoStroke(-0.18, 0.37, 0.2, "Simulacion", 0.08, 0.08, 0.08, AMARILLO, GLUT_STROKE_ROMAN);
	else
		textoStroke(-0.18, 0.37, 0.2, "Simulacion", 0.08, 0.08, 0.08, BLANCO, GLUT_STROKE_ROMAN);


	if (ButtonPausa == 1)
		textoStroke(-0.18, 0.07, 0.2, "Reanudar", 0.08, 0.08, 0.08, AMARILLO, GLUT_STROKE_ROMAN);
	else
		textoStroke(-0.18, 0.07, 0.2, "Reanudar", 0.08, 0.08, 0.08, BLANCO, GLUT_STROKE_ROMAN);

	if (ButtonPausa == 2)
		textoStroke(-0.27, -0.23, 0.2, "Cargar Circuito", 0.08, 0.08, 0.08, AMARILLO, GLUT_STROKE_ROMAN);
	else
		textoStroke(-0.27, -0.23, 0.2, "Cargar Circuito", 0.08, 0.08, 0.08, BLANCO, GLUT_STROKE_ROMAN);

	if (ButtonPausa == 3)
		textoStroke(-0.18, -0.53, 0.2, "Opciones", 0.08, 0.08, 0.08, AMARILLO, GLUT_STROKE_ROMAN);
	else
		textoStroke(-0.18, -0.53, 0.2, "Opciones", 0.08, 0.08, 0.08, BLANCO, GLUT_STROKE_ROMAN);

	if (ButtonPausa == 4)
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

void texto_simulacion() {
	char *a = "velocidad: ";
	char buf[8];
	sprintf(buf, "%d", (1/delay)*1000);
	char result[100];   
	strcpy(result, a); 
	strcat(result, buf);

	textoStroke(-0.4, 0.05, 0.2, result, 0.04, 0.04, 0.04, ROJO, GLUT_STROKE_ROMAN);
	textoStroke(-1, 0.05, 0.2, "W/S: aumentar/disminuir velocidad", 0.04, 0.04, 0.04, ROJO, GLUT_STROKE_ROMAN);
	textoStroke(-1, 0.00, 0.2, "M: mostrar/ocultar path", 0.04, 0.04, 0.04, ROJO, GLUT_STROKE_ROMAN);

}

void hudSimulacion() {

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

		texto_simulacion();
	}
	else {
		menuPausa();
	}


	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
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
		texto_tramo_actual(tramoactual);
	}
	else {
		menuPausa();
	}


	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

}

void dibujarPuntosPath() {
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	// Habilitamos blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Z-Buffer
	glDepthMask(GL_FALSE);
	// Dibujar traslucidos

	//la tira donde se colocaran las piezas
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glColor4f(0, 0.8, 1, 1);
	for (int i = 0; i < pathSimulacion.size(); i++) {
		glPushMatrix();
		glTranslatef(pathSimulacion[i].x, pathSimulacion[i].y+0.2, pathSimulacion[i].z);
		glutSolidSphere(0.2, 10, 10);
		glPopMatrix();
	}

	glPopAttrib();
}

void dibujoCircuito() {
	// Borra buffers y selecciona modelview
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (startSimulacion) {
		camaraflotante.LookAtPoint(posicionActualCoche, 5);
	}

	camaraflotante.SetGluLookUp();
	glPolygonMode(GL_FRONT, GL_LINE);

	Entorno(resolucion,1).draw(textura_mesa,200,150,20,texturaSuelo,texturaParednegX,texturaParedposX,texturaParednegZ,texturaParedposZ,texturaTecho);
	dibujarCircuitoEnMemoria();
	if (startSimulacion) {
		if (mostrarPath) {
			dibujarPuntosPath();
		}
	
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		// Habilitamos blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// Z-Buffer
		glDepthMask(GL_FALSE);
		// Dibujar traslucidos

		//la tira donde se colocaran las piezas
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor4f(1, 0, 0, 1);

		glTranslatef(posicionActualCoche.x, posicionActualCoche.y, posicionActualCoche.z);
		glutSolidSphere(0.4, 10, 10);

		glPopAttrib();
	}
	

}



//TODO completar esta mision
void CreationModeState::Draw(StateEngine* game) {
	

	glPushMatrix();
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	dibujoCircuito();
	if (startSimulacion) {
		hudSimulacion();
	}
	else {
		hudModoCreacion();
	}


	
	glPopMatrix();


}


