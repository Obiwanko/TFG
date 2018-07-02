#include <iostream> // Biblioteca de entrada salida
#include <fstream>
#include <sstream> // Biblioteca de manejo de strings
#include <cmath> // Biblioteca matematica de C

#include "Utilidades.h" // Biblioteca de Utilidades
#include "Globals.h" // Encabezado donde se quedaran definidas las variables globales.
#include "MainMenuState.h" //Encabezado donde queda definida la clase del menu principal
#include <glm/glm.hpp>


using namespace std;
//Para hacer el screenshot
GLint ancho;
GLint alto;

//declaraciones de las variables globales
 string saveFolder = "./savefiles/";
 string textureFolder = "./textures/";


 GLint resolucion = 0;
 GLint repeticionTex = 0;
 GLboolean fullscreen = false;
 GLint graficos = 1;


//Activadores de modos

BOOLEAN test = true; //Variable para el control del testeo de niveles.
BOOLEAN menu = true;
BOOLEAN creacion = true; //Variable para controlar el estado de creacion
BOOLEAN alambricoSimple = false;
BOOLEAN diurnoNocturno =false;
BOOLEAN niebla = false;
BOOLEAN camaraLibre = true; //por defecto el modo construccion esta activado, TODO una vez implementado el manejador de estados modificar.
BOOLEAN hud = false;
BOOLEAN terminado = false;

//BOLEANOS para el juego
BOOLEAN dentro = FALSE;
BOOLEAN trampas = TRUE;


//CLASE manejadora de estados
StateEngine engine;



void reshape(GLint w, GLint h)
// Funcion de atencion al redimensionamiento
{
	ancho = w;
	alto = h;
	// Usamos toda el area de dibujo
	glViewport(0, 0, w, h);
	// Definimos la camara (matriz de proyeccion)
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float razon = (float)w / h;
	/* CAMARA PERSPECTIVA */
	gluPerspective(120, razon, 0.1, 510);
}


void onIdle()
// Funcion de atencion al evento idle
{



	static int antesc = 0;
	int ahorac, tiempo_transcurridoc;

	ahorac = glutGet(GLUT_ELAPSED_TIME); //Tiempo transcurrido desde el inicio
	tiempo_transcurridoc = ahorac - antesc; //Tiempo transcurrido desde antes en msg.
										
	
	/* Codigo  ya no utilizado
	rotacion_pieza += float((0.036f*float(tiempo_transcurridoc)));
	//Calculamos el tiempo transcurrido desde la última vez
	static int antess = 0;
	static int cont = 0;
	int ahoras, tiempo_transcurridos;

	ahoras = glutGet(GLUT_ELAPSED_TIME); //Tiempo transcurrido desde el inicio
	tiempo_transcurridos = ahoras - antess; //Tiempo transcurrido desde antes en msg.
	if (terminado) {

	if (tiempo_transcurridos >= 1000) {
	cont += 1;
	if (cont == 10) {
	exit(0);
	}
	antess = ahoras;
	}

	}




	desplazamientoz += moduloVelocidad * cos(direccionVelocidad*aumentovelocidadgiro*3.1415926 / 180)*tiempo_transcurridoc / 1000;
	desplazamientox += moduloVelocidad * sin(direccionVelocidad*aumentovelocidadgiro*3.1415926 / 180)*tiempo_transcurridoc / 1000;
	//a donde mira el coche/camara
	mirandoz = desplazamientoz + 20 * cos(direccionVelocidad*aumentovelocidadgiro*3.1415926 / 180);
	mirandox = desplazamientox + 20 * sin(direccionVelocidad*aumentovelocidadgiro*3.1415926 / 180);
	mirandozaerea = desplazamientoz + 1 * cos(direccionVelocidad*aumentovelocidadgiro*3.1415926 / 180);
	mirandoxaerea = desplazamientox + 1 * sin(direccionVelocidad*aumentovelocidadgiro*3.1415926 / 180);
	//direccion del foco normalizada
	focoz = cos(direccionVelocidad*aumentovelocidadgiro*3.1415926 / 180);
	focox = sin(direccionVelocidad*aumentovelocidadgiro*3.1415926 / 180);

	eventoMeta();
	if (!terminado) {
		antess = ahoras;
	}
	antesc = ahorac;

	*/
	glutPostRedisplay();
}

//funcion de display donde se llama a la funcion de dibujado del estado en la cima de la pila
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//borramos buffers
	engine.Draw();
	glutSwapBuffers(); // Intercambia los buffers
}

//simple switch para establecer la calidad grafica
void setResolucion(int v) {

	switch (v)
	{
	case 1: //low settings
		resolucion = 10;
		repeticionTex = 1;
		graficos = 1;
		break;
	case 2: //medium settings
		resolucion = 20;
		repeticionTex = 1;
		graficos = 2;
		break;
	case 3:
		resolucion = 30;
		repeticionTex = 1;
		graficos = 3;
		break;
	default:
		break;
	}
}
//simple switch para establecer la pantalla completa
void setFullScreen(int f) {
	switch (f)
	{
	case 0: //sin ser full screnn
		fullscreen = false;
		break;
	case 1: //fullscreen
		fullscreen = true;
		break;

	default:
		break;
	}
}
//funcion utilizada para cargar el fichero de opciones y establecer las opciones en memoria
void cargarOpciones() {
	std::ifstream i(saveFolder+"options/options.txt");

	if (!i.fail()) {
		auto line = std::string{};
		int a = 0;
		while (std::getline(i, line)) {
			auto pos = line.find_first_of(';');
			string variable = line.substr(0, pos);
			if (a == 0) {
				setResolucion(stoi(variable));
			}
			if (a == 1) {
				setFullScreen(stoi(variable));
			}
			a++;
		}
	}
}

void main(int argc, char** argv)
// Programa principal
{
	FreeImage_Initialise();
	glutInit(&argc, argv); // Inicializacion de GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // Alta de buffers a usar
	cargarOpciones();
	

	engine.Init("Road Maker 2018",800,600,fullscreen);
	
	engine.ChangeState(MainMenuState::Instance());
	glutDisplayFunc(display);

	glutReshapeFunc(reshape); // Alta de la funcion de atencion a reshape

	glutIdleFunc(onIdle); // Alta de la funcion de atencion a idle


	glutMainLoop(); // Puesta en marcha del programa

	FreeImage_DeInitialise();

}