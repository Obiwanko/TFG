#include <iostream> // Biblioteca de entrada salida
#include <sstream> // Biblioteca de manejo de strings
#include <cmath> // Biblioteca matematica de C
#include <Utilidades.h> // Biblioteca de Utilidades
#include "Tramos.h" // Encabezado donde se recopilan los diferentes tramos
#include "Camera.h" // Encabezado donde esta definida la camara.
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
			
//Lista de los identificadores de los tramos.
GLint const totalTramos = 7;
GLint tramos_disponibles[totalTramos];

//Atributos para el funcionamiento del Hud
GLboolean seleccionado = false;
GLint tramoactual = 0;
//control de la rotacion de las piezas;
GLfloat rotacion_pieza = 0;

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



//Atributos de la camara
Camera camaraflotante;
GLfloat velocidadcamara=1.0;

GLfloat xpos = 0, ypos = 0, zpos = 0, xrot = 0, yrot = 0;
GLint reduccionVelocidadCamara = 2;
GLfloat lastx, lasty;


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

BOOLEAN leftclick=false;
BOOLEAN rightclick=false;

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
/*
En esta funcion utilizaremos la funcion quads para crear una densidad de quads/metro consistente para todo el circuito
*/
void quadsEnCuadradosPorMetro(GLfloat v0[], GLfloat v1[], GLfloat v2[], GLfloat v3[], GLint cuadradosPorMetro) {
	float distancia_horizontal = sqrt((v0[0] - v1[0])*(v0[0] - v1[0]) + (v0[2] - v1[2])*(v0[2] - v1[2]));
	float distancia_vertical = sqrt((v0[0] - v3[0])*(v0[0] - v3[0]) + (v0[2] - v3[2])*(v0[2] - v3[2]));
	//quads(v0, v1, v2, v3, (int)distancia_horizontal *cuadradosPorMetro, (int)distancia_vertical* cuadradosPorMetro);
	/*
	v0------v1
	|		 |
	|		 |
	v3------v2
	*/
	quadtex(v3, v2, v1, v0, 0.0, 1.0, 0.0, 1.0, (int)distancia_horizontal *cuadradosPorMetro, (int)distancia_vertical* cuadradosPorMetro);
}

void quadsEnCuadradosPorMetroTex(GLfloat v0[], GLfloat v1[], GLfloat v2[], GLfloat v3[], GLint cuadradosPorMetro, GLint repX, GLint repY) {
	float distancia_horizontal = sqrt((v0[0] - v1[0])*(v0[0] - v1[0]) + (v0[2] - v1[2])*(v0[2] - v1[2]));
	float distancia_vertical = sqrt((v0[0] - v3[0])*(v0[0] - v3[0]) + (v0[2] - v3[2])*(v0[2] - v3[2]));
	//quads(v0, v1, v2, v3, (int)distancia_horizontal *cuadradosPorMetro, (int)distancia_vertical* cuadradosPorMetro);
	/*
	v0------v1
	|		 |
	|		 |
	v3------v2
	*/
	//v1, v0, v3, v2,
	quadtex( v2,v3, v0, v1, 0.0, repX, 0.0, repY, (int)distancia_horizontal *cuadradosPorMetro, (int)distancia_vertical* cuadradosPorMetro);
}



void init_de_Textura(GLuint &id, char* nombre)
{
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	loadImageFile(nombre);
}

void dibuja_anuncio(GLuint &id, GLfloat x, GLfloat z) {
	//Anuncios

	glPushAttrib(GL_CURRENT_BIT);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	

	GLfloat v0[] = { x - 3,5,z };
	GLfloat v1[] = { x + 3,5,z };
	GLfloat v2[] = { x + 3,2,z };
	GLfloat v3[] = { x - 3,2,z };
	quadtex(v3, v2, v1, v0, 0.0, 1, 0.0, 1, 20, 20);

	glBindTexture(GL_TEXTURE_2D, textura_metal);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);



	glBegin(GL_QUAD_STRIP);
	glNormal3f(0, 0, 1);
	glTexCoord2f(0, 0);
	glVertex3f(x - 2.5, 0, z - 0.01);

	glTexCoord2f(0, 1);
	glVertex3f(x - 2.5, 5, z - 0.01);

	glTexCoord2f(1, 0);
	glVertex3f(x - 2, 0, z - 0.01);

	glTexCoord2f(1, 1);
	glVertex3f(x - 2, 5, z - 0.01);
	glEnd();

	glBegin(GL_QUAD_STRIP);
	glNormal3f(0, 0, 1);
	glTexCoord2f(0, 0);
	glVertex3f(x + 2, 0, z - 0.01);

	glTexCoord2f(0, 1);
	glVertex3f(x + 2, 5, z - 0.01);

	glTexCoord2f(1, 0);
	glVertex3f(x + 2.5, 0, z - 0.01);

	glTexCoord2f(1, 1);
	glVertex3f(x + 2.5, 5, z - 0.01);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, textura_panel_trasero);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


	 v0[2] = z - 0.001 ;
	 v1[2] = z - 0.001 ;
	 v2[2] = z - 0.001 ;
	 v3[2] = z - 0.001 ;
	quadtex(v2, v3, v0, v1, 0.0, 1, 0.0, 1, 20, 20);



	glPopAttrib();
}

void dibuja_meta( GLfloat x, GLfloat z) {
	//Anuncios
	glPushAttrib(GL_CURRENT_BIT);
	glBindTexture(GL_TEXTURE_2D, textura_start);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	GLfloat v0[] = { x - 3,5,z };
	GLfloat v1[] = { x + 3,5,z  };
	GLfloat v2[] = { x + 3,2,z  };
	GLfloat v3[] = { x - 3,2,z  };
	quadtex(v3, v2, v1, v0, 0.0, 1, 0.0, 1, 20, 20);


	glBindTexture(GL_TEXTURE_2D,  textura_metal);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glBegin(GL_QUAD_STRIP);
	glTexCoord2f(0, 0);
	glVertex3f(x - 2.5, 0, z - 0.01);

	glTexCoord2f(0, 1);
	glVertex3f(x - 2.5, 5, z - 0.01);

	glTexCoord2f(1, 0);
	glVertex3f(x - 2, 0, z - 0.01);

	glTexCoord2f(1, 1);
	glVertex3f(x - 2, 5, z - 0.01);
	glEnd();

	glBegin(GL_QUAD_STRIP);
	glTexCoord2f(0, 0);
	glVertex3f(x + 2, 0, z - 0.01);

	glTexCoord2f(0, 1);
	glVertex3f(x + 2, 5, z - 0.01);

	glTexCoord2f(1, 0);
	glVertex3f(x + 2.5, 0, z - 0.01);

	glTexCoord2f(1, 1);
	glVertex3f(x + 2.5, 5, z - 0.01);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, textura_goal);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


	v0[2] = z - 0.02;
	v1[2] = z - 0.02;
	v2[2] = z - 0.02;
	v3[2] = z - 0.02;
	quadtex(v2, v3, v0, v1, 0.0, 1, 0.0, 1, 20, 20);



	glPopAttrib();
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

void instrucciones() {

	cout << "Reglas:\n El juego terminara una vez hayas dado 3 vueltas completas al circuito\n\nControles:\n Flechas direccionales: Aceleracion y movimiento del vehiculo\n" 
		<< " Barra espaciadora: freno de mano(frenar mas rapido)\n P/p: Captura de pantalla\n G/g: Activa/Desactiva camara aerea\n L/l: Activa/Desactiva iluminacion\n"
		<< " N/n: Activa/Desactiva efecto niebla\n C/c: Activa/Desactiva HUD\n S/s: Cambia modo Alambrico/Texturado\n 1/2: Cambia entre circuitos 1(sencillo) 2(complejo)\n\n Notas:\n Todas las funciones de visualizacion se pueden activar en conjunto salvo alambrico simple la cual desactiva temporalmente las demas\n Al cambiar de mapa se perdera el progreso en el nivel actual y se comenzara desde 0";
}

void dibuja_recta(GLfloat ancho, GLfloat largoinicio, GLfloat largofin, GLint cuadradosmetro, BOOLEAN iluminacion) {
	//Uso de las texturas
	glBindTexture(GL_TEXTURE_2D, textura_carretera);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// La textura se repite en abcisas
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	// La textura se repite en ordenadas
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	if (iluminacion) {
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	}
	else {
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	}

	//Recta con
	GLfloat mitancho = (float)ancho / 2.0;
	GLfloat v0[] = { -mitancho,0,largofin };
	GLfloat v1[] = { mitancho,0,largofin };
	GLfloat v2[] = { mitancho,0,largoinicio };
	GLfloat v3[] = { -mitancho,0,largoinicio };
	quadsEnCuadradosPorMetroTex(v0, v1, v2, v3, cuadradosmetro, 1, (int)largofin + abs(largoinicio));
}

//radio>=8  curvicidad 0-100
void curva_circular(GLfloat radio, GLint curvicidad, GLint cuadradospormetro, BOOLEAN iluminacion, GLboolean angulo_comienzo = 0) {

	glBindTexture(GL_TEXTURE_2D, textura_carretera);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (iluminacion) {
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	}
	else {
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	}


	float fuera[100][2];
	float dentro[100][2];

	for (int i = 0; i < 100; i++) {
		fuera[i][0] = (1 * cos(i * 2 * PI / 100 + angulo_comienzo)) * radio;
		fuera[i][1] = (1 * sin(i * 2 * PI / 100 + angulo_comienzo)) * radio;
		dentro[i][0] = fuera[i][0] * (radio - 4) / radio;
		dentro[i][1] = fuera[i][1] * (radio - 4) / radio;
	}

	for (int i = 0; i < curvicidad; i++) {
		GLfloat v0[] = { fuera[i][0],0,fuera[i][1] };
		GLfloat v1[] = { dentro[i][0],0,dentro[i][1] };
		GLfloat v2[] = { dentro[i + 1][0],0, dentro[i + 1][1] };
		GLfloat v3[] = { fuera[i + 1][0],0,fuera[i + 1][1] };
		quadsEnCuadradosPorMetroTex(v0, v1, v2, v3, cuadradospormetro, 1, 1);
	}
}

void circuitoNivel1(GLint cuadradospormetro, BOOLEAN iluminacion) {

	dibuja_meta(0, 2);
	//recta inicio
	dibuja_recta(4, -10, 30, cuadradospormetro, iluminacion);
	//recta paralela
	glPushMatrix();
	glTranslatef(40, 0, 0);
	dibuja_recta(4, -10, 30, cuadradospormetro, iluminacion);
	glPopMatrix();

	//recta entre curvas
	glPushMatrix();
	glTranslatef(6, 0, 36);
	glRotatef(90, 0, 1, 0);
	dibuja_recta(4, 0, 28, cuadradospormetro, iluminacion);
	glPopMatrix();

	//primera curva
	glPushMatrix();
	glTranslatef(6, 0, 30);
	glRotatef(-90, 0, 1, 0);
	//glCallList(cuarto_curva);
	curva_circular(8, 25, cuadradospormetro, iluminacion);
	glPopMatrix();

	//segunda curva
	glPushMatrix();
	glTranslatef(34, 0, 30);
	glRotatef(0, 0, 1, 0);
	curva_circular(8, 25, cuadradospormetro, iluminacion);
	glPopMatrix();

	// grande curva
	glPushMatrix();
	glTranslatef(20, 0, -10);
	glRotatef(180, 0, 1, 0);
	curva_circular(22, 50, cuadradospormetro, iluminacion);
	glPopMatrix();
}

/*Este circuito no lo termine*/
void circuitoYoshi(GLint cuadradospormetro, BOOLEAN iluminacion) {
	dibuja_meta(0, 2);
	//recta inicio
	dibuja_recta(4, -10, 30, cuadradospormetro, iluminacion);

	//primera curva
	glPushMatrix();
	glTranslatef(10, 0, 30);
	glRotatef(0, 0, 1, 0);
	glNormal3f(0.0, 1.0, 0.0);
	curva_circular(12, 35, cuadradospormetro, iluminacion, PI / 2);
	glPopMatrix();


	//segunda recta
	glPushMatrix();
	glTranslatef(15.4, 0, 38.42);
	glRotatef(122.5, 0, 1, 0);
	dibuja_recta(4, 0, 15, cuadradospormetro, iluminacion);
	glPopMatrix();

	//segunda curva de 90grados
	glPushMatrix();
	glTranslatef(31.26, 0, 35.42);
	glRotatef(122.5, 0, 1, 0);
	curva_circular(8, 25, cuadradospormetro, iluminacion);
	glPopMatrix();

	//segunda recta
	glPushMatrix();
	glTranslatef(36.31, 0, 32.2);
	glRotatef(122.5 - 90, 0, 1, 0);
	dibuja_recta(4, 0, 30, cuadradospormetro, iluminacion);
	glPopMatrix();


	//primera curva grande
	glPushMatrix();

	glTranslatef(52.435 + 18.0 * 0.8433, 0, 57.48 - 18 * 0.537);
	glRotatef(32.5, 0, 1, 0);;
	curva_circular(20, 50, cuadradospormetro, iluminacion, 0);
	glPopMatrix();

	//tercera recta
	glPushMatrix();
	glTranslatef(82.7938, 0, 38.148);
	glRotatef(122.5 - 270, 0, 1, 0);
	dibuja_recta(4, 0, 30, cuadradospormetro, iluminacion);
	glPopMatrix();


	//tercera curva de 90grados
	glPushMatrix();
	glTranslatef(66.67, 0, 13.11);
	glRotatef(122.5, 0, 1, 0);
	curva_circular(12, 35, cuadradospormetro, iluminacion, PI / 2);
	glPopMatrix();
}

void circuitoNivel2(GLint cuadradospormetro, BOOLEAN iluminacion) {
	dibuja_meta(0, 2);
	//recta inicio
	dibuja_recta(4, -30, 40, cuadradospormetro, iluminacion);

	//primera curva
	glPushMatrix();
	glTranslatef(24, 0, 40);
	glRotatef(-90, 0, 1, 0);
	glNormal3f(0.0, 1.0, 0.0);
	curva_circular(26, 25, cuadradospormetro, iluminacion);
	glPopMatrix();


	//segunda recta
	glPushMatrix();
	glTranslatef(24, 0, 64);
	glRotatef(90, 0, 1, 0);
	dibuja_recta(4, 0, 15, cuadradospormetro, iluminacion);
	glPopMatrix();

	//segunda curva
	glPushMatrix();
	glTranslatef(39, 0, 54);
	glRotatef(0, 0, 1, 0);
	curva_circular(12, 25, cuadradospormetro, iluminacion);
	glPopMatrix();

	//tercera curva
	glPushMatrix();
	glTranslatef(59, 0, 54);
	glRotatef(180, 0, 1, 0);
	curva_circular(12, 25, cuadradospormetro, iluminacion);
	glPopMatrix();

	//cuarta curva
	glPushMatrix();
	glTranslatef(59, 0, 22);
	glRotatef(0, 0, 1, 0);
	curva_circular(24, 25, cuadradospormetro, iluminacion);
	glPopMatrix();


	//tercera recta
	glPushMatrix();
	glTranslatef(81, 0, 22);
	glRotatef(180, 0, 1, 0);
	dibuja_recta(4, 0, 25, cuadradospormetro, iluminacion);
	glPopMatrix();

	//quinta curva
	glPushMatrix();
	glTranslatef(53, 0, -3);
	glRotatef(270, 0, 1, 0);
	curva_circular(30, 75, cuadradospormetro, iluminacion);
	glPopMatrix();

	//sexta curva
	glPushMatrix();
	glTranslatef(53, 0, 31);
	glRotatef(90, 0, 1, 0);
	curva_circular(8, 50, cuadradospormetro, iluminacion);
	glPopMatrix();


	//cuarta recta
	glPushMatrix();
	glTranslatef(53, 0, 37);
	glRotatef(-90, 0, 1, 0);
	dibuja_recta(4, 0, 25, cuadradospormetro, iluminacion);
	glPopMatrix();

	//sexta curva
	glPushMatrix();
	glTranslatef(28, 0, 23);
	glRotatef(-90, 0, 1, 0);
	curva_circular(16, 25, cuadradospormetro, iluminacion);
	glPopMatrix();


	//quinta recta
	glPushMatrix();
	glTranslatef(14, 0, 23);
	glRotatef(180, 0, 1, 0);
	dibuja_recta(4, 0, 53, cuadradospormetro, iluminacion);
	glPopMatrix();

	//ultima curva
	glPushMatrix();
	glTranslatef(7, 0, -30);
	glRotatef(180, 0, 1, 0);
	curva_circular(9, 50, cuadradospormetro, iluminacion);
	glPopMatrix();


}


void minimapanivel1() {
	//Minimapa
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glRotatef(0, 0, 1, 0);
	glTranslatef(0.6, 0, -0.6);

	glScalef(0.007, 0.007, 0.007);

	glCallList(circuitomini1);
	glPopMatrix();
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);

	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glTranslatef(0.585, 0.6, 0);
	glBindTexture(GL_TEXTURE_2D, textura_start);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	GLfloat v0[] = { 0,0.03,0 };
	GLfloat v1[] = { 0.03,0.03,0 };
	GLfloat v2[] = { 0.03,0,0 };
	GLfloat v3[] = { 0,0,0 };
	quadtex(v2, v3, v0, v1, 0.0, 1, 0.0, 1, 10, 10);
	glTranslatef(0.015, 0, 0);

	glTranslatef(desplazamientox*0.007 + 0.01*focoz, -desplazamientoz*0.007 + 0.01*focox, 0);
	glRotatef(direccionVelocidad*aumentovelocidadgiro + 90, 0, 0, 1);
	glBindTexture(GL_TEXTURE_2D, textura_coche);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	quadtex(v2, v3, v0, v1, 0.0, 1, 0.0, 1, 10, 10);

	glPopAttrib();
	glPopMatrix();
	if (diurnoNocturno) {
		glEnable(GL_LIGHTING);
	}
}

void rellenarListaDeTramosDisponibles() {
	
	for (int i = 0; i < totalTramos; i++) {

		tramos_disponibles[i] = i + 1;
	}
	

}

void init()
// Funcion propia de inicializacion
{
	if (test) {
		glClearColor(1.0, 1.0, 1.0, 1.0);
		glEnable(GL_DEPTH_TEST);
		glutSetWindowTitle("MODO TEST");

		//inicializamos la camara con los valores por defecto.
		camaraflotante = Camera();

	}
	else {


		//cout << "Version: OpenGL " << glGetString(GL_VERSION) << endl;
		glClearColor(0, 0, 0, 1.0);
		instrucciones();

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




		//Cilindro
		cilindro = glGenLists(1);
		glNewList(cilindro, GL_COMPILE);
		glPushAttrib(GL_CURRENT_BIT);


		//Uso de texturas
		glBindTexture(GL_TEXTURE_2D, textura_fondo);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		float punto[40][2];

		for (int i = 0; i < 40; i++) {
			punto[i][0] = 200 * cos(i * 2 * PI / 40);
			punto[i][1] = 200 * sin(i * 2 * PI / 40);
		}
		glBegin(GL_QUAD_STRIP);
		for (int i = 0; i < 40; i++) {

			glTexCoord2f((float)i / 40.0 * 5, 0);
			glVertex3f(punto[i][0], 0, punto[i][1]);
			glTexCoord2f((float)i / 40.0 * 5, 1);
			glVertex3f(punto[i][0], 20, punto[i][1]);

		}
		glTexCoord2f(5, 0);
		glVertex3f(punto[0][0], 0, punto[0][1]);
		glTexCoord2f(5, 1);
		glVertex3f(punto[0][0], 20, punto[0][1]);
		glEnd();
		glPopAttrib();
		glEndList();

		//Creacion de los Anuncios
		anuncio_nuka = glGenLists(1);
		glNewList(anuncio_nuka, GL_COMPILE);
		dibuja_anuncio(textura_nukacola, 0, 0);
		glEndList();

		anuncio_slurm = glGenLists(1);
		glNewList(anuncio_slurm, GL_COMPILE);
		dibuja_anuncio(textura_slurm, 0, 0);
		glEndList();

		anuncio_sonic = glGenLists(1);
		glNewList(anuncio_sonic, GL_COMPILE);
		dibuja_anuncio(textura_sonic, 0, 0);
		glEndList();

		aguja = glGenLists(1);
		glNewList(aguja, GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, textura_aguja);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glPushAttrib(GL_CURRENT_BIT);

		glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(0, 0);
		glVertex3f(-0.01, 0, 0);
		glTexCoord2f(1, 0);
		glVertex3f(0.01, 0, 0);
		glTexCoord2f(-0, 1);
		glTexCoord2f(1, 1);
		glVertex3f(0, 0.2, 0);
		glEnd();
		glPopAttrib();
		glEndList();

		//Suelo y Techo(cielo)

		hierba = glGenLists(1);
		glNewList(hierba, GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, textura_hierba);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// La textura se repite en abcisas
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		// La textura se repite en ordenadas
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		glBegin(GL_QUADS);

		glNormal3f(0, 1, 0);
		glTexCoord2f(-50, -50);
		glVertex3f(-200, -0.001, -200);

		glTexCoord2f(50, -50);
		glVertex3f(200, -0.001, -200);

		glTexCoord2f(50, 50);
		glVertex3f(200, -0.001, 200);

		glTexCoord2f(-50, 50);
		glVertex3f(-200, -0.001, 200);
		glEnd();
		glEndList();

		cielo_dia = glGenLists(1);
		glNewList(cielo_dia, GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, textura_cielodia);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// La textura se repite en abcisas
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		// La textura se repite en ordenadas
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glBegin(GL_QUADS);
		glNormal3f(0, 1, 0);
		glTexCoord2f(-50, -50);
		glVertex3f(-200, 20, -200);

		glTexCoord2f(50, -50);
		glVertex3f(200, 20, -200);

		glTexCoord2f(50, 50);
		glVertex3f(200, 20, 200);

		glTexCoord2f(-50, 50);
		glVertex3f(-200, 20, 200);

		glEnd();
		glEndList();

		cielo_noche = glGenLists(1);
		glNewList(cielo_noche, GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, textura_cielonoche);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// La textura se repite en abcisas
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		// La textura se repite en ordenadas
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glBegin(GL_QUADS);
		glNormal3f(0, 1, 0);
		glTexCoord2f(-50, -50);
		glVertex3f(-200, 20, -200);

		glTexCoord2f(50, -50);
		glVertex3f(200, 20, -200);

		glTexCoord2f(50, 50);
		glVertex3f(200, 20, 200);

		glTexCoord2f(-50, 50);
		glVertex3f(-200, 20, 200);

		glEnd();
		glEndList();

		circuito1 = glGenLists(1);
		glNewList(circuito1, GL_COMPILE);
		circuitoNivel1(20, TRUE);
		glEndList();

		circuitomini1 = glGenLists(1);
		glNewList(circuitomini1, GL_COMPILE);
		circuitoNivel1(1, TRUE);
		glEndList();

		circuito2 = glGenLists(1);
		glNewList(circuito2, GL_COMPILE);
		circuitoNivel2(10, TRUE);
		glEndList();

		circuitomini2 = glGenLists(1);
		glNewList(circuitomini2, GL_COMPILE);
		circuitoNivel2(1, TRUE);
		glEndList();
		//Enables
		glEnable(GL_DEPTH_TEST); // Habilita visibilidad
		glEnable(GL_NORMALIZE);
		glEnable(GL_TEXTURE_2D);

	}


}


void minimapanivel2() {
	//Minimapa
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glRotatef(0, 0, 1, 0);
	glTranslatef(0.5, 0, -0.6);

	glScalef(0.005, 0.005, 0.005);

	glCallList(circuitomini2);
	glPopMatrix();
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);

	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glTranslatef(0.485, 0.6, 0);
	glBindTexture(GL_TEXTURE_2D, textura_start);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	GLfloat v0[] = { 0,0.03,0 };
	GLfloat v1[] = { 0.03,0.03,0 };
	GLfloat v2[] = { 0.03,0,0 };
	GLfloat v3[] = { 0,0,0 };
	quadtex(v2, v3, v0, v1, 0.0, 1, 0.0, 1, 10, 10);


	glTranslatef(0.015, 0, 0);
	glTranslatef(desplazamientox*0.005 + 0.01*focoz, -desplazamientoz*0.005 + 0.01*focox, 0);
	glRotatef(direccionVelocidad*aumentovelocidadgiro + 90, 0, 0, 1);
	glBindTexture(GL_TEXTURE_2D, textura_coche);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	quadtex(v2, v3, v0, v1, 0.0, 1, 0.0, 1, 10, 10);

	glPopAttrib();
	glPopMatrix();
	if (diurnoNocturno) {
		glEnable(GL_LIGHTING);
	}

}

void hudJuego() {
	// Habilitamos blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Z-Buffer
	glDepthMask(GL_FALSE);
	// Dibujar traslucidos
	if (nivel == 1) {
		minimapanivel1();
	}else{
		minimapanivel2();
	}
	
	
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	if (terminado) {
		glBindTexture(GL_TEXTURE_2D, textura_victoria);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glBegin(GL_QUAD_STRIP);
		glTexCoord2f(0, 0);
		glVertex3f(-1, -1, 1);

		glTexCoord2f(0, 1);
		glVertex3f(-1, 1, 1);

		glTexCoord2f(1, 0);
		glVertex3f(1, -1, 1);

		glTexCoord2f(1, 1);
		glVertex3f(1, 1, 1);

		glEnd();
	}

	glBindTexture(GL_TEXTURE_2D, textura_velocimetro);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBegin(GL_QUAD_STRIP);
	glTexCoord2f(0, 0);
	glVertex3f(0.6, -1, 1);

	glTexCoord2f(0, 1);
	glVertex3f(0.6, -0.6, 1);

	glTexCoord2f(1, 0);
	glVertex3f(0.9, -1, 1);

	glTexCoord2f(1, 1);
	glVertex3f(0.9, -0.6, 1);

	glEnd();

	if (vueltas == 0) {
		glBindTexture(GL_TEXTURE_2D, textura_bandera1);
	}
	else if (vueltas == 1) {
		glBindTexture(GL_TEXTURE_2D, textura_bandera2);

	}else if(vueltas >=2){
		glBindTexture(GL_TEXTURE_2D, textura_bandera3);

	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBegin(GL_QUAD_STRIP);
	glTexCoord2f(1, 1);
	glVertex3f(-0.6, 1, 1);

	glTexCoord2f(1, 0);
	glVertex3f(-0.6, 0.6, 1);

	glTexCoord2f(0, 1);
	glVertex3f(-0.9, 1, 1);

	glTexCoord2f(0, 0);
	glVertex3f(-0.9, 0.6, 1);

	glEnd();

	glPopAttrib();
	glPushMatrix();
	glTranslatef(0.75,-0.8,0);
	glRotatef(145-rotacion_velocidad, 0, 0, 1);
	glCallList(aguja);
	glPopMatrix();




	// Z-Buffer a estado normal
	glDepthMask(GL_TRUE);




}

void dibuja_tramo(GLint identificador) {
	switch (identificador) {
	case 1:
		Tramo();

		break;
	case 2:
		TramoCurvo();
		break;
	case 3:
		Rampa();
		break;
	case 4:
		RampaCurva();
		break;
	case 5:
		TramoSinuosoHorizontal();
		break;
	case 6:
		TramoSinuosoVertical();
		break;
	case 7:
		Looping();
		break;
	}
}

void hudElementBaseSelectorPiezas() {
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
		glColor4f(0.196,0.8, 0.196,0.7);
		glBegin(GL_QUAD_STRIP);
		glTexCoord2f(0, 0);
		glVertex3f(-0.85, 0.95, 1);
		glTexCoord2f(0, 1);
		glVertex3f(-0.85,0.7, 1);
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


}


//TODO continuar con el selector de piezas, ver como hacer las animaciones mas automatizables y la colocacion de las piezas mas automatizables.
void hudElementPiezasVisibles() {
	// Habilitamos blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Z-Buffer
	glDepthMask(GL_FALSE);
	
	glPushMatrix();

	
	//derecho
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glTranslatef(0.6, 0.825-0.075, 0.8);
	//rotacion a aplicar.
	glRotatef(90, 1, 0, 0);
	glRotatef(90, 0, 1, 0);
	glRotatef(rotacion_pieza, 1, 0, 0);

	Tramo(0.05,0.15,2,1).draw();

	glPopAttrib();
	glPopMatrix();
	// Z-Buffer a estado normal
	glDepthMask(GL_TRUE);

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
		hudElementBaseSelectorPiezas();
		hudElementPiezasVisibles();

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

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

void coche() {
	
	GLfloat v0[] = { 0, 0, 1.5 };
	GLfloat v1[] = { 3.0, 0 ,1.5 };
	GLfloat v2[] = { 3.0, 0 ,0 };
	GLfloat v3[] = { 0, 0 ,0 };
	glPushMatrix();
	glTranslatef(desplazamientox , 1, desplazamientoz);
	glRotatef(direccionVelocidad*aumentovelocidadgiro + 90, 0, 1, 0);
	glBindTexture(GL_TEXTURE_2D, textura_coche);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	quadtex(v2, v3, v0, v1, 0.0, 1, 0.0, 1, 10, 10);
	glPopMatrix();

}


void cargarCircuito()
{
	glPushMatrix();
	{
		glColor3f(0, 0, 0);
		Tramo(2, 4, 10, 1).draw(); ejes();

		glColor3f(1, 0, 1);
		TramoCurvo(2, 5, -90, 5, 1).draw(); ejes();

		glColor3f(0, 0, 1);
		TramoCurvo(2, 5, -90, 5, 1).draw(); ejes();

		Tramo(2, 4, 10, 1).draw(); ejes();

		Looping(2,4,3,20,1).draw(); ejes();

		Tramo(2, 4, 10, 1).draw(); ejes();
		
		//piezas a testear
		glColor3f(0, 1, 1);
		TramoSinuosoHorizontal(2, 4, 9,2,true, 10, 1).draw(); ejes();
		
		glColor3f(1, 0, 0);
		Tramo(2, 4, 10, 1).draw(); ejes();

		glColor3f(0, 0.2, 0);
		TramoSinuosoVertical(2, 4, 2,2, false, 10, 1).draw(); ejes();

		glColor3f(1, 0.2, 0);
		Tramo(2, 4, 10, 1).draw(); ejes();
	}
	glPopMatrix();
}


void camera(void) {


}


void testeandoCircuito() {
	// Borra buffers y selecciona modelview
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	if (camaraLibre) {//camara libre usando trackball para ver el circuito desde arriba
		camaraflotante.SetGluLookUp();
	}
	else {//camara normal del problema
		GLfloat posicion1[] = { 0.0f, -0.3f, 0.0f, 1.0f };
		glLightfv(GL_LIGHT1, GL_POSITION, posicion1);
		gluLookAt(desplazamientox, 1, desplazamientoz,
			mirandox, 0, mirandoz,
			0, 1, 0); // Situa la camara
	}

	ejes();


	glPolygonMode(GL_FRONT, GL_LINE);




	cargarCircuito();


}

void display()
// Funcion de atencion al dibujo
{
	if (test) {

		testeandoCircuito();
		
		//if(modo==creacion) si estamos en modo creacion se aplicarán unas reglas
		hudModoCreacion();


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
		//cout << desplazamientox << " x " << desplazamientoz << " z \n";
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
			coche();
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
			hudJuego();


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
	gluPerspective(120, razon, 0.1, 400);
}

void onSpecialKeyModoCreacion(int specialKey, int x, int y) {
	switch (specialKey) {
	case GLUT_KEY_LEFT:
		tramoactual = tramoactual - 1;

		break;
	case GLUT_KEY_RIGHT:
		tramoactual = tramoactual + 1;
		break;
	case GLUT_KEY_UP:

		break;
	case GLUT_KEY_DOWN:

		break;
	}

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


void mouseMovement(int x, int y) {
	//Comprobar diferencia entre la 'x' de y la 'y' de la posicion actual del raton con la anterior
	int diffx = x - lastx;
	int diffy = y - lasty;
	//Actualizamos la posicion anterior para poder continuar con el desplazamiento
	lastx = x;
	lasty = y;
	//Actualizamos los valores de la rotacion de la camara
	camaraflotante.ProcessMouseMovement(diffx, diffy);

	/*
	//ajustamos los valores de las rotaciones para que sean siempre valores entre 0 y 360
	xrot += (float)diffy;
	if (xrot >360) xrot -= 360;
	if (xrot < -360) xrot += 360;

	yrot += (float)diffx;
	if (yrot >360) yrot -= 360;
	if (yrot < -360)yrot += 360;
	*/

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
		//al soltar el boton debemos de dejar las variables de la ultima posicion a 0
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

void onKeyOld(unsigned char tecla, int x, int y)
// Funcion de atencion al teclado
{
	//float xrotrad, yrotrad;
	switch (tecla) {

		//la division /2 en xpos,zpos,ypos es simplemente para graduar el desplazamiento de la camara.
	case 'W':
	case 'w':
		/* Antiguo metodo ahora en desuso

		yrotrad =  (yrot / 180 * 3.141592654f);
		xrotrad = (xrot / 180 * 3.141592654f);
		cout << "\n xpos " << xpos << " ypos " << ypos << " zpos " << zpos  << " yrotad " << yrot << " xrotrad " << xrot;
		xpos += float(sin(yrotrad))/ reduccionVelocidadCamara;
		zpos -= float(cos(yrotrad))/ reduccionVelocidadCamara;

		ypos -= float(sin(xrotrad))/ reduccionVelocidadCamara;

		*/
		camaraflotante.ProcessKeyboard(FORWARD, velocidadcamara);
		break;

	case 'S':
	case 's':
		/*
		yrotrad = (yrot / 180 * 3.141592654f);
		xrotrad = (xrot / 180 * 3.141592654f);
		xpos -= float(sin(yrotrad))/ reduccionVelocidadCamara;
		zpos += float(cos(yrotrad))/ reduccionVelocidadCamara;
		ypos += float(sin(xrotrad))/ reduccionVelocidadCamara;
		*/
		camaraflotante.ProcessKeyboard(BACKWARD, velocidadcamara);
		break;

	case 'D':
	case 'd':

		/*
		yrotrad = (yrot / 180 * 3.141592654f);
		xpos += float(cos(yrotrad)) * 0.2;
		zpos += float(sin(yrotrad)) * 0.2;
		*/
		camaraflotante.ProcessKeyboard(RIGHT, velocidadcamara);
		break;
	case 'A':
	case 'a':
		/*
		yrotrad = (yrot / 180 * 3.141592654f);
		xpos -= float(cos(yrotrad)) * 0.2;
		zpos -= float(sin(yrotrad)) * 0.2;
		*/
		camaraflotante.ProcessKeyboard(LEFT, velocidadcamara);
		break;

		/*
		case 's':
		case 'S':
		alambricoSimple = !alambricoSimple;

		break;
		*/

	case 'l':
	case 'L':
		diurnoNocturno = !diurnoNocturno;

		break;
	case 'n':
	case 'N':
		niebla = !niebla;

		break;

	case 'g':
	case 'G':
		camaraLibre = !camaraLibre;
		break;
	case ' '://Freno de mano
			 //TODO añadir el fijado de camara sobre la pieza actual

		moduloVelocidad = moduloVelocidad - 1.0;
		if (moduloVelocidad < 0) {
			moduloVelocidad = 0;
		}
		rotacion_velocidad = moduloVelocidad * 10;
		break;
	case 'c':
	case 'C':
		hud = !hud;
		break;
	case 'p':
	case 'P':
		saveScreenshot("foto.jpg", ancho, alto);
		break;
	case '1':
		nivel = 1;
		reinicio();
		break;
	case '2':
		nivel = 2;
		reinicio();
		break;
	case 13:

		if (creacion) {
			if (seleccionado)
			{
				//si ya se habia seleccionado una pieza la dibujamos
			}
			else
			{
				// si no, simplemente marcamos la pieza como seleccionada
			}
			seleccionado = !seleccionado;
		}
		break;
	case 27: // Pulso escape
			 //TODO AÑADIR MENU
		exit(0);
	}
	stringstream titulo;
	titulo << "Circuito final: " << moduloVelocidad << " m/s";
	glutSetWindowTitle(titulo.str().c_str()); // Pone el titulo

}



void onKeyCreacion(unsigned char tecla, int x, int y)
// Funcion de atencion al teclado
{
	//float xrotrad, yrotrad;
	switch (tecla) {
	
	//la division /2 en xpos,zpos,ypos es simplemente para graduar el desplazamiento de la camara.
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
	case 13:
		
		
			if (seleccionado)
			{
				//si ya se habia seleccionado una pieza la dibujamos
				dibuja_tramo(tramoactual);
			}
			else
			{
				// si no, simplemente marcamos la pieza como seleccionada
			}
			seleccionado = !seleccionado;
		break;
	case 27: // Pulso escape
		//TODO AÑADIR MENU
		exit(0);
	}
	stringstream titulo;
	titulo << "Circuito final: " << moduloVelocidad << " m/s";
	glutSetWindowTitle(titulo.str().c_str()); // Pone el titulo
							
}

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
										
	rotacion_pieza += float((0.018f*float(tiempo_transcurridoc)));

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


void main(int argc, char** argv)
// Programa principal
{
	FreeImage_Initialise();
	glutInit(&argc, argv); // Inicializacion de GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // Alta de buffers a usar
	glutInitWindowSize(800, 600); // Tamanyo inicial de la ventana

	glutCreateWindow("Circuito final: 0 m/s");	// Pone el titulo para que se vaya actualizando luego en onkey
	glutDisplayFunc(display); // Alta de la funcion de atencion a display
	glutReshapeFunc(reshape); // Alta de la funcion de atencion a reshape
	glutMouseFunc(mouse);//Alta de la funcion de atencion a los botones del raton
	//glutPassiveMotionFunc(mouseMovement); //Funcion de atencion al raton pasiva siempre a la escucha
	glutKeyboardFunc(onKeyCreacion); // Alta de la funcion de atencion al teclado
	glutSpecialFunc(onSpecialKey);// Alta de la funcion de atencion al teclado especial
	glutIdleFunc(onIdle); // Alta de la funcion de atencion a idle
	init(); // Inicializacion propia
	glutMainLoop(); // Puesta en marcha del programa
	FreeImage_DeInitialise();

}