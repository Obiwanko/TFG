#include <iostream> // Biblioteca de entrada salida
#include <fstream>
#include <sstream> // Biblioteca de manejo de strings
#include <cmath> // Biblioteca matematica de C

#include "Utilidades.h" // Biblioteca de Utilidades
#include "Globals.h" // Encabezado donde se quedaran definidas las variables globales.
#include "MainMenuState.h" //Encabezado donde queda definida la clase del menu principal
#include <glm/glm.hpp>

using namespace std;
//Para hace el screenshot
GLint ancho;
GLint alto;

GLfloat moduloVelocidad = 0.0;
GLfloat rotacion_velocidad = 0.0;
GLfloat direccionVelocidad = 0.0;
GLfloat desplazamientoz = 0.0;
GLfloat desplazamientox = 0.0;
GLfloat mirandoz = 0;
GLfloat mirandox = 0;
GLfloat mirandozaerea = 0;
GLfloat mirandoxaerea = 0;
GLfloat focoz = 0;
GLfloat focox = 0;
GLint vueltas = 0;//contador vueltas
GLint nivel = 1;

GLfloat aumentovelocidadgiro = 10;// lo uso para probar ya que la velocidad de giro que se da en el problema es ridiculamente baja
			

//declaraciones de las variables globales
 string saveFolder = "./savefiles/";
 string textureFolder = "./textures/";


 GLint resolucion = 0;
 GLint repeticionTex = 0;
 GLboolean fullscreen = false;
 GLint graficos = 1;

//Identificadores listas poligonos
GLint circuito1;
GLint circuito2;
GLint circuitomini1;
GLint circuitomini2;
GLint cilindro;
GLint cielo_dia;
GLint cielo_noche;
GLint anuncio_nuka;
GLint anuncio_slurm;
GLint anuncio_sonic;
GLint meta;
GLint hierba;
GLint aguja;
GLint recta;





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

//Variables globales

//CLASE manejadora de estados
StateEngine engine;

//vector donde se guardaran las piezas colocadas

//Declaracion variable de la posicion del ultimo tramo.
Point3D posicionUltimoTramo = Point3D(0,0,0);


/*
Funcion que dibujara N*M quads dentro de unos vertices dados
Importante pasar los vertices en el orden siguiente:
v0------v1
|		 |
|		 |
v3------v2
*/
void quads(GLfloat v0[], GLfloat v1[], GLfloat v2[], GLfloat v3[], GLint N, GLint M) {

	//Calculamos las diferencias entre vertices verticales para ir subiendo niveles con j
	GLfloat diffXverticalIz = (v0[0] - v3[0]) / M;
	GLfloat diffZerticalIz = (v0[1] - v3[1]) / M;

	GLfloat diffXverticalDer = (v1[0] - v2[0]) / M;
	GLfloat diffZverticalDer = (v1[1] - v2[1]) / M;


	for (int j = 0; j < M; j++) {
		//diferencia horizontal actual
		GLfloat diffxhorizontal0 = ((v0[0] - (diffXverticalIz*j)) - (v1[0] - (diffXverticalDer*j))) / N;
		GLfloat diffzhorizontal0 = ((v0[1] - (diffZerticalIz*j)) - (v1[1] - (diffZverticalDer*j))) / N;
		//diferencia horizontal siguientej
		GLfloat diffxhorizontal1 = ((v0[0] - (diffXverticalIz*(j + 1))) - (v1[0] - (diffXverticalDer*(j + 1)))) / N;
		GLfloat diffzhorizontal1 = ((v0[1] - (diffZerticalIz*(j + 1))) - (v1[1] - (diffZverticalDer*(j + 1)))) / N;
		//punto x,z actual j
		GLfloat Xactual = v0[0] - (diffXverticalIz*j);
		GLfloat Zactual = v0[1] - (diffZerticalIz*j);
		//punto x,z siguiente j
		GLfloat Xsiguiente = v0[0] - (diffXverticalIz*(j + 1));
		GLfloat Zsiguiente = v0[1] - (diffZerticalIz*(j + 1));
		for (int i = 0; i < N; i++) {
			glBegin(GL_QUAD_STRIP);
			glNormal3f(0.0, 1.0, 0.0);
			glVertex3f(Xactual - diffxhorizontal0*i, 0, Zactual - diffzhorizontal0*i);
			glVertex3f(Xactual - diffxhorizontal0*(i + 1), 0, Zactual - diffzhorizontal0*(i + 1));
			glVertex3f(Xsiguiente - diffxhorizontal1*i, 0, Zsiguiente - diffzhorizontal1*i);
			glVertex3f(Xsiguiente - diffxhorizontal1*(i + 1), 0, Zsiguiente - diffzhorizontal1*(i + 1));
			glEnd();
		}
	}
}




void instrucciones() {

	cout << "Reglas:\n El juego terminara una vez hayas dado 3 vueltas completas al circuito\n\nControles:\n Flechas direccionales: Aceleracion y movimiento del vehiculo\n" 
		<< " Barra espaciadora: freno de mano(frenar mas rapido)\n P/p: Captura de pantalla\n G/g: Activa/Desactiva camara aerea\n L/l: Activa/Desactiva iluminacion\n"
		<< " N/n: Activa/Desactiva efecto niebla\n C/c: Activa/Desactiva HUD\n S/s: Cambia modo Alambrico/Texturado\n 1/2: Cambia entre circuitos 1(sencillo) 2(complejo)\n\n Notas:\n Todas las funciones de visualizacion se pueden activar en conjunto salvo alambrico simple la cual desactiva temporalmente las demas\n Al cambiar de mapa se perdera el progreso en el nivel actual y se comenzara desde 0";
}


void init()
// Funcion propia de inicializacion
{

}



//Un sencillo metodo que se encarga de comprobar si ha llegado a la meta o no el vehiculo
void eventoMeta() {
	if (focoz > 0 && desplazamientox >= -2 && desplazamientox <= 2 && desplazamientoz <= 4 && desplazamientoz >= 2 && !dentro && !trampas) {
		vueltas += 1;
		dentro = TRUE;
	}
	else if (focoz > 0 && desplazamientox >= -2 && desplazamientox <= 2 && desplazamientoz > 4 ) {
		dentro = FALSE;
		trampas = FALSE;
	}

	if (focoz < 0 && desplazamientox >= -2 && desplazamientox <= 2 && desplazamientoz <= 4 && desplazamientoz >= 2 && !dentro ) {
		trampas = TRUE;
	}
	if (vueltas == 3) {
		terminado = TRUE;
	}
}

void colocarfarolasyanuncios(GLint nivel) {
	if (nivel == 1) {
		//Colocamos las farolas y les damos la direccion hacia donde apuntan
		GLfloat posicion2[] = { 0.0f, 6.0f, 1.5f, 1.0f };
		glLightfv(GL_LIGHT2, GL_POSITION, posicion2);
		GLfloat dir_centralFarolas[] = { 0.0f, -1.0f, 0.0f };
		glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, dir_centralFarolas);


		GLfloat posicion3[] = { 25.5f, 6.0f, 36.0f, 1.0f };
		glLightfv(GL_LIGHT3, GL_POSITION, posicion3);
		glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, dir_centralFarolas);

		GLfloat posicion4[] = { 40.0f, 6.0f, 10.5f, 1.0f };
		glLightfv(GL_LIGHT4, GL_POSITION, posicion4);
		glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, dir_centralFarolas);

		GLfloat posicion5[] = { 18.5f, 6.0f, -30.0f, 1.0f };
		glLightfv(GL_LIGHT5, GL_POSITION, posicion5);
		glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, dir_centralFarolas);



		//colocamos anuncios
		glPushMatrix();
		glTranslatef(26, 0, 36);
		glRotatef(-90, 0, 1, 0);
		glCallList(anuncio_nuka);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(18, 0, -30);
		glRotatef(90, 0, 1, 0);
		glCallList(anuncio_slurm);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(40, 0, 10);
		glCallList(anuncio_sonic);
		glPopMatrix();

	}
	else {
		//Colocamos las farolas y les damos la direccion hacia donde apuntan
		GLfloat posicion2[] = { 0.0f, 6.0f, 1.5f, 1.0f };
		glLightfv(GL_LIGHT2, GL_POSITION, posicion2);
		GLfloat dir_centralFarolas[] = { 0.0f, -1.0f, 0.0f };
		glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, dir_centralFarolas);


		GLfloat posicion3[] = { 46.5f, 6.0f, 37.0f, 1.0f };
		glLightfv(GL_LIGHT3, GL_POSITION, posicion3);
		glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, dir_centralFarolas);

		GLfloat posicion4[] = {81.0f, 6.0f, 10.5f, 1.0f };
		glLightfv(GL_LIGHT4, GL_POSITION, posicion4);
		glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, dir_centralFarolas);

		GLfloat posicion5[] = { 36.0f, 6.0f, 63.5f, 1.0f };
		glLightfv(GL_LIGHT5, GL_POSITION, posicion5);
		glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, dir_centralFarolas);

		//colocamos anuncios
		glPushMatrix();
		glTranslatef(46, 0, 37);
		glRotatef(90, 0, 1, 0);
		glCallList(anuncio_nuka);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(36, 0, 64);
		glRotatef(-90, 0, 1, 0);
		glCallList(anuncio_slurm);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(81, 0, 10);
		glCallList(anuncio_sonic);
		glPopMatrix();
	}

}

void nieblaComoToca() {
	glEnable(GL_FOG);
	GLfloat colorniebla[] = { 0.2,0.2,0.2,0.8 };

	if (!diurnoNocturno) {
		colorniebla[0] = 0.7;
		colorniebla[1] = 0.7;
		colorniebla[2] = 0.7;
	}

	glFogfv(GL_FOG_COLOR, colorniebla);
	glFogf(GL_FOG_DENSITY, 0.2);

}












void display()
// Funcion de atencion al dibujo
{
	if (test) {


		//else si no dibujaremos el circuito cargado, y aplicaremos las reglas de juego estandar.

	}
	else {


	

	if (menu) {
		//TODO Implementar menu
		/*
	
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(-1, 1, -1, 1, -1.0, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glDisable(GL_CULL_FACE);

		glClear(GL_DEPTH_BUFFER_BIT);
		interfaz();


		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		*/
	}
	else {




		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Borra la pantalla
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		if (alambricoSimple) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDisable(GL_TEXTURE_2D);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glEnable(GL_TEXTURE_2D);
		}


		if (camaraLibre) {//camara aerea para ver el circuito desde arriba
			GLfloat posicionvoladora[] = { 0.0f, -17.3f, 0.0f, 1.0f };
			glLightfv(GL_LIGHT1, GL_POSITION, posicionvoladora);
			gluLookAt(desplazamientox, 18, desplazamientoz,
				mirandoxaerea, 0, mirandozaerea,
				0, 1, 0);
		}
		else {//camara normal del problema
			GLfloat posicion1[] = { 0.0f, -0.3f, 0.0f, 1.0f };
			glLightfv(GL_LIGHT1, GL_POSITION, posicion1);
			gluLookAt(desplazamientox, 1, desplazamientoz,
				mirandox, 0, mirandoz,
				0, 1, 0); // Situa la camara
		}

		colocarfarolasyanuncios(nivel);



		if (!camaraLibre) {
			//Direccion del foco del coche
			GLfloat dir_central[] = { focox, -0.6, focoz };
			glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, dir_central);
		}
		else {
			GLfloat dir_central[] = { focox, -1, focoz };//que apunte directamente al coche
			glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, dir_central);
			//coche();
		}



		glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR); //brillos por separado





		glCallList(cilindro);
		if (diurnoNocturno && !alambricoSimple) {
			glCallList(cielo_noche);
		}
		else {
			glCallList(cielo_dia);
		}

		//dibujamos carretera
		if (nivel == 1) {
			glCallList(circuito1);

		}
		else {
			//menos cuadrados por metro que al ser mas grande gasta mucho
			glCallList(circuito2);

		}
		glCallList(hierba);

		if (diurnoNocturno && !alambricoSimple) {
			glEnable(GL_LIGHTING);
			if (niebla) {
				nieblaComoToca();
			}
		}
		else {
			glDisable(GL_LIGHTING);
			if (niebla) {
				nieblaComoToca();
			}
		}

		if (niebla && !alambricoSimple) {
			nieblaComoToca();
		}
		else {
			glDisable(GL_FOG);
		}

		if (hud && !alambricoSimple) {
			//HUD
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			glOrtho(-1, 1, -1, 1, -1.0, 1);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glDisable(GL_CULL_FACE);

			glClear(GL_DEPTH_BUFFER_BIT);
			//hudJuego();


			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
			glPopMatrix();

		}

	}

}
glutSwapBuffers(); // Intercambia los buffers
}

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




void onSpecialKey(int specialKey, int x, int y) {
	switch (specialKey) {
	case GLUT_KEY_LEFT:
		if (creacion) {

		}
		else {
			direccionVelocidad = direccionVelocidad + 0.25;
		}

		break;
	case GLUT_KEY_RIGHT:
	
		direccionVelocidad = direccionVelocidad - 0.25;

		break;
	case GLUT_KEY_UP:
		moduloVelocidad = moduloVelocidad + 0.1;
		if (moduloVelocidad > 29) {
			moduloVelocidad = 29;
		}
		rotacion_velocidad = moduloVelocidad*10;
		break;
	case GLUT_KEY_DOWN:
		moduloVelocidad = moduloVelocidad - 0.1;
		if (moduloVelocidad < 0) {
			moduloVelocidad = 0;
		}
		rotacion_velocidad = moduloVelocidad*10;
		break;
	}
	stringstream titulo;
	titulo << "Circuito final: " << moduloVelocidad << " m/s";
	glutSetWindowTitle(titulo.str().c_str()); // Pone el titulo
}



void reinicio(){
	 moduloVelocidad = 0.0;
	 rotacion_velocidad = 0.0;
	 direccionVelocidad = 0.0;
	 desplazamientoz = 0.0;
	 desplazamientox = 0.0;
	 mirandoz = 0;
	 mirandox = 0;
	 mirandozaerea = 0;
	 mirandoxaerea = 0;
	 focoz = 0;
	 focox = 0;
	 vueltas = 0;//contador vueltas
	 trampas = true;
	 alambricoSimple = false;
	 diurnoNocturno = false;
	 niebla = false;
	 camaraLibre = false;
	 hud = false;
}


//funcion pasiva de lectura del raton










void onIdle()
// Funcion de atencion al evento idle
{

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


	

	static int antesc = 0;
	int ahorac, tiempo_transcurridoc;

	ahorac = glutGet(GLUT_ELAPSED_TIME); //Tiempo transcurrido desde el inicio
	tiempo_transcurridoc = ahorac - antesc; //Tiempo transcurrido desde antes en msg.
										
	rotacion_pieza += float((0.036f*float(tiempo_transcurridoc)));

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
	glutPostRedisplay();
}


void testo() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//borramos buffers
	engine.Draw();
	glutSwapBuffers(); // Intercambia los buffers
}

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
	//Todo cambiarlo para que vaya por opciones
	cargarOpciones();
	
	//glutInitWindowSize(800, 600); // Tamanyo inicial de la ventana
	//glutCreateWindow("Creador Circuitos");	// Pone el titulo para que se vaya actualizando luego en onkey
	//glutDisplayFunc(display); // Alta de la funcion de atencion a display
	engine.Init("test",800,600,fullscreen);
	
	engine.ChangeState(MainMenuState::Instance());
	glutDisplayFunc(testo);

	glutReshapeFunc(reshape); // Alta de la funcion de atencion a reshape
	
	//glutPassiveMotionFunc(mouseMovement); //Funcion de atencion al raton pasiva siempre a la escucha
	//glutKeyboardFunc(onKeyCreacion); // Alta de la funcion de atencion al teclado
	//glutSpecialFunc(onSpecialKeyModoCreacion);// Alta de la funcion de atencion al teclado especial
	glutIdleFunc(onIdle); // Alta de la funcion de atencion a idle
	init(); // Inicializacion propia

	glutMainLoop(); // Puesta en marcha del programa

	FreeImage_DeInitialise();

}