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
#include <Utilidades.h> // Biblioteca de Utilidades

class Point3D
{ 
public:
	GLfloat x, y, z;
	Point3D(): x(0),y(0),z(0){};
	Point3D( float cx, float cy, float cz ): x(cx),y(cy),z(cz) {};
	GLfloat& operator[](int i)
	{
		// Devuelve la coordenada con acceso indexado por referencia
		try{
			if( i == 0 )      return x;
			else if( i == 1 ) return y;
			else if( i == 2 ) return z;
			else throw 999;
		}
		catch( int error ){
			cerr << "Excepcion en Point3D: " << error << endl;
		}
	};
	operator GLfloat*()
	{
		// Permite hacer un cast para convertirlo en array GLfloat[3]
		GLfloat* v = new GLfloat[3];
		v[0] = x; v[1] = y; v[2] = z;
		return v;
	};
};

class Tramo
{
protected:
	int _res;								// resolucion
	int _texX;								// n� rep textura
	float _ancho, _longitud;				// ancho y longitud del tramo
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

	void draw( ) const
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

		float resXmetro = _res / max( _longitud, _ancho );
		quadtex( (GLfloat*) v0, (GLfloat*) v1, (GLfloat*) v2, (GLfloat*) v3,
			     0, _texX, 0, _texX, int(_ancho*resXmetro), int(_longitud*resXmetro) );

		glTranslatef( _longitud, 0, 0 );

	};
};

class TramoCurvo : public Tramo
{
protected:
	float _angulo;
public:
	TramoCurvo() : _angulo() {};
	
	//Codiciones para que este tramo sea valido
	TramoCurvo( float ancho, float longitud, float grados, int res = 1, int repitetex = 1 ) : _angulo( grados ), Tramo( ancho, longitud, res, repitetex )
	{
		// El radio debe ser mayor que la mitad del ancho
		if( longitud < ancho*rad(abs(grados))/2 ) _longitud = ancho*rad(abs(grados))/2;
	};

	void draw()
	{
		// Dibuja una curva a la izquierda cuando el angulo es positivo y a la derecha cuando negativo

		if( _angulo == 0 ) return;
		float radio = abs( _longitud / rad(_angulo) );

		// Dibuja tantos quads como la resoluci�n pedida
		// Cada quad se construye a la resolucion _res a lo ancho
		// La textura en t se reparte entre los quads

		float angulo_quad = rad( abs( _angulo ) ) / _res;

		glPushMatrix();
		if( _angulo < 0 ) glScalef( 1, 1, -1 );				// curva a derechas
		glTranslatef( 0, 0, -radio );

		Point3D v0( 0 , 0, radio - _ancho/2 );
		Point3D v1( 0 , 0, radio + _ancho/2 );
		Point3D v3;
		Point3D v2;

		for( int i = 0; i < _res; i++ ){
			v3.x = (radio - _ancho/2) * sin( (i+1)*angulo_quad );
			v3.z = (radio - _ancho/2) * cos( (i+1)*angulo_quad );
			v2.x = (radio + _ancho/2) * sin( (i+1)*angulo_quad );
			v2.z = (radio + _ancho/2) * cos( (i+1)*angulo_quad );

			// Al hacer reflexion con glScale(1,1,-1) el quad hay que darlo en horario para que quede en antihorario
			if( _angulo > 0 )
				quadtex( (GLfloat*)v0, (GLfloat*)v1, (GLfloat*)v2, (GLfloat*)v3,
				          0, 1, i*float(_texX)/_res, (i+1)*float(_texX)/_res, _res, 1 );
			else quadtex( (GLfloat*)v1, (GLfloat*)v0, (GLfloat*)v3, (GLfloat*)v2,
						  0, 1, i*float(_texX)/_res, (i+1)*float(_texX)/_res, _res, 1 );

			v0.x = v3.x; v0.z = v3.z;
			v1.x = v2.x; v1.z = v2.z;
		}
		glPopMatrix();

		// Situa el s.r. al final de tramo
		glTranslatef( 0, 0, -signo(_angulo) * radio );
		glRotatef( _angulo, 0,1,0 );
		glTranslatef( 0, 0, signo(_angulo) * radio );
	};

};