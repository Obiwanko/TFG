/***************************************************
ISGI::Crcuito estatico
Roberto Vivo', 2014 (v2.0)

Clases para la construccion de un circuito uniendo piezas
como las del scalextric.

Clases:
Poin3D: Tripleta de coordenadas
Tramo:  Geometr�a para un tramo recto de carretera
TramoCurvo: Geometria para una curva como arco de circunferencia



Dependencias: 
+utilidades.h
***************************************************/ 
#include "Utilidades.h"// Biblioteca de Utilidades
#include <cmath> // Biblioteca matematica de C

class Tramo
{
protected:
	int _res;								// resolucion
	int _texX;								// n� rep textura
	float _ancho, _longitud;				// ancho y longitud del tramo
	const int _id = 1;
public:
	//valores por defecto

	Tramo( ): _res(10), _texX(1), _ancho(1), _longitud(1) {};

	//Codiciones para que un tramo sea valido
	Tramo( float ancho, float longitud, int res = 1, int repitetex = 1 )
	{
		_ancho = max(0, ancho);
		_longitud = max(0, longitud);
		_res = min( max( 1, res) , 100 );
		_texX = max( 1, repitetex);

		
		
	};

	void draw(GLuint textura, GLuint texturaLateral) 
	{
		// Se construye un quadtex con la resolucion pedida para que los 
		// miniquads salgan cuadrados. La textura se repite a lo largo del 
		// quad, no a lo ancho
		// El tramo se dibuja en el s.r. del modelo hacia x+
		// Al final mueve el s.r. al final del tramo

		Point3D v0( 0, 0, -_ancho/2 );
		Point3D v1( 0, 0,  _ancho/2 );
		Point3D v3( _longitud, 0, -_ancho/2 );
		Point3D v2( _longitud, 0,  _ancho/2 );

		glPushAttrib(GL_ALL_ATTRIB_BITS);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D); //habilitamos textura
								 

		//Uso de las texturas
		glBindTexture(GL_TEXTURE_2D, textura);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);



		float resXmetro = _res / max( _longitud, _ancho );
		quadtex(  v0, v1, v2,v3,
			     0, _texX, 0, _texX, int(_ancho*resXmetro), int(_longitud*resXmetro) );

		glPopAttrib();
		

		//Dibujamos las otras placas de forma que los tramos sean figuras con volumen
		glPushAttrib(GL_ALL_ATTRIB_BITS);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D); //habilitamos textura
		glBindTexture(GL_TEXTURE_2D, texturaLateral);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		//parte de abajo

		v0.y -= _ancho * 0.15;
		v1.y -= _ancho * 0.15;
		v2.y -= _ancho * 0.15;
		v3.y -= _ancho * 0.15;
		
		//cambiamos la orientacion para que la luz se refleje correctamente
		quadtex(v1, v0,v3, v2,
			0, _texX, 0, _texX, int(_ancho*resXmetro), int(_longitud*resXmetro));


		Point3D l0(0, 0, -_ancho / 2);
		Point3D l1(0, -_ancho*0.15, -_ancho / 2);
		Point3D l3(_longitud, 0, -_ancho / 2);
		Point3D l2(_longitud, -_ancho * 0.15, -_ancho / 2);

		quadtex(l0, l1, l2, l3,
			0, _texX, 0, _texX, int(_ancho*resXmetro), int(_longitud*resXmetro));

		Point3D r0(0, 0, _ancho / 2);
		Point3D r1(0, -_ancho * 0.15, _ancho / 2);
		Point3D r3(_longitud, 0, _ancho / 2);
		Point3D r2(_longitud, -_ancho * 0.15, _ancho / 2);

		quadtex(r0, r1, r2, r3,
			0, _texX, 0, _texX, int(_ancho*resXmetro), int(_longitud*resXmetro));

		//finalmente dibujamos la tapa trasera y la tapa delantera

		Point3D b0(0, 0, _ancho / 2);
		Point3D b1(0, -_ancho * 0.15, _ancho / 2);
		Point3D b3(0, 0, -_ancho / 2);
		Point3D b2(0, -_ancho * 0.15, -_ancho / 2);

		quadtex(b0,b1, b2,b3,
			0, _texX, 0, _texX, int(_ancho*resXmetro), int(_longitud*resXmetro));


		Point3D f0(_longitud, 0, _ancho / 2);
		Point3D f1(_longitud, -_ancho * 0.15, _ancho / 2);
		Point3D f3(_longitud, 0, -_ancho / 2);
		Point3D f2(_longitud, -_ancho * 0.15, -_ancho / 2);

		quadtex(f0, f1, f2, f3,
			0, _texX, 0, _texX, int(_ancho*resXmetro), int(_longitud*resXmetro));

		glPopAttrib();


		glTranslatef( _longitud, 0, 0 );

		//actualizamos la variable posicion global
		//posicionUltimoTramo.x = posicionUltimoTramo.x + _longitud;

		

	};

	void setRes(GLint res) {
		_res = res;
	}

	virtual void drawing(GLuint textura, GLuint texturaLateral) {
		draw(textura, texturaLateral);
		//actualizamos la variable posicion global

	}

	/*
	Metodo que escribir� en el output stream dado, los parametros necesarios para pintar la pieza, la resolucion y las repeticiones vendr�n dadas por las opciones
	*/
	virtual void writeToFile(std::ostream& o) {
				
		o << _id << ';' << _ancho << ';' << _longitud << '\n';

	}

};

class TramoCurvo : public Tramo
{
protected:
	float _angulo;
	const int  _id = 2;
public:

	TramoCurvo() : _angulo() {};
	
	//Codiciones para que este tramo sea valido
	TramoCurvo( float ancho, float longitud, float grados, int res = 1, int repitetex = 1 ) : _angulo( grados ), Tramo( ancho, longitud, res, repitetex )
	{
		// El radio debe ser mayor que la mitad del ancho
		if( longitud < ancho*rad(abs(grados))/2 ) _longitud = ancho*rad(abs(grados))/2;
	};

	void draw(GLuint textura, GLuint texturaLateral)
	{
		// Dibuja una curva a la izquierda cuando el angulo es positivo y a la derecha cuando negativo

		if( _angulo == 0 ) return;
		float radio = abs( _longitud / rad(_angulo) );

		// Dibuja tantos quads como la resoluci�n pedida
		// Cada quad se construye a la resolucion _res a lo ancho
		// La textura en t se reparte entre los quads

		float angulo_quad = rad( abs( _angulo ) ) / _res;

		glPushMatrix();
		
		glPushAttrib(GL_ALL_ATTRIB_BITS);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D); //habilitamos textura

		//Parte trasera
		glBindTexture(GL_TEXTURE_2D, texturaLateral);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		Point3D b0(0, 0, _ancho / 2);
		Point3D b1(0, -_ancho * 0.15, _ancho / 2);
		Point3D b3(0, 0, -_ancho / 2);
		Point3D b2(0, -_ancho * 0.15, -_ancho / 2);

		quadtex(b0, b1, b2, b3,
			0, _texX, 0, _texX, 1, 1);

		if( _angulo < 0 ) glScalef( 1, 1, -1 );				// curva a derechas
		glTranslatef( 0, 0, -radio );

		Point3D v0( 0 , 0, radio - _ancho/2 );
		Point3D v1( 0 , 0, radio + _ancho/2 );
		Point3D v3;
		Point3D v2;


		//parte inferior
		Point3D lo0(0, -_ancho*0.15, radio - _ancho / 2);
		Point3D lo1(0, -_ancho * 0.15, radio + _ancho / 2);
		Point3D lo3;
		Point3D lo2;

		lo3.y = -_ancho * 0.15;
		lo2.y = -_ancho * 0.15;


		//parte lateral izquierda
		Point3D l0(0, 0, radio -_ancho / 2);
		Point3D l1(0, -_ancho * 0.15, radio -_ancho / 2);
		Point3D l3;
		Point3D l2;
		l2.y -= _ancho * 0.15;


		Point3D r0(0, 0, radio+ _ancho / 2);
		Point3D r1(0, -_ancho * 0.15, radio+ _ancho / 2);
		Point3D r3;
		Point3D r2;
		r2.y -= _ancho * 0.15;

		for( int i = 0; i < _res; i++ ){
			//Uso de las texturas
			glBindTexture(GL_TEXTURE_2D, textura);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			//parte superior	
			v3.x = (radio - _ancho/2) * sin( (i+1)*angulo_quad );
			v3.z = (radio - _ancho/2) * cos( (i+1)*angulo_quad );
			v2.x = (radio + _ancho/2) * sin( (i+1)*angulo_quad );
			v2.z = (radio + _ancho/2) * cos( (i+1)*angulo_quad );
			// Al hacer reflexion con glScale(1,1,-1) el quad hay que darlo en horario para que quede en antihorario
			
				quadtex( v0, v1, v2, v3,
				          0, 1, i*float(_texX)/_res, (i+1)*float(_texX)/_res, _res, 1 );
			
			//el resto de partes tienen la textura secundaria
			glBindTexture(GL_TEXTURE_2D, texturaLateral);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			
			//parte inferior
			lo3.x = v3.x;
			lo3.z = v3.z;
			lo2.x = v2.x;
			lo2.z = v2.z;

			quadtex(lo0, lo1, lo2, lo3,
					0, 1, i*float(_texX) / _res, (i + 1)*float(_texX) / _res, _res, 1);
			
			//laterales
			l3.x = v3.x;
			l3.z = v3.z;
			l2.x = v3.x;
			l2.z = v3.z;

			quadtex(l0, l1, l2, l3,0, 1, i*float(_texX) / _res, (i + 1)*float(_texX) / _res, _res, 1);

			r3.x = v2.x;
			r3.z = v2.z;
			r2.x = v2.x;
			r2.z = v2.z;

			quadtex(r0, r1, r2, r3,0, 1, i*float(_texX) / _res, (i + 1)*float(_texX) / _res, _res, 1);
			
			//actualizamos valores de los puntos anteriores

			//lateral izquierdo
			l0.x = l3.x; l0.z = l3.z;
			l1.x = l2.x; l1.z = l2.z;

			//lateral derecho
			r0.x = r3.x; r0.z = r3.z;
			r1.x = r2.x; r1.z = r2.z;

			//superior
			v0.x = v3.x; v0.z = v3.z;
			v1.x = v2.x; v1.z = v2.z;

			//inferior
			lo0.x = lo3.x; lo0.z = lo3.z;
			lo1.x = lo2.x; lo1.z = lo2.z;


			


		}
		glPopMatrix();

	
		// Situa el s.r. al final de tramo
		glTranslatef( 0, 0, -signo(_angulo) * radio );
		glRotatef( _angulo, 0,1,0 );
		glTranslatef( 0, 0, signo(_angulo) * radio );


		//bujamos la parte delantera aprovechando el cambio de posicion del translate

		//Parte trasera
		glBindTexture(GL_TEXTURE_2D, texturaLateral);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		quadtex(b0, b1, b2, b3,
			0, _texX, 0, _texX, 1, 1);
		glPopAttrib();
		//aplicamos los mismos cambios en nuestro vector de posicion
		//posicionUltimoTramo.z +=  -signo(_angulo) * radio;
		//posicionUltimoTramo.x = (posicionUltimoTramo.x*cos(_angulo)) + (sin(_angulo)*posicionUltimoTramo.z);
		//posicionUltimoTramo.z = (posicionUltimoTramo.x*-sin(_angulo)) + (cos(_angulo)*posicionUltimoTramo.z);
		//posicionUltimoTramo.z += signo(_angulo) * radio;

	};

	virtual void drawing(GLuint textura, GLuint texturaLateral) {
		draw(textura, texturaLateral);
	}

	/*
	Metodo que escribir� en el output stream dado, los parametros necesarios para pintar la pieza, la resolucion y las repeticiones vendr�n dadas por las opciones
	*/
	virtual void writeToFile(std::ostream& o) {

		o << _id << ';' << _ancho << ';' << _longitud << ';' << _angulo << '\n';

	}

};

class Rampa : public Tramo
{
protected:
	float _pendiente;				// ancho y longitud del tramo
	const int  _id = 3;
public:
	//valores por defecto
	
	Rampa() : _pendiente(0.5) {}; 

	//Codiciones para que un tramo sea valido
	Rampa(float ancho, float longitud, float pendiente = 0.5, int res = 1, int repitetex = 1 )
	{
		_ancho = max(0, ancho);
		_longitud = max(0, longitud);
		_res = min(max(1, res), 100);
		_texX = max(1, repitetex);
		_pendiente = min(pendiente, 1.5);


	};

	void draw(GLuint textura, GLuint texturaLateral) const
	{
		// Se construye un quadtex con la resolucion pedida para que los 
		// miniquads salgan cuadrados. La textura se repite a lo largo del 
		// quad, no a lo ancho
		// El tramo se dibuja en el s.r. del modelo hacia x+
		// Al final mueve el s.r. al final del tramo

		float altura = _longitud * _pendiente;

		Point3D v0(0, 0, -_ancho / 2);
		Point3D v1(0, 0, _ancho / 2);
		Point3D v3(_longitud, altura, -_ancho / 2);
		Point3D v2(_longitud, altura, _ancho / 2);

		glPushAttrib(GL_ALL_ATTRIB_BITS);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D); //habilitamos textura
								 
								 //Uso de las texturas
		glBindTexture(GL_TEXTURE_2D, textura);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);


		float resXmetro = _res / max(_longitud, _ancho);
		quadtex(v0, v1, v2, v3, 0, _texX, 0, _texX, int(_ancho*resXmetro), int(_longitud*resXmetro));
		
		glPopAttrib();
		

		//Dibujamos las otras placas de forma que los tramos sean figuras con volumen
		glPushAttrib(GL_ALL_ATTRIB_BITS);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D); //habilitamos textura
		glBindTexture(GL_TEXTURE_2D, texturaLateral);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		//parte de abajo

		v0.y -= _ancho * 0.15;
		v1.y -= _ancho * 0.15;
		v2.y -= _ancho * 0.15;
		v3.y -= _ancho * 0.15;

		//cambiamos la orientacion para que la luz se refleje correctamente
		quadtex(v1, v0, v3, v2,
			0, _texX, 0, _texX, int(_ancho*resXmetro), int(_longitud*resXmetro));

		

		//laterales
		Point3D l0(0, 0, -_ancho / 2);
		Point3D l1(0, -_ancho * 0.15, -_ancho / 2);
		Point3D l3(_longitud, altura, -_ancho / 2);
		Point3D l2(_longitud, altura - _ancho * 0.15, -_ancho / 2);

		quadtex(l0, l1, l2, l3,
			0, _texX, 0, _texX, int(_ancho*resXmetro), int(_longitud*resXmetro));

		Point3D r0(0, 0, _ancho / 2);
		Point3D r1(0, -_ancho * 0.15, _ancho / 2);
		Point3D r3(_longitud, altura, _ancho / 2);
		Point3D r2(_longitud, altura - _ancho * 0.15, _ancho / 2);

		quadtex(r0, r1, r2, r3,
			0, _texX, 0, _texX, int(_ancho*resXmetro), int(_longitud*resXmetro));




		//finalmente dibujamos la tapa trasera y la tapa delantera

		Point3D b0(0, 0, _ancho / 2);
		Point3D b1(0, -_ancho * 0.15, _ancho / 2);
		Point3D b3(0, 0, -_ancho / 2);
		Point3D b2(0, -_ancho * 0.15, -_ancho / 2);

		quadtex(b0, b1, b2, b3,
			0, _texX, 0, _texX, int(_ancho*resXmetro), int(_longitud*resXmetro));


		Point3D f0(_longitud, altura, _ancho / 2);
		Point3D f1(_longitud, altura - _ancho * 0.15, _ancho / 2);
		Point3D f3(_longitud, altura, -_ancho / 2);
		Point3D f2(_longitud, altura - _ancho * 0.15, -_ancho / 2);

		quadtex(f0, f1, f2, f3,
			0, _texX, 0, _texX, int(_ancho*resXmetro), int(_longitud*resXmetro));
		


		glPopAttrib();

		glTranslatef(_longitud, altura, 0);
		//posicionUltimoTramo.x = posicionUltimoTramo.x + _longitud;
		//posicionUltimoTramo.y = posicionUltimoTramo.y + _longitud * _pendiente;
	};

	virtual void drawing(GLuint textura, GLuint texturaLateral) {
		draw(textura, texturaLateral);
	}

	/*
	Metodo que escribir� en el output stream dado, los parametros necesarios para pintar la pieza, la resolucion y las repeticiones vendr�n dadas por las opciones
	*/
	virtual void writeToFile(std::ostream& o) {

		o << _id << ';' << _ancho << ';' << _longitud << ';' << _pendiente << '\n';

	}

};


class RampaCurva : public TramoCurvo
{
protected:
	float _angulo;
	float _pendiente;
	const int  _id = 4;
public:

	RampaCurva() : _angulo() {};

	//Codiciones para que este tramo sea valido
	RampaCurva(float ancho, float longitud, float grados, float pendiente = 0.5, int res = 10, int repitetex = 1 ) : _angulo(grados), TramoCurvo(ancho, longitud, grados, res, repitetex)
	{
		// El radio debe ser mayor que la mitad del ancho
		//if (longitud < ancho*rad(abs(grados)) / 2) _longitud = ancho * rad(abs(grados)) / 2;
		_pendiente = min(pendiente, 1.5);
	};

	void draw(GLuint textura, GLuint texturaLateral)
	{
		// Dibuja una curva a la izquierda cuando el angulo es positivo y a la derecha cuando negativo

		if (_angulo == 0) return;
		float radio = abs(_longitud / rad(_angulo));

		// Dibuja tantos quads como la resoluci�n pedida
		// Cada quad se construye a la resolucion _res a lo ancho
		// La textura en t se reparte entre los quads

		float angulo_quad = rad(abs(_angulo)) / _res;
		float altura = _longitud * _pendiente;

		glPushAttrib(GL_ALL_ATTRIB_BITS);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D); //habilitamos textura

								 //Parte trasera
		glBindTexture(GL_TEXTURE_2D, texturaLateral);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		Point3D b0(0, 0, _ancho / 2);
		Point3D b1(0, -_ancho * 0.15, _ancho / 2);
		Point3D b3(0, 0, -_ancho / 2);
		Point3D b2(0, -_ancho * 0.15, -_ancho / 2);

		quadtex(b0, b1, b2, b3,
			0, _texX, 0, _texX, 1, 1);

		glPushMatrix();
		if (_angulo < 0) glScalef(1, 1, -1);				// curva a derechas
		glTranslatef(0, 0, -radio);

		//parte superior
		Point3D v0(0, 0, radio - _ancho / 2);
		Point3D v1(0, 0, radio + _ancho / 2);
		Point3D v3;
		Point3D v2;



		//parte inferior
		Point3D lo0(0, -_ancho * 0.15, radio - _ancho / 2);
		Point3D lo1(0, -_ancho * 0.15, radio + _ancho / 2);
		Point3D lo3;
		Point3D lo2;


		//parte lateral izquierda
		Point3D l0(0, 0, radio - _ancho / 2);
		Point3D l1(0, -_ancho * 0.15, radio - _ancho / 2);
		Point3D l3;
		Point3D l2;


		Point3D r0(0, 0, radio + _ancho / 2);
		Point3D r1(0, -_ancho * 0.15, radio + _ancho / 2);
		Point3D r3;
		Point3D r2;
	


		for (int i = 0; i < _res; i++) {
			
			float inc = (float)(i+1) / (float)_res;
			
			v3.x = (radio - _ancho / 2) * sin((i + 1)*angulo_quad);
			v3.y = altura* inc;
	
			v3.z = (radio - _ancho / 2) * cos((i + 1)*angulo_quad);
			v2.x = (radio + _ancho / 2) * sin((i + 1)*angulo_quad);
			v2.y = altura *inc ;
			v2.z = (radio + _ancho / 2) * cos((i + 1)*angulo_quad);

			// Al hacer reflexion con glScale(1,1,-1) el quad hay que darlo en horario para que quede en antihorario
			glBindTexture(GL_TEXTURE_2D, textura);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			quadtex(v0, v1, v2, v3,
				0, 1, i*float(_texX) / _res, (i + 1)*float(_texX) / _res, _res, 1);
	

			//el resto de partes tienen la textura secundaria
			glBindTexture(GL_TEXTURE_2D, texturaLateral);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

			//parte inferior
			lo3.x = v3.x;
			lo3.z = v3.z;
			lo3.y = v3.y - (_ancho * 0.15);
			lo2.x = v2.x;
			lo2.z = v2.z;
			lo2.y = v2.y - (_ancho * 0.15);

			quadtex(lo0, lo1, lo2, lo3,0, 1, i*float(_texX) / _res, (i + 1)*float(_texX) / _res, _res, 1);

			//laterales
			l3.x = v3.x;
			l3.z = v3.z;
			l3.y = v3.y;
			l2.x = v3.x;
			l2.z = v3.z;
			l2.y = v3.y - _ancho * 0.15;

			quadtex(l0, l1, l2, l3, 0, 1, i*float(_texX) / _res, (i + 1)*float(_texX) / _res, _res, 1);

			r3.x = v2.x;
			r3.z = v2.z;
			r3.y = v2.y;
			r2.x = v2.x;
			r2.z = v2.z;
			r2.y = v2.y - _ancho * 0.15;

			quadtex(r0, r1, r2, r3, 0, 1, i*float(_texX) / _res, (i + 1)*float(_texX) / _res, _res, 1);

			//actualizamos valores de los puntos anteriores

			//lateral izquierdo
			l0 = l3;
			l1 = l2;

			//lateral derecho
			r0 = r3; 
			r1 = r2; 

			//inferior
			lo0 = lo3; 
			lo1 = lo2;

			//superior
			v0 = v3; 
			v1 = v2;
		}
		glPopMatrix();

		// Situa el s.r. al final de tramo
		glTranslatef(0, 0, -signo(_angulo) * radio);
		glRotatef(_angulo, 0, 1, 0);
		glTranslatef(0, altura, signo(_angulo) * radio);
		
		//Parte trasera
		glBindTexture(GL_TEXTURE_2D, texturaLateral);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		quadtex(b0, b1, b2, b3,
			0, _texX, 0, _texX, 1, 1);
		
		glPopAttrib();
		//aplicamos los cambios al vector de posicion
		//posicionUltimoTramo.z += -signo(_angulo) * radio;
		//posicionUltimoTramo.x = (posicionUltimoTramo.x*cos(_angulo)) + (sin(_angulo)*posicionUltimoTramo.z);
		//posicionUltimoTramo.z = (posicionUltimoTramo.x*-sin(_angulo)) + (cos(_angulo)*posicionUltimoTramo.z);
		//posicionUltimoTramo.z += signo(_angulo) * radio;
		//posicionUltimoTramo.y += _longitud * _pendiente;

	};

	virtual void drawing(GLuint textura, GLuint texturaLateral) {
		draw(textura, texturaLateral);
	}

	/*
	Metodo que escribir� en el output stream dado, los parametros necesarios para pintar la pieza, la resolucion y las repeticiones vendr�n dadas por las opciones
	*/
	virtual void writeToFile(std::ostream& o) {

		o << _id << ';' << _ancho << ';' << _longitud << ';' << _angulo << ';' << _pendiente << '\n';

	}

};



class TramoSinuosoHorizontal : public Tramo
{
protected:
	float _ondulacion;				
	float _orientacion;
	float _potencia;
	const int  _id = 5;
public:
	//valores por defecto

	TramoSinuosoHorizontal() : _ondulacion(), _orientacion(){};

	//Codiciones para que un tramo sea valido
	TramoSinuosoHorizontal(float ancho, float longitud, float ondulacion, float potencia = 1, bool orientacion = true, int res = 10, int repitetex = 1 )
	{
		_ancho = max(0, ancho);
		_longitud = max(0, longitud);
		//minimo cinco quads para que se pueda mostrar la sinuosidad
		_res = min(max(5, res), 100);
		_texX = max(1, repitetex);
		//la ondulacion vendr� marcada un maximo de 1 ondulacion completa cada 4 metros
		_ondulacion = min(ondulacion, longitud/2);
		_orientacion = orientacion;
		//la potencia por  la que se multiplique la ondulacion no puede ser mayor de 1 cada 4 metros
		_potencia = min(potencia, longitud / 4);

	};

	void draw(GLuint textura, GLuint texturaLateral) const
	{
		// Se construye un quadtex con la resolucion pedida para que los 
		// miniquads salgan cuadrados. La textura se repite a lo largo del 
		// quad, no a lo ancho
		// El tramo se dibuja en el s.r. del modelo hacia x+
		// Al final mueve el s.r. al final del tramo

		//cada punto de ondulacion es media vuelta es decir giro a izquierda y giro a derecha 
		float ondulacion_quad = rad(abs(_ondulacion*180)) / _res;
		int ondulacion_inicial_quad = (_orientacion ? 90 : 270);
		

		Point3D v0(0, 0, -_ancho / 2);
		Point3D v1(0, 0, _ancho / 2);
		Point3D v3;
		Point3D v2;


		//parte inferior
		Point3D lo0(0, -_ancho * 0.15, - _ancho / 2);
		Point3D lo1(0, -_ancho * 0.15, + _ancho / 2);
		Point3D lo3;
		Point3D lo2;


		//parte lateral izquierda
		Point3D l0(0, 0,  - _ancho / 2);
		Point3D l1(0, -_ancho * 0.15,  - _ancho / 2);
		Point3D l3;
		Point3D l2;


		Point3D r0(0, 0,  _ancho / 2);
		Point3D r1(0, -_ancho * 0.15, _ancho / 2);
		Point3D r3;
		Point3D r2;

		glPushAttrib(GL_ALL_ATTRIB_BITS);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D); //habilitamos textura

		for (int i = 0; i < _res; i++) {

			float inc = (float)(i + 1) / (float)_res;


			v3.x = inc * _longitud;
			v3.y = 0;
			v3.z = (-_ancho / 2) + _potencia *cos(((i + 1)*ondulacion_quad) - rad(abs(ondulacion_inicial_quad)));

			v2.x = inc * _longitud;
			v2.y = 0;
			v2.z = (_ancho / 2) + _potencia *cos(((i + 1)*ondulacion_quad) - rad(abs(ondulacion_inicial_quad)));
			//Uso de las texturas
			glBindTexture(GL_TEXTURE_2D, textura);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			quadtex(v0, v1, v2, v3,
				0, 1, i*float(_texX) / _res, (i + 1)*float(_texX) / _res, _res, 1);

			//Activamos textura secundaria
			glBindTexture(GL_TEXTURE_2D, texturaLateral);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

			//parte inferior
			lo3.x = v3.x;
			lo3.z = v3.z;
			lo3.y = v3.y - (_ancho * 0.15);
			lo2.x = v2.x;
			lo2.z = v2.z;
			lo2.y = v2.y - (_ancho * 0.15);

			quadtex(lo0, lo1, lo2, lo3, 0, 1, i*float(_texX) / _res, (i + 1)*float(_texX) / _res, _res, 1);

			//laterales
			l3.x = v3.x;
			l3.z = v3.z;
			l3.y = v3.y;
			l2.x = v3.x;
			l2.z = v3.z;
			l2.y = v3.y - _ancho * 0.15;

			quadtex(l0, l1, l2, l3, 0, 1, i*float(_texX) / _res, (i + 1)*float(_texX) / _res, _res, 1);

			r3.x = v2.x;
			r3.z = v2.z;
			r3.y = v2.y;
			r2.x = v2.x;
			r2.z = v2.z;
			r2.y = v2.y - _ancho * 0.15;

			quadtex(r0, r1, r2, r3, 0, 1, i*float(_texX) / _res, (i + 1)*float(_texX) / _res, _res, 1);



			//lateral izquierdo
			l0 = l3;
			l1 = l2;

			//lateral derecho
			r0 = r3;
			r1 = r2;

			//inferior
			lo0 = lo3;
			lo1 = lo2;
			
			//superior
			v0.x = v3.x; v0.z = v3.z; v0.y = v3.y;
			v1.x = v2.x; v1.z = v2.z; v1.y = v3.y;
		}

		//tapa posterior
		Point3D b0(0, 0, _ancho / 2);
		Point3D b1(0, -_ancho * 0.15, _ancho / 2);
		Point3D b3(0, 0, -_ancho / 2);
		Point3D b2(0, -_ancho * 0.15, -_ancho / 2);

		quadtex(b0, b1, b2, b3,
			0, _texX, 0, _texX, 1, 1);


		
		glTranslatef(_longitud, 0, _potencia *cos(((_res)*ondulacion_quad) - rad(abs(ondulacion_inicial_quad))));

		//tapa frontal
		quadtex(b0, b1, b2, b3,
			0, _texX, 0, _texX, 1, 1);
		glPopAttrib();
		//posicionUltimoTramo.x = posicionUltimoTramo.x + _longitud;
	};

	virtual void drawing(GLuint textura, GLuint texturaLateral) {
		draw(textura, texturaLateral);
	}

	/*
	Metodo que escribir� en el output stream dado, los parametros necesarios para pintar la pieza, la resolucion y las repeticiones vendr�n dadas por las opciones
	*/
	virtual void writeToFile(std::ostream& o) {

		o << _id << ';' << _ancho << ';' << _longitud << ';' << _ondulacion << ';' << _potencia << ';' << _orientacion << '\n';

	}

};



class TramoSinuosoVertical : public Tramo
{
protected:
	float _ondulacion;
	float _orientacion;
	float _potencia;
	const int  _id = 6;
public:
	//valores por defecto

	TramoSinuosoVertical() : _ondulacion(), _orientacion() {};

	//Codiciones para que un tramo sea valido
	TramoSinuosoVertical(float ancho, float longitud, float ondulacion,float potencia = 1, bool orientacion = true, int res = 10, int repitetex = 1)
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
		//la ondulacion vendr� marcada un maximo de 1 ondulacion completa cada 4 metros
		_ondulacion = min(ondulacion, longitud / 2);
		_orientacion = orientacion;
		//la potencia por  la que se multiplique la ondulacion no puede ser mayor de 1 cada 4 metros
		_potencia = min(potencia, longitud/4);

	};

	void draw(GLuint textura, GLuint texturaLateral) const
	{
		// Se construye un quadtex con la resolucion pedida para que los 
		// miniquads salgan cuadrados. La textura se repite a lo largo del 
		// quad, no a lo ancho
		// El tramo se dibuja en el s.r. del modelo hacia x+
		// Al final mueve el s.r. al final del tramo

		//cada punto de ondulacion es media vuelta es decir giro a izquierda y giro a derecha 
		float ondulacion_quad = rad(abs(_ondulacion * 180)) / _res;
		int ondulacion_inicial_quad = (_orientacion ? 90 : 270);


		Point3D v0(0, 0, -_ancho / 2);
		Point3D v1(0, 0, _ancho / 2);
		Point3D v3;
		Point3D v2;

		//parte inferior
		Point3D lo0(0, -_ancho * 0.15, -_ancho / 2);
		Point3D lo1(0, -_ancho * 0.15, +_ancho / 2);
		Point3D lo3;
		Point3D lo2;


		//parte lateral izquierda
		Point3D l0(0, 0, -_ancho / 2);
		Point3D l1(0, -_ancho * 0.15, -_ancho / 2);
		Point3D l3;
		Point3D l2;


		Point3D r0(0, 0, _ancho / 2);
		Point3D r1(0, -_ancho * 0.15, _ancho / 2);
		Point3D r3;
		Point3D r2;

		glPushAttrib(GL_ALL_ATTRIB_BITS);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D); //habilitamos textura
								 



		for (int i = 0; i < _res; i++) {

			float inc = (float)(i + 1) / (float)_res;


			v3.x = inc * _longitud;
			v3.y = _potencia*cos(((i + 1)*ondulacion_quad) - rad(abs(ondulacion_inicial_quad)));
			v3.z = (-_ancho / 2);

			v2.x = inc * _longitud;
			v2.y = _potencia*cos(((i + 1)*ondulacion_quad) - rad(abs(ondulacion_inicial_quad)));
			v2.z = (_ancho / 2);
			//Uso de las texturas
			glBindTexture(GL_TEXTURE_2D, textura);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			quadtex(v0, v1, v2, v3,

				0, 1, i*float(_texX) / _res, (i + 1)*float(_texX) / _res, _res, 1);

			glBindTexture(GL_TEXTURE_2D, texturaLateral);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			//parte inferior
			lo3.x = v3.x;
			lo3.z = v3.z;
			lo3.y = v3.y - (_ancho * 0.15);
			lo2.x = v2.x;
			lo2.z = v2.z;
			lo2.y = v2.y - (_ancho * 0.15);

			quadtex(lo0, lo1, lo2, lo3, 0, 1, i*float(_texX) / _res, (i + 1)*float(_texX) / _res, _res, 1);

			//laterales
			l3.x = v3.x;
			l3.z = v3.z;
			l3.y = v3.y;
			l2.x = v3.x;
			l2.z = v3.z;
			l2.y = v3.y - _ancho * 0.15;

			quadtex(l0, l1, l2, l3, 0, 1, i*float(_texX) / _res, (i + 1)*float(_texX) / _res, _res, 1);

			r3.x = v2.x;
			r3.z = v2.z;
			r3.y = v2.y;
			r2.x = v2.x;
			r2.z = v2.z;
			r2.y = v2.y - _ancho * 0.15;

			quadtex(r0, r1, r2, r3, 0, 1, i*float(_texX) / _res, (i + 1)*float(_texX) / _res, _res, 1);



			//lateral izquierdo
			l0 = l3;
			l1 = l2;

			//lateral derecho
			r0 = r3;
			r1 = r2;

			//inferior
			lo0 = lo3;
			lo1 = lo2;


			v0.x = v3.x; v0.z = v3.z; v0.y = v3.y;
			v1.x = v2.x; v1.z = v2.z; v1.y = v3.y;
		}

		
		//tapa posterior
		Point3D b0(0, 0, _ancho / 2);
		Point3D b1(0, -_ancho * 0.15, _ancho / 2);
		Point3D b3(0, 0, -_ancho / 2);
		Point3D b2(0, -_ancho * 0.15, -_ancho / 2);

		quadtex(b0, b1, b2, b3,
			0, _texX, 0, _texX, 1, 1);

		glTranslatef(_longitud, _potencia*cos(((_res)*ondulacion_quad) - rad(abs(ondulacion_inicial_quad))), 0);
		//tapa frontal
		quadtex(b0, b1, b2, b3,
			0, _texX, 0, _texX, 1, 1);
		glPopAttrib();
		//posicionUltimoTramo.x = posicionUltimoTramo.x + _longitud;
	};

	virtual void drawing(GLuint textura, GLuint texturaLateral) {
		draw(textura, texturaLateral);
	}

	virtual void writeToFile(std::ostream& o) {

		o << _id << ';' << _ancho << ';' << _longitud << ';' << _ondulacion << ';' << _potencia << ';' << _orientacion << '\n';

	}

};



class Looping : public Tramo
{
protected:
	float _radio;
	float _separacion;
	const int  _id = 7;
public:

	Looping() : _radio(),_separacion() {};

	//Codiciones para que este tramo sea valido
	Looping(float ancho,float separacion, float radio, int res = 1, int repitetex = 1) : _separacion(separacion), _radio(), Tramo(ancho,1, res, repitetex)
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
		
	};

	void draw(GLuint textura, GLuint texturaLateral)
	{
		

		// Dibuja tantos quads como la resoluci�n pedida
		// Cada quad se construye a la resolucion _res a lo ancho
		// La textura en t se reparte entre los quads

		float angulo_quad = rad(abs(360)) / _res;

	
		Point3D v0(0, 0, -_ancho / 2);
		Point3D v1(0, 0, _ancho / 2);
		Point3D v3;
		Point3D v2;

		//parte inferior
		Point3D lo0(0, -_ancho * 0.15, -_ancho / 2);
		Point3D lo1(0, -_ancho * 0.15, +_ancho / 2);
		Point3D lo3;
		Point3D lo2;


		//parte lateral izquierda
		Point3D l0(0, 0, -_ancho / 2);
		Point3D l1(0, -_ancho * 0.15, -_ancho / 2);
		Point3D l3;
		Point3D l2;


		Point3D r0(0, 0, _ancho / 2);
		Point3D r1(0, -_ancho * 0.15, _ancho / 2);
		Point3D r3;
		Point3D r2;

		glPushAttrib(GL_ALL_ATTRIB_BITS);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D); //habilitamos textura



		
		for (int i = 0; i < _res; i++) {

			float inc = (float)(i + 1) / (float)_res;
			
		
			v3.x =    (_radio * cos(  ((i + 1)*angulo_quad) - rad(abs(90) )));
			v3.y = (_radio * sin(((i + 1)*angulo_quad) - rad(abs(90) ))) +_radio;
			v3.z = (-_ancho / 2) + inc * _separacion;


			v2.x = (_radio * cos(((i + 1)*angulo_quad) - rad(abs(90) ) ) );
			v2.y = (_radio * sin(((i + 1)*angulo_quad) - rad(abs(90) ) ) )+_radio;
			v2.z = (_ancho / 2) + inc * _separacion;
			//Uso de las texturas
			glBindTexture(GL_TEXTURE_2D, textura);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			quadtex(v0, v1, v2, v3,

					0, 1, i*float(_texX) / _res, (i + 1)*float(_texX) / _res, _res, 1);
		

			glBindTexture(GL_TEXTURE_2D, texturaLateral);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			float aux = _radio+(0.15*_ancho) ;

			//parte inferior
			lo3.x = (aux * cos(((i + 1)*angulo_quad) - rad(abs(90))));
			lo3.y = -(0.15*_ancho) +(aux * sin(((i + 1)*angulo_quad) - rad(abs(90)))) + aux;
			lo3.z = v3.z;

			lo2.x = (aux * cos(((i + 1)*angulo_quad) - rad(abs(90))));
			lo2.y = -(0.15*_ancho) + (aux * sin(((i + 1)*angulo_quad) - rad(abs(90)))) + aux;
			lo2.z = v2.z;
			quadtex(lo0, lo1, lo2, lo3, 0, 1, i*float(_texX) / _res, (i + 1)*float(_texX) / _res, _res, 1);

			//laterales
			l3.x = v3.x;
			l3.z = v3.z;
			l3.y = v3.y;
			l2.x = lo3.x;
			l2.z = lo3.z;
			l2.y = lo3.y;

			quadtex(l0, l1, l2, l3, 0, 1, i*float(_texX) / _res, (i + 1)*float(_texX) / _res, _res, 1);

			r3.x = v2.x;
			r3.z = v2.z;
			r3.y = v2.y;
			r2.x = lo2.x;
			r2.z = lo2.z;
			r2.y = lo2.y;

			quadtex(r0, r1, r2, r3, 0, 1, i*float(_texX) / _res, (i + 1)*float(_texX) / _res, _res, 1);



			//lateral izquierdo
			l0 = l3;
			l1 = l2;

			//lateral derecho
			r0 = r3;
			r1 = r2;

			//inferior
			lo0 = lo3;
			lo1 = lo2;


			v0.x = v3.x; v0.z = v3.z; v0.y = v3.y;
			v1.x = v2.x; v1.z = v2.z; v1.y = v3.y;
		}

	

		//tapa posterior
		Point3D b0(0, 0, _ancho / 2);
		Point3D b1(0, -_ancho * 0.15, _ancho / 2);
		Point3D b3(0, 0, -_ancho / 2);
		Point3D b2(0, -_ancho * 0.15, -_ancho / 2);

		quadtex(b0, b1, b2, b3,
			0, _texX, 0, _texX, 1, 1);

		glTranslatef(0, 0, _separacion);
		//tapa frontal
		quadtex(b0, b1, b2, b3,
			0, _texX, 0, _texX, 1, 1);
		glPopAttrib();
		//posicionUltimoTramo.z = posicionUltimoTramo.z + _separacion;
	};

	virtual void drawing(GLuint textura, GLuint texturaLateral) {
		draw(textura, texturaLateral);
	}

	virtual void writeToFile(std::ostream& o) {

		o << _id << ';' << _ancho << ';' << _separacion << ';' << _radio << '\n';

	}

};




