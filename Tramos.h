/***************************************************
ISGI::Crcuito estatico
Roberto Vivo', 2014 (v2.0)

Clases para la construccion de un circuito uniendo piezas
como las del scalextric.

Clases:
Poin3D: Tripleta de coordenadas
Tramo:  Geometría para un tramo recto de carretera
TramoCurvo: Geometria para una curva como arco de circunferencia



Dependencias: 
+utilidades.h
***************************************************/ 
#include "Utilidades.h"// Biblioteca de Utilidades
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

class Tramo
{
protected:
	int _res;								// resolucion
	int _texX;								// nº rep textura
	float _ancho, _longitud;				// ancho y longitud del tramo
	const int _id = 1;
	glm::mat4 _matinicial;
	glm::mat4 _matfinal;
	
	virtual void setMatrizfinal(glm::mat4 matini);

public:
	//valores por defecto

	Tramo( ): _res(10), _texX(1), _ancho(1), _longitud(1) {};

	//Codiciones para que un tramo sea valido
	Tramo( float ancho, float longitud, glm::mat4 inicial, int res = 1, int repitetex = 1)
	{
		_ancho = max(0, ancho);
		_longitud = max(0, longitud);
		_res = min( max( 1, res) , 100 );
		_texX = max( 1, repitetex);
		_matinicial = inicial;

		setMatrizfinal(inicial);
	};

	glm::mat4 getMatInicial();

	glm::mat4 getMatFinal();

	virtual void draw(GLuint textura, GLuint texturaLateral) ;

	void setRes(GLint res);

	virtual void drawing(GLuint textura, GLuint texturaLateral);

	virtual void calcularPathPoints(std::vector<Point3D> &path);

	/*
	Metodo que escribirá en el output stream dado, los parametros necesarios para pintar la pieza, la resolucion y las repeticiones vendrán dadas por las opciones
	*/
	virtual void writeToFile(std::ostream& o);

};

class TramoCurvo : public Tramo
{
protected:
	float _angulo;
	const int  _id = 2;
	virtual void setMatrizfinal(glm::mat4 matini);
public:

	TramoCurvo() : _angulo() {};
	
	//Codiciones para que este tramo sea valido
	TramoCurvo( float ancho, float longitud, float grados, glm::mat4 inicial, int res = 1, int repitetex = 1 )
	{

		_ancho = max(0, ancho);
		_longitud = max(0, longitud);
		_res = min(max(1, res), 100);
		_texX = max(1, repitetex);
		_matinicial = inicial;
		_angulo = grados;
		setMatrizfinal(inicial);
		// El radio debe ser mayor que la mitad del ancho
		if( longitud < ancho*rad(abs(grados))/2 ) _longitud = ancho*rad(abs(grados))/2;
	};

	glm::mat4 getMatInicial();

	glm::mat4 getMatFinal();

	void draw(GLuint textura, GLuint texturaLateral);

	virtual void drawing(GLuint textura, GLuint texturaLateral);

	virtual void calcularPathPoints(std::vector<Point3D> &path);
	/*
	Metodo que escribirá en el output stream dado, los parametros necesarios para pintar la pieza, la resolucion y las repeticiones vendrán dadas por las opciones
	*/
	virtual void writeToFile(std::ostream& o);

};

class Rampa : public Tramo
{
protected:
	float _pendiente;				// ancho y longitud del tramo
	const int  _id = 3;
	void setMatrizfinal(glm::mat4 matini);
public:
	//valores por defecto
	
	Rampa() : _pendiente(0.5) {}; 

	//Codiciones para que un tramo sea valido
	Rampa(float ancho, float longitud, float pendiente , glm::mat4 inicial, int res = 1, int repitetex = 1 )
	{
		_ancho = max(0, ancho);
		_longitud = max(0, longitud);
		_res = min(max(1, res), 100);
		_texX = max(1, repitetex);
		_pendiente = min(pendiente, 1.5);
		_matinicial = inicial;
		setMatrizfinal(inicial);

	};

	glm::mat4 getMatInicial();

	glm::mat4 getMatFinal();

	void draw(GLuint textura, GLuint texturaLateral);

	virtual void drawing(GLuint textura, GLuint texturaLateral);

	virtual void calcularPathPoints(std::vector<Point3D> &path);
	/*
	Metodo que escribirá en el output stream dado, los parametros necesarios para pintar la pieza, la resolucion y las repeticiones vendrán dadas por las opciones
	*/
	virtual void writeToFile(std::ostream& o);

};


class RampaCurva : public TramoCurvo
{
protected:
	float _angulo;
	float _pendiente;
	const int  _id = 4;
	virtual void setMatrizfinal(glm::mat4 matini);
public:

	RampaCurva() : _angulo() {};

	//Codiciones para que este tramo sea valido
	RampaCurva(float ancho, float longitud, float grados, float pendiente, glm::mat4 inicial, int res = 10, int repitetex = 1 )
	{
		// El radio debe ser mayor que la mitad del ancho
		//if (longitud < ancho*rad(abs(grados)) / 2) _longitud = ancho * rad(abs(grados)) / 2;
		_pendiente = min(pendiente, 1.5);
		_ancho = max(0, ancho);
		_longitud = max(0, longitud);
		_res = min(max(1, res), 100);
		_texX = max(1, repitetex);
		_matinicial = inicial;
		_angulo = grados;
		setMatrizfinal(inicial);

	};

	glm::mat4 getMatInicial();

	glm::mat4 getMatFinal();

	void draw(GLuint textura, GLuint texturaLateral);

	virtual void drawing(GLuint textura, GLuint texturaLateral);

	virtual void calcularPathPoints(std::vector<Point3D> &path);
	/*
	Metodo que escribirá en el output stream dado, los parametros necesarios para pintar la pieza, la resolucion y las repeticiones vendrán dadas por las opciones
	*/
	virtual void writeToFile(std::ostream& o);

};



class TramoSinuosoHorizontal : public Tramo
{
protected:
	float _ondulacion;				
	float _orientacion;
	float _potencia;
	const int  _id = 5;
	virtual void setMatrizfinal(glm::mat4 matini);
public:
	//valores por defecto

	TramoSinuosoHorizontal() : _ondulacion(), _orientacion(){};

	//Codiciones para que un tramo sea valido
	TramoSinuosoHorizontal(float ancho, float longitud, float ondulacion, glm::mat4 inicial, float potencia = 1, bool orientacion = true, int res = 10, int repitetex = 1 )
	{
		_ancho = max(0, ancho);
		_longitud = max(0, longitud);
		//minimo cinco quads para que se pueda mostrar la sinuosidad
		_res = min(max(5, res), 100);
		_texX = max(1, repitetex);
		//la ondulacion vendrá marcada un maximo de 1 ondulacion completa cada 4 metros
		_ondulacion = min(ondulacion, longitud/2);
		_orientacion = orientacion;
		//la potencia por  la que se multiplique la ondulacion no puede ser mayor de 1 cada 4 metros
		_potencia = min(potencia, longitud / 4);
		_matinicial = inicial;
		setMatrizfinal(inicial);

	};

	glm::mat4 getMatInicial();

	glm::mat4 getMatFinal();

	void draw(GLuint textura, GLuint texturaLateral);

	virtual void drawing(GLuint textura, GLuint texturaLateral);

	virtual void calcularPathPoints(std::vector<Point3D> &path);
	/*
	Metodo que escribirá en el output stream dado, los parametros necesarios para pintar la pieza, la resolucion y las repeticiones vendrán dadas por las opciones
	*/
	virtual void writeToFile(std::ostream& o);

};



class TramoSinuosoVertical : public Tramo
{
protected:
	float _ondulacion;
	float _orientacion;
	float _potencia;
	const int  _id = 6;
	virtual void setMatrizfinal(glm::mat4 matini);
public:
	//valores por defecto

	TramoSinuosoVertical() : _ondulacion(), _orientacion() {};

	//Codiciones para que un tramo sea valido
	TramoSinuosoVertical(float ancho, float longitud, float ondulacion, glm::mat4 inicial, float potencia = 1, bool orientacion = true, int res = 10, int repitetex = 1)
	{
		if (ancho > 10) {
			_ancho = 10;
		}
		else {
			_ancho = max(0, ancho);
		}
		_longitud = max(0, longitud);
		//minimo cinco quads para que se pueda mostrar la sinuosidad
		_res = min(max(5, res), 100);
		_texX = max(1, repitetex);
		//la ondulacion vendrá marcada un maximo de 1 ondulacion completa cada 4 metros
		_ondulacion = min(ondulacion, longitud / 2);
		_orientacion = orientacion;
		//la potencia por  la que se multiplique la ondulacion no puede ser mayor de 1 cada 4 metros
		_potencia = min(potencia, longitud/4);
		_matinicial = inicial;
		setMatrizfinal(inicial);
	};

	glm::mat4 getMatInicial();

	glm::mat4 getMatFinal();

	void draw(GLuint textura, GLuint texturaLateral);

	virtual void drawing(GLuint textura, GLuint texturaLateral);

	void calcularPathPoints(std::vector<Point3D> &path);

	virtual void writeToFile(std::ostream& o);

};



class Looping : public Tramo
{
protected:
	float _radio;
	float _separacion;
	const int  _id = 7;
	virtual void setMatrizfinal(glm::mat4 matini);
public:

	Looping() : _radio(),_separacion() {};

	//Codiciones para que este tramo sea valido
	Looping(float ancho,float separacion, float radio, glm::mat4 inicial, int res = 1, int repitetex = 1)
	{
		// El radio debe ser igual que el ancho
		if (radio < ancho*2) _radio = ancho*2;
		else _radio = radio;

		if (ancho*1.5 > abs(separacion)) {
			_separacion = ancho * 1.5;
		}
		else {
			_separacion = separacion;
		}
		_ancho = max(0, ancho);
		_res = min(max(1, res), 100);
		_texX = max(1, repitetex);
		_matinicial = inicial;

		setMatrizfinal(inicial);

	};

	glm::mat4 getMatInicial();

	glm::mat4 getMatFinal();

	void draw(GLuint textura, GLuint texturaLateral);

	virtual void drawing(GLuint textura, GLuint texturaLateral);

	virtual void calcularPathPoints(std::vector<Point3D> &path);

	virtual void writeToFile(std::ostream& o);

};




