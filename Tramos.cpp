#include "Tramos.h"
#include <iostream>

//TODO completar las funciones de calcularPathPoints para cada tramo

/*
Metodos de la clase Tramo
*/

void Tramo::setMatrizfinal(glm::mat4 matini) {
	_matfinal = matini * glm::translate(glm::mat4(1), glm::vec3(_longitud, 0.0f, 0.0f));
}

glm::mat4 Tramo::getMatInicial() {
	return _matinicial;
}

glm::mat4 Tramo::getMatFinal() {
	return _matfinal;
}

std::vector<Point3D*> Tramo::calcularPathPoints() {
	vector<Point3D*> puntos;
	return puntos;
}

void Tramo::draw(GLuint textura, GLuint texturaLateral) {
	// Se construye un quadtex con la resolucion pedida para que los 
	// miniquads salgan cuadrados. La textura se repite a lo largo del 
	// quad, no a lo ancho
	// El tramo se dibuja en el s.r. del modelo hacia x+
	// Al final mueve el s.r. al final del tramo

	Point3D v0(0, 0, -_ancho / 2);
	Point3D v1(0, 0, _ancho / 2);
	Point3D v3(_longitud, 0, -_ancho / 2);
	Point3D v2(_longitud, 0, _ancho / 2);

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
	quadtex(v0, v1, v2, v3,
		0, _texX, 0, _texX, int(_ancho*resXmetro), int(_longitud*resXmetro));

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


	Point3D l0(0, 0, -_ancho / 2);
	Point3D l1(0, -_ancho * 0.15, -_ancho / 2);
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

	quadtex(b0, b1, b2, b3,
		0, _texX, 0, _texX, int(_ancho*resXmetro), int(_longitud*resXmetro));


	Point3D f0(_longitud, 0, _ancho / 2);
	Point3D f1(_longitud, -_ancho * 0.15, _ancho / 2);
	Point3D f3(_longitud, 0, -_ancho / 2);
	Point3D f2(_longitud, -_ancho * 0.15, -_ancho / 2);

	quadtex(f0, f1, f2, f3,
		0, _texX, 0, _texX, int(_ancho*resXmetro), int(_longitud*resXmetro));

	glPopAttrib();


	glTranslatef(_longitud, 0, 0);




}


void Tramo::setRes(GLint res) {
	_res = res;
}


void Tramo::drawing(GLuint textura, GLuint texturaLateral) {
	this->draw(textura, texturaLateral);
}


/*
Metodo que escribirá en el output stream dado, los parametros necesarios para pintar la pieza, la resolucion y las repeticiones vendrán dadas por las opciones
*/
void  Tramo::writeToFile(std::ostream& o) {
	o << _id << ';' << _ancho << ';' << _longitud << '\n';
}



/*
Metodos de la clase TramoCurvo
*/
std::vector<Point3D*> TramoCurvo::calcularPathPoints() {
	vector<Point3D*> puntos;
	return puntos;
}

void TramoCurvo::setMatrizfinal(glm::mat4 matini) {
	float radio = abs(_longitud / rad(_angulo));
	float angulo = rad(_angulo);
	glm::mat4 aux;
	aux = matini * glm::translate(glm::mat4(1), glm::vec3(0.0f, 0.0f, -signo(_angulo) * radio));
	aux = aux * glm::rotate(glm::mat4(1), angulo, glm::vec3(0.0f, 1.0f, 0.0f));
	aux = aux * glm::translate(glm::mat4(1), glm::vec3(0.0f, 0.0f, signo(_angulo) * radio));
	_matfinal = aux;
}

glm::mat4 TramoCurvo::getMatInicial() {
	return _matinicial;
}

glm::mat4 TramoCurvo::getMatFinal() {
	return _matfinal;
}



void TramoCurvo::draw(GLuint textura, GLuint texturaLateral)
{
	// Dibuja una curva a la izquierda cuando el angulo es positivo y a la derecha cuando negativo

	if (_angulo == 0) return;
	float radio = abs(_longitud / rad(_angulo));

	// Dibuja tantos quads como la resolución pedida
	// Cada quad se construye a la resolucion _res a lo ancho
	// La textura en t se reparte entre los quads

	float angulo_quad = rad(abs(_angulo)) / _res;

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

	if (_angulo < 0) glScalef(1, 1, -1);				// curva a derechas
	glTranslatef(0, 0, -radio);

	Point3D v0(0, 0, radio - _ancho / 2);
	Point3D v1(0, 0, radio + _ancho / 2);
	Point3D v3;
	Point3D v2;


	//parte inferior
	Point3D lo0(0, -_ancho * 0.15, radio - _ancho / 2);
	Point3D lo1(0, -_ancho * 0.15, radio + _ancho / 2);
	Point3D lo3;
	Point3D lo2;

	lo3.y = -_ancho * 0.15;
	lo2.y = -_ancho * 0.15;


	//parte lateral izquierda
	Point3D l0(0, 0, radio - _ancho / 2);
	Point3D l1(0, -_ancho * 0.15, radio - _ancho / 2);
	Point3D l3;
	Point3D l2;
	l2.y -= _ancho * 0.15;


	Point3D r0(0, 0, radio + _ancho / 2);
	Point3D r1(0, -_ancho * 0.15, radio + _ancho / 2);
	Point3D r3;
	Point3D r2;
	r2.y -= _ancho * 0.15;

	for (int i = 0; i < _res; i++) {
		//Uso de las texturas
		glBindTexture(GL_TEXTURE_2D, textura);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		//parte superior	
		v3.x = (radio - _ancho / 2) * sin((i + 1)*angulo_quad);
		v3.z = (radio - _ancho / 2) * cos((i + 1)*angulo_quad);
		v2.x = (radio + _ancho / 2) * sin((i + 1)*angulo_quad);
		v2.z = (radio + _ancho / 2) * cos((i + 1)*angulo_quad);
		// Al hacer reflexion con glScale(1,1,-1) el quad hay que darlo en horario para que quede en antihorario

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
		lo2.x = v2.x;
		lo2.z = v2.z;

		quadtex(lo0, lo1, lo2, lo3,
			0, 1, i*float(_texX) / _res, (i + 1)*float(_texX) / _res, _res, 1);

		//laterales
		l3.x = v3.x;
		l3.z = v3.z;
		l2.x = v3.x;
		l2.z = v3.z;

		quadtex(l0, l1, l2, l3, 0, 1, i*float(_texX) / _res, (i + 1)*float(_texX) / _res, _res, 1);

		r3.x = v2.x;
		r3.z = v2.z;
		r2.x = v2.x;
		r2.z = v2.z;

		quadtex(r0, r1, r2, r3, 0, 1, i*float(_texX) / _res, (i + 1)*float(_texX) / _res, _res, 1);

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
	glTranslatef(0, 0, -signo(_angulo) * radio);
	glRotatef(_angulo, 0, 1, 0);
	glTranslatef(0, 0, signo(_angulo) * radio);


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

void  TramoCurvo::drawing(GLuint textura, GLuint texturaLateral) {
	draw(textura, texturaLateral);
}

/*
Metodo que escribirá en el output stream dado, los parametros necesarios para pintar la pieza, la resolucion y las repeticiones vendrán dadas por las opciones
*/
void  TramoCurvo::writeToFile(std::ostream& o) {

	o << _id << ';' << _ancho << ';' << _longitud << ';' << _angulo << '\n';

}


/*
Metodos de la clase Rampa
*/

void Rampa::setMatrizfinal(glm::mat4 matini) {
	_matfinal = matini * glm::translate(glm::mat4(1), glm::vec3(_longitud, _longitud*_pendiente, 0.0f));
}

glm::mat4 Rampa::getMatInicial() {
	return _matinicial;
}

glm::mat4 Rampa::getMatFinal() {
	return _matfinal;
}

std::vector<Point3D*> Rampa::calcularPathPoints() {
	vector<Point3D*> puntos;
	return puntos;
}


void Rampa::draw(GLuint textura, GLuint texturaLateral) 
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

void Rampa::drawing(GLuint textura, GLuint texturaLateral) {
	draw(textura, texturaLateral);
}

/*
Metodo que escribirá en el output stream dado, los parametros necesarios para pintar la pieza, la resolucion y las repeticiones vendrán dadas por las opciones
*/
void Rampa::writeToFile(std::ostream& o) {

	o << _id << ';' << _ancho << ';' << _longitud << ';' << _pendiente << '\n';

}


/*
Metodos de la clase RampaCurva
*/

std::vector<Point3D*> RampaCurva::calcularPathPoints() {
	vector<Point3D*> puntos;
	return puntos;
}

void RampaCurva::setMatrizfinal(glm::mat4 matini) {
	float radio = abs(_longitud / rad(_angulo));
	float angulo = rad(_angulo);
	glm::mat4 aux;
	aux = matini * glm::translate(glm::mat4(1), glm::vec3(0.0f, 0.0f, -signo(_angulo) * radio));
	aux = aux * glm::rotate(glm::mat4(1), angulo, glm::vec3(0.0f, 1.0f, 0.0f));
	aux = aux * glm::translate(glm::mat4(1), glm::vec3(0.0f, _longitud*_pendiente, signo(_angulo) * radio));
	_matfinal = aux;
}

glm::mat4 RampaCurva::getMatInicial() {
	return _matinicial;
}

glm::mat4 RampaCurva::getMatFinal() {
	return _matfinal;
}




void RampaCurva::draw(GLuint textura, GLuint texturaLateral)
{
	// Dibuja una curva a la izquierda cuando el angulo es positivo y a la derecha cuando negativo

	if (_angulo == 0) return;
	float radio = abs(_longitud / rad(_angulo));

	// Dibuja tantos quads como la resolución pedida
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

		float inc = (float)(i + 1) / (float)_res;

		v3.x = (radio - _ancho / 2) * sin((i + 1)*angulo_quad);
		v3.y = altura * inc;

		v3.z = (radio - _ancho / 2) * cos((i + 1)*angulo_quad);
		v2.x = (radio + _ancho / 2) * sin((i + 1)*angulo_quad);
		v2.y = altura * inc;
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

void RampaCurva::drawing(GLuint textura, GLuint texturaLateral) {
	draw(textura, texturaLateral);
}

/*
Metodo que escribirá en el output stream dado, los parametros necesarios para pintar la pieza, la resolucion y las repeticiones vendrán dadas por las opciones
*/
void RampaCurva::writeToFile(std::ostream& o) {

	o << _id << ';' << _ancho << ';' << _longitud << ';' << _angulo << ';' << _pendiente << '\n';
}


/*
Metodos de la clase TramoSinuosoHorizontal
*/

std::vector<Point3D*> TramoSinuosoHorizontal::calcularPathPoints() {
	vector<Point3D*> puntos;
	return puntos;
}

void TramoSinuosoHorizontal::setMatrizfinal(glm::mat4 matini) {
	float ondulacion_quad = rad(abs(_ondulacion * 180)) / _res;
	int ondulacion_inicial_quad = (_orientacion ? 90 : 270);


	_matfinal = matini * glm::translate(glm::mat4(1), glm::vec3(_longitud, 0.0f, _potencia *cos(((_res)*ondulacion_quad) - rad(abs(ondulacion_inicial_quad)))));
}

glm::mat4 TramoSinuosoHorizontal::getMatInicial() {
	return _matinicial;
}

glm::mat4 TramoSinuosoHorizontal::getMatFinal() {
	return _matfinal;
}


void TramoSinuosoHorizontal::draw(GLuint textura, GLuint texturaLateral)
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
		v3.y = 0;
		v3.z = (-_ancho / 2) + _potencia * cos(((i + 1)*ondulacion_quad) - rad(abs(ondulacion_inicial_quad)));

		v2.x = inc * _longitud;
		v2.y = 0;
		v2.z = (_ancho / 2) + _potencia * cos(((i + 1)*ondulacion_quad) - rad(abs(ondulacion_inicial_quad)));
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

 void TramoSinuosoHorizontal::drawing(GLuint textura, GLuint texturaLateral) {
	draw(textura, texturaLateral);
}

/*
Metodo que escribirá en el output stream dado, los parametros necesarios para pintar la pieza, la resolucion y las repeticiones vendrán dadas por las opciones
*/
 void TramoSinuosoHorizontal::writeToFile(std::ostream& o) {

	o << _id << ';' << _ancho << ';' << _longitud << ';' << _ondulacion << ';' << _potencia << ';' << _orientacion << '\n';

}

 /*
 Metodos de la clase TramoSinuosoVertical
 */

 std::vector<Point3D*> TramoSinuosoVertical::calcularPathPoints() {
	 vector<Point3D*> puntos;
	 return puntos;
 }


 void TramoSinuosoVertical::setMatrizfinal(glm::mat4 matini) {
	 float ondulacion_quad = rad(abs(_ondulacion * 180)) / _res;
	 int ondulacion_inicial_quad = (_orientacion ? 90 : 270);
	 _matfinal = matini * glm::translate(glm::mat4(1), glm::vec3(_longitud, _potencia*cos(((_res)*ondulacion_quad) - rad(abs(ondulacion_inicial_quad))), 0.0f));
 }

 glm::mat4 TramoSinuosoVertical::getMatInicial() {
	 return _matinicial;
 }

 glm::mat4 TramoSinuosoVertical::getMatFinal() {
	 return _matfinal;
 }


 void TramoSinuosoVertical::draw(GLuint textura, GLuint texturaLateral) 
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
		 v3.y = _potencia * cos(((i + 1)*ondulacion_quad) - rad(abs(ondulacion_inicial_quad)));
		 v3.z = (-_ancho / 2);

		 v2.x = inc * _longitud;
		 v2.y = _potencia * cos(((i + 1)*ondulacion_quad) - rad(abs(ondulacion_inicial_quad)));
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

 void TramoSinuosoVertical::drawing(GLuint textura, GLuint texturaLateral) {
	 draw(textura, texturaLateral);
 }

 void TramoSinuosoVertical::writeToFile(std::ostream& o) {

	 o << _id << ';' << _ancho << ';' << _longitud << ';' << _ondulacion << ';' << _potencia << ';' << _orientacion << '\n';

 }


 /*
 Metodos de la clase Looping
 */

 std::vector<Point3D*> Looping::calcularPathPoints() {
	 vector<Point3D*> puntos;
	 return puntos;
 }

 void Looping::setMatrizfinal(glm::mat4 matini) {
	 _matfinal = matini * glm::translate(glm::mat4(1), glm::vec3(0.0f, 0.0f, _separacion));
 }

 glm::mat4 Looping::getMatInicial() {
	 return _matinicial;
 }

 glm::mat4 Looping::getMatFinal() {
	 return _matfinal;
 }


 void Looping::draw(GLuint textura, GLuint texturaLateral)
 {


	 // Dibuja tantos quads como la resolución pedida
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


		 v3.x = (_radio * cos(((i + 1)*angulo_quad) - rad(abs(90))));
		 v3.y = (_radio * sin(((i + 1)*angulo_quad) - rad(abs(90)))) + _radio;
		 v3.z = (-_ancho / 2) + inc * _separacion;


		 v2.x = (_radio * cos(((i + 1)*angulo_quad) - rad(abs(90))));
		 v2.y = (_radio * sin(((i + 1)*angulo_quad) - rad(abs(90)))) + _radio;
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
		 float aux = _radio + (0.15*_ancho);

		 //parte inferior
		 lo3.x = (aux * cos(((i + 1)*angulo_quad) - rad(abs(90))));
		 lo3.y = -(0.15*_ancho) + (aux * sin(((i + 1)*angulo_quad) - rad(abs(90)))) + aux;
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

 void Looping::drawing(GLuint textura, GLuint texturaLateral) {
	 draw(textura, texturaLateral);
 }

 void Looping::writeToFile(std::ostream& o) {

	 o << _id << ';' << _ancho << ';' << _separacion << ';' << _radio << '\n';

 }