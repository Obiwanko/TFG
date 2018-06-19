
#ifndef ENTORNO_H
#define ENTORNO_H
#include "Utilidades.h"// Biblioteca de Utilidades

class Entorno
{
protected:
	int _res;								// resolucion
	int _texX;								// nº rep textura
	const int _id = 1;
	
	void drawDesk(GLuint textura,GLfloat ancho,GLfloat largo, GLfloat profundo);


	void drawRoom();

public:
	//valores por defecto

	Entorno() : _res(30), _texX(1) {};

	Entorno( int res, int repitetex )
	{
		_res = res;
		_texX = repitetex;
	};
	
	/*
	funcion de dibujar el entorno completo, recibe como parametros las longitudes de la mesa ya que la habitacion será fija
	*/
	void draw(GLuint texturaMesa, GLfloat ancho, GLfloat largo, GLfloat profundo);


};
#endif