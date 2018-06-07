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
	int _texX;								// nº rep textura
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

	void draw( ) 
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

	virtual void drawing() {
		draw();
	}

	/*
	Metodo que escribirá en el output stream dado, los parametros necesarios para pintar la pieza, la resolucion y las repeticiones vendrán dadas por las opciones
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

	void draw()
	{
		// Dibuja una curva a la izquierda cuando el angulo es positivo y a la derecha cuando negativo

		if( _angulo == 0 ) return;
		float radio = abs( _longitud / rad(_angulo) );

		// Dibuja tantos quads como la resolución pedida
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
			
				quadtex( (GLfloat*)v0, (GLfloat*)v1, (GLfloat*)v2, (GLfloat*)v3,
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

	virtual void drawing() {
		draw();
	}

	/*
	Metodo que escribirá en el output stream dado, los parametros necesarios para pintar la pieza, la resolucion y las repeticiones vendrán dadas por las opciones
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

	void draw() const
	{
		// Se construye un quadtex con la resolucion pedida para que los 
		// miniquads salgan cuadrados. La textura se repite a lo largo del 
		// quad, no a lo ancho
		// El tramo se dibuja en el s.r. del modelo hacia x+
		// Al final mueve el s.r. al final del tramo

		Point3D v0(0, 0, -_ancho / 2);
		Point3D v1(0, 0, _ancho / 2);
		Point3D v3(_longitud, _longitud*_pendiente, -_ancho / 2);
		Point3D v2(_longitud, _longitud*_pendiente, _ancho / 2);

		float resXmetro = _res / max(_longitud, _ancho);
		quadtex((GLfloat*)v0, (GLfloat*)v1, (GLfloat*)v2, (GLfloat*)v3, 0, _texX, 0, _texX, int(_ancho*resXmetro), int(_longitud*resXmetro));

		glTranslatef(_longitud, _longitud*_pendiente, 0);

	};

	virtual void drawing() {
		draw();
	}

	/*
	Metodo que escribirá en el output stream dado, los parametros necesarios para pintar la pieza, la resolucion y las repeticiones vendrán dadas por las opciones
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

	void draw()
	{
		// Dibuja una curva a la izquierda cuando el angulo es positivo y a la derecha cuando negativo

		if (_angulo == 0) return;
		float radio = abs(_longitud / rad(_angulo));

		// Dibuja tantos quads como la resolución pedida
		// Cada quad se construye a la resolucion _res a lo ancho
		// La textura en t se reparte entre los quads

		float angulo_quad = rad(abs(_angulo)) / _res;

		glPushMatrix();
		if (_angulo < 0) glScalef(1, 1, -1);				// curva a derechas
		glTranslatef(0, 0, -radio);

		Point3D v0(0, 0, radio - _ancho / 2);
		Point3D v1(0, 0, radio + _ancho / 2);
		Point3D v3;
		Point3D v2;

		for (int i = 0; i < _res; i++) {
			
			float inc = (float)(i+1) / (float)_res;
			
			v3.x = (radio - _ancho / 2) * sin((i + 1)*angulo_quad);
			v3.y = _longitud * inc* _pendiente;
	
			v3.z = (radio - _ancho / 2) * cos((i + 1)*angulo_quad);
			v2.x = (radio + _ancho / 2) * sin((i + 1)*angulo_quad);
			v2.y = _longitud*inc * _pendiente;
			v2.z = (radio + _ancho / 2) * cos((i + 1)*angulo_quad);

			// Al hacer reflexion con glScale(1,1,-1) el quad hay que darlo en horario para que quede en antihorario
		
				quadtex((GLfloat*)v0, (GLfloat*)v1, (GLfloat*)v2, (GLfloat*)v3,
					
					0, 1, i*float(_texX) / _res, (i + 1)*float(_texX) / _res, _res, 1);
	

			v0.x = v3.x; v0.z = v3.z; v0.y = v3.y;
			v1.x = v2.x; v1.z = v2.z; v1.y = v3.y;
		}
		glPopMatrix();

		// Situa el s.r. al final de tramo
		glTranslatef(0, 0, -signo(_angulo) * radio);
		glRotatef(_angulo, 0, 1, 0);
		glTranslatef(0, _longitud*_pendiente, signo(_angulo) * radio);
	};

	virtual void drawing() {
		draw();
	}

	/*
	Metodo que escribirá en el output stream dado, los parametros necesarios para pintar la pieza, la resolucion y las repeticiones vendrán dadas por las opciones
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
		//la ondulacion vendrá marcada un maximo de 1 ondulacion completa cada 4 metros
		_ondulacion = min(ondulacion, longitud/2);
		_orientacion = orientacion;
		//la potencia por  la que se multiplique la ondulacion no puede ser mayor de 1 cada 4 metros
		_potencia = min(potencia, longitud / 4);

	};

	void draw() const
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

		for (int i = 0; i < _res; i++) {

			float inc = (float)(i + 1) / (float)_res;


			v3.x = inc * _longitud;
			v3.y = 0;
			v3.z = (-_ancho / 2) + _potencia *cos(((i + 1)*ondulacion_quad) - rad(abs(ondulacion_inicial_quad)));

			//cout << float(i * 2 * PI) / _res << " " ;

			v2.x = inc * _longitud;
			v2.y = 0;
			v2.z = (_ancho / 2) + _potencia *cos(((i + 1)*ondulacion_quad) - rad(abs(ondulacion_inicial_quad)));

			quadtex((GLfloat*)v0, (GLfloat*)v1, (GLfloat*)v2, (GLfloat*)v3,

				0, 1, i*float(_texX) / _res, (i + 1)*float(_texX) / _res, _res, 1);


			v0.x = v3.x; v0.z = v3.z; v0.y = v3.y;
			v1.x = v2.x; v1.z = v2.z; v1.y = v3.y;
		}

		glTranslatef(_longitud, 0, 0);

	};

	virtual void drawing() {
		draw();
	}

	/*
	Metodo que escribirá en el output stream dado, los parametros necesarios para pintar la pieza, la resolucion y las repeticiones vendrán dadas por las opciones
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
		_ancho = max(0, ancho);
		_longitud = max(0, longitud);
		//minimo cinco quads para que se pueda mostrar la sinuosidad
		_res = min(max(5, res), 100);
		_texX = max(1, repitetex);
		//la ondulacion vendrá marcada un maximo de 1 ondulacion completa cada 4 metros
		_ondulacion = min(ondulacion, longitud / 2);
		_orientacion = orientacion;
		//la potencia por  la que se multiplique la ondulacion no puede ser mayor de 1 cada 4 metros
		_potencia = min(potencia, longitud/4);

	};

	void draw() const
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

		for (int i = 0; i < _res; i++) {

			float inc = (float)(i + 1) / (float)_res;


			v3.x = inc * _longitud;
			v3.y = _potencia*cos(((i + 1)*ondulacion_quad) - rad(abs(ondulacion_inicial_quad)));
			v3.z = (-_ancho / 2);

			//cout << float(i * 2 * PI) / _res << " " ;

			v2.x = inc * _longitud;
			v2.y = _potencia*cos(((i + 1)*ondulacion_quad) - rad(abs(ondulacion_inicial_quad)));
			v2.z = (_ancho / 2);

			quadtex((GLfloat*)v0, (GLfloat*)v1, (GLfloat*)v2, (GLfloat*)v3,

				0, 1, i*float(_texX) / _res, (i + 1)*float(_texX) / _res, _res, 1);


			v0.x = v3.x; v0.z = v3.z; v0.y = v3.y;
			v1.x = v2.x; v1.z = v2.z; v1.y = v3.y;
		}

		glTranslatef(_longitud, 0, 0);

	};

	virtual void drawing() {
		draw();
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
		if (radio < ancho) _radio = ancho;
		else _radio = radio;
	};

	void draw()
	{
		

		// Dibuja tantos quads como la resolución pedida
		// Cada quad se construye a la resolucion _res a lo ancho
		// La textura en t se reparte entre los quads

		float angulo_quad = rad(abs(360)) / _res;

	
		Point3D v0(0, 0, -_ancho / 2);
		Point3D v1(0, 0, _ancho / 2);
		Point3D v3;
		Point3D v2;
		
		for (int i = 0; i < _res; i++) {

			float inc = (float)(i + 1) / (float)_res;
			
		
			v3.x =    (_radio * cos(  ((i + 1)*angulo_quad) - rad(abs(90) )));
			v3.y = (_radio * sin(((i + 1)*angulo_quad) - rad(abs(90) ))) +_radio;
			v3.z = (-_ancho / 2) + inc * _separacion;

			//cout << float(i * 2 * PI) / _res << " " ;

			v2.x = (_radio * cos(((i + 1)*angulo_quad) - rad(abs(90) ) ) );
			v2.y = (_radio * sin(((i + 1)*angulo_quad) - rad(abs(90) ) ) )+_radio;
			v2.z = (_ancho / 2) + inc * _separacion;

			quadtex((GLfloat*)v0, (GLfloat*)v1, (GLfloat*)v2, (GLfloat*)v3,

					0, 1, i*float(_texX) / _res, (i + 1)*float(_texX) / _res, _res, 1);
		

			v0.x = v3.x; v0.z = v3.z; v0.y = v3.y;
			v1.x = v2.x; v1.z = v2.z; v1.y = v3.y;
		}

		glTranslatef(0, 0, _separacion);
	};

	virtual void drawing() {
		draw();
	}

	virtual void writeToFile(std::ostream& o) {

		o << _id << ';' << _ancho << ';' << _separacion << ';' << _radio << '\n';

	}

};
