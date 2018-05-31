/***************************************************
ISGI::Test del circuito estatico
Roberto Vivo', 2014 (v2.0)

Dependencias: 
+scalextric
***************************************************/ 

#define PROYECTO "Conduccion"

#include <iostream>										// Biblioteca de entrada salida
#include <sstream>
#include <cmath>
#include "Tramos.h"

using namespace std;

// Constantes de la aplicación
static const float DELTA_GIRO = 0.25f * PI / 180;		// grados de giro por pulsacion
static const float DELTA_VELOCIDAD = 0.1f;				// metros/segundo por pulsacion
static const float A = 5;								// amplitud de la senoide en metros
static const float T = 2*PI/50;							// periodo de la senoide en metros
static const float ANCHO = 2;							// ancho del carril en metros

// Variables dependientes de la interacción
static struct { float x; float y; float z; } posicion = {0, 1, 0};
static struct { float x; float z; float mod;} velocidad = {1, 0, 0};



void giraIzquierda()
{
	// Aplica una rotacion en y (+) al vector velocidad
	// El modulo no cambia
	static float cosa = cos(DELTA_GIRO);
	static float sena = sin(DELTA_GIRO);

	//			cos		0	sen
	//  Ry()=	0		1   0
	//			-sen	0	cos
	float vx =  velocidad.x*cosa + velocidad.z*sena;
	float vz = -velocidad.x*sena + velocidad.z*cosa;

	velocidad.x = vx;
	velocidad.z = vz;
}

void giraDerecha()
{
	// Aplica una rotacion en y (-) al vector velocidad
	// El modulo no cambia
	static float cosa = cos(-DELTA_GIRO);
	static float sena = sin(-DELTA_GIRO);

	//			cos		0	sen
	//  Ry()=	0		1   0
	//			-sen	0	cos
	float vx =  velocidad.x*cosa + velocidad.z*sena;
	float vz = -velocidad.x*sena + velocidad.z*cosa;

	velocidad.x = vx;
	velocidad.z = vz;
}

void acelera()
{
	stringstream title;  
	// Cada vez que se llama aumenta al velocidad en 0.1 m/sg
	velocidad.mod += DELTA_VELOCIDAD;
	title << "Ejemplo P6. SPEED " << velocidad.mod << " m/sg";
	glutSetWindowTitle( title.str().c_str() );
}

void frena()
{
	stringstream title;  
	// Cada vez que se llama disminuye la velocidad en 0.1 m/sg
	// No hay velocidad negativa
	velocidad.mod -= DELTA_VELOCIDAD;
	if(velocidad.mod < 0) velocidad.mod = 0;
	title << "Ejemplo P6. SPEED " << velocidad.mod << " m/sg";
	glutSetWindowTitle( title.str().c_str() );
}

void init()
// Funcion de inicializacion propia
{
	glClearColor(1.0,1.0,1.0,1.0);
	glEnable(GL_DEPTH_TEST);

	glutSetWindowTitle("Ejemplo P6. SPEED 0 m/s");
}

void cargarCircuito()
{ 
	glPushMatrix();
	{
		glColor3f( 0,0,0 );
		Tramo(2, 4, 10, 1).draw(); ejes();

		glColor3f( 1,0,1 );
		TramoCurvo(2, 5, -90, 5, 1).draw(); ejes();

		glColor3f( 0,0,1 );
		TramoCurvo(2, 5, -90, 5, 1).draw(); ejes();

		Tramo(2, 4, 10, 1).draw(); ejes();
		TramoCurvo(2, 5, -90, 5, 1).draw(); ejes();
		Tramo(2, 8.17, 10, 1).draw(); ejes();
		TramoCurvo(2, 10, -270, 10, 1).draw(); ejes();
		Tramo(2, 6, 6, 1).draw(); ejes();
		TramoCurvo(2, 4.5, 180, 10, 1).draw(); ejes();
		Tramo(2, 7.1, 6, 1).draw(); ejes();

//		Tramo(2,3,4).draw();
		glColor3f( 0,1,1 );

		glColor3f( 1,0,0 );
//		TramoCurvo(2,1,-100,4,2).draw();

		glColor3f( 0,0.2,0 );
//		TramoCurvo(2,5,40,10,4).draw();
	}
	glPopMatrix();
}

void display()
// Funcion de atencion al dibujo
{
	// Borra buffers y selecciona modelview
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Situa la camara en posicion mirando en la direccion de la velocidad
	//gluLookAt(	posicion.x, posicion.y, posicion.z,
	//			posicion.x+velocidad.x, posicion.y, posicion.z+velocidad.z,
	//			0,1,0 );

	gluLookAt(0,30,0, 0,0,0, 1,0,0);
	ejes();


		glPolygonMode( GL_FRONT, GL_LINE );




	cargarCircuito();

	/*
	// Calcula el intervalo donde esta la camara en z
	int inicio = int( posicion.z );

	// Siempre dibuja 50 tramos desde uno antes de inicio
	// Se calculan los vertices del quad siguiendo la normal a la curva 
	// Si la tangente unitaria es ( tz, tx ), la normal a las 9 es ( -tx, tz )
	glBegin(GL_QUAD_STRIP);
	for(int i = -1; i < 50; i++){
		float z = (float)(inicio-i);			// nos movemos hacia -z
		float x = A * sin( T * z );				// trazado de la carretera (senoide)
		float xp = A * T * cos( T * z );		// pendiente en z
		// normal al trazado
		float modn = 1 / sqrt(1 + xp*xp);		// modulo del vector perpendicular a la curva
		float nx = -modn;						// componente de la normal unitaria en x
		float nz = xp * modn;					// componente de la normal unitaria en z

		glVertex3f(x+ANCHO*nx, 0, z+ANCHO*nz);	// vertice a la izquierda
		glVertex3f(x-ANCHO*nx, 0, z-ANCHO*nz);	// vertice a la derecha
	}
	glEnd();
	glPopMatrix();
	*/
	glutSwapBuffers();
}

void reshape(GLint w, GLint h)
// Funcion de atencion al redimensionamiento
{
	// Usamos toda el area de dibujo
	glViewport(0,0,w,h);

	// Definimos la camara (matriz de proyeccion)
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Isometria
	float razon = (float) w / h;
	gluPerspective(40,razon,0.1,100); 
}
void onIdle()
{
	int ahora;
	float tiempo_transcurrido;
	static int antes = 0;

	// Gestion del tiempo
	ahora= glutGet(GLUT_ELAPSED_TIME);				// tiempo transcurrido desde el inicio
	tiempo_transcurrido= (ahora - antes) / 1000.0f;	// tiempo transcurrido desde antes en sg.
	if(tiempo_transcurrido<0.01) return;			// minimo tiempo de refresco 10 milisegundos
	antes = ahora;

	// Actualizacion de la posicion
	posicion.x += velocidad.mod *velocidad.x * tiempo_transcurrido;
	posicion.z += velocidad.mod *velocidad.z * tiempo_transcurrido;

	glutPostRedisplay();
}

void onFlecha(int flecha, int x, int y)
// Atencion al pulsado de flechas
{
	switch(flecha){
	case GLUT_KEY_LEFT:
		giraIzquierda();
		break;
	case GLUT_KEY_RIGHT:
		giraDerecha();
		break;
	case GLUT_KEY_UP:
		acelera();
		break;
	case GLUT_KEY_DOWN:
		frena();
		break;
	case GLUT_KEY_END:
		exit(0);
	}
	glutPostRedisplay();
}

void main(int argc, char** argv)
// Programa principal
{
  glutInit(&argc, argv);							// Inicializacion de GLUT
  glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB|GLUT_DEPTH);		// Alta de buffers a usar
  glutInitWindowSize(600,400);						// Tamanyo inicial de la ventana
  glutCreateWindow(PROYECTO);						// Creacion de la ventana con su titulo
  glutDisplayFunc(display);							// Alta de la funcion de atencion a display
  glutReshapeFunc(reshape);							// Alta de la funcion de atencion a reshape
  //glutIdleFunc(onIdle);								
  glutSpecialFunc(onFlecha);						// Atencion a las teclas de flecha
  init();
  glutMainLoop();									// Puesta en marcha del programa
}