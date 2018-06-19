#include "Entorno.h"
#include "Utilidades.h"


void drawLeg(GLfloat ancho, GLfloat largo, GLfloat profundo, GLint texX, GLint res) {
	GLfloat medioancho = ancho / 2;
	GLfloat mediolargo = largo / 2;

	Point3D w0(0, -profundo, 0);
	Point3D w1(0, -profundo - medioancho, 0);
	Point3D w3(0, -profundo, 0);
	Point3D w2(0, -profundo - medioancho, 0);

	float anguloquad = rad(abs(360)) / res;
	float radio = min(largo, ancho) / 8.0;
	for (int i = 0; i < res; i++) {

		float inc = (float)(i + 1) / (float)res;


		w3.x = radio * cos(((i + 1)*anguloquad) - rad(abs(90)));
		w3.z = (radio * sin(((i + 1)*anguloquad) - rad(abs(90)))) + radio;


		w2.x = (radio * cos(((i + 1)*anguloquad) - rad(abs(90))));
		w2.z = (radio * sin(((i + 1)*anguloquad) - rad(abs(90)))) + radio;

		quadtex(w0, w1, w2, w3, 0, 1, i*float(texX) / res, (i + 1)*float(texX) / res, res, 1);


		w0.x = w3.x; w0.z = w3.z;
		w1.x = w2.x; w1.z = w2.z;
	}
}

void Entorno::drawDesk(GLuint textura, GLfloat ancho, GLfloat largo, GLfloat profundo) {

	glPushMatrix();

	glTranslatef(0, -1.5, 0);

	GLfloat medioancho = ancho / 2;
	GLfloat mediolargo = largo / 2;

	Point3D v0(-mediolargo, 0, -medioancho);
	Point3D v1(-mediolargo, 0, medioancho);
	Point3D v3(mediolargo, 0, -medioancho);
	Point3D v2(mediolargo, 0, medioancho);

	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D); //habilitamos textura


							 //Uso de las texturas
	glBindTexture(GL_TEXTURE_2D, textura);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);



	float resXmetro = _res / max(largo, ancho);
	quadtex(v0, v1, v2, v3,
		0, _texX, 0, _texX, int(ancho*resXmetro), int(largo*resXmetro));

	//parte de abajo

	v0.y -= profundo;
	v1.y -= profundo;
	v2.y -= profundo;
	v3.y -= profundo;

	//cambiamos la orientacion para que la luz se refleje correctamente
	quadtex(v1, v0, v3, v2,
		0, _texX, 0, _texX, int(ancho*resXmetro), int(largo*resXmetro));


	Point3D l0(-mediolargo, 0, -medioancho);
	Point3D l1(-mediolargo, -profundo, -medioancho);
	Point3D l3(mediolargo, 0, -medioancho);
	Point3D l2(mediolargo, -profundo, -medioancho);

	quadtex(l0, l1, l2, l3,
		0, _texX, 0, _texX, int(ancho*resXmetro), int(largo*resXmetro));

	Point3D r0(-mediolargo, 0, medioancho);
	Point3D r1(-mediolargo, -profundo, medioancho);
	Point3D r3(mediolargo, 0, medioancho);
	Point3D r2(mediolargo, -profundo, medioancho);

	quadtex(r0, r1, r2, r3,
		0, _texX, 0, _texX, int(ancho*resXmetro), int(largo*resXmetro));

	//finalmente dibujamos la tapa trasera y la tapa delantera

	Point3D b0(-mediolargo, 0, medioancho);
	Point3D b1(-mediolargo, -profundo, medioancho);
	Point3D b3(-mediolargo, 0, -medioancho);
	Point3D b2(-mediolargo, -profundo, -medioancho);

	quadtex(b0, b1, b2, b3,
		0, _texX, 0, _texX, int(ancho*resXmetro), int(largo*resXmetro));


	Point3D f0(mediolargo, 0, medioancho);
	Point3D f1(mediolargo, -profundo, medioancho);
	Point3D f3(mediolargo, 0, -medioancho);
	Point3D f2(mediolargo, -profundo, -medioancho);

	quadtex(f0, f1, f2, f3,
		0, _texX, 0, _texX, int(ancho*resXmetro), int(largo*resXmetro));


	//patas

	glPushMatrix();
	glTranslatef(largo*0.4, 0, ancho*0.25);
	drawLeg(ancho,largo,profundo,_texX,_res);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(largo*0.4, 0, -ancho * 0.5);
	drawLeg(ancho, largo, profundo, _texX, _res);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-largo * 0.4, 0, ancho*0.25);
	drawLeg(ancho, largo, profundo, _texX, _res);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-largo * 0.4, 0, -ancho * 0.5);
	drawLeg(ancho, largo, profundo, _texX, _res);
	glPopMatrix();

	glPopMatrix();

	glPopAttrib();

};

void Entorno::drawRoom() {

	//aceptamos la posibilidad de cambiar el metodo mas adelante para que acepte estos valores por como parametros
	GLfloat largo = 500;
	GLfloat ancho = 500;
	GLfloat alto = 400;

	GLfloat medioLargo = largo / 2;
	GLfloat medioAncho = ancho / 2;
	GLfloat medioAlto = alto / 2;

	GLfloat resXmetro = _res / max(largo, ancho);

	GLuint texturaSuelo;
	GLuint texturaTecho;
	GLuint texturaParedposX;
	GLuint texturaParednegX;
	GLuint texturaParedposZ;
	GLuint texturaParednegZ;

	inicializarTextura(texturaSuelo, "./textures/room/negy.dds");
	inicializarTextura(texturaTecho, "./textures/room/posy.dds");
	inicializarTextura(texturaParedposX, "./textures/room/posx.dds");
	inicializarTextura(texturaParednegX, "./textures/room/negx.dds");
	inicializarTextura(texturaParedposZ, "./textures/room/posz.dds");
	inicializarTextura(texturaParednegZ, "./textures/room/negz.dds");


	glPushAttrib(GL_ALL_ATTRIB_BITS);


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D); //habilitamos textura

	glBindTexture(GL_TEXTURE_2D, texturaSuelo);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	//dibujamos el suelo y el techo usando un mismo conjunto de vectores
	Point3D v0(-medioLargo, -100, -medioAncho);
	Point3D v1(-medioLargo, -100, medioAncho);
	Point3D v3(medioLargo, -100, -medioAncho);
	Point3D v2(medioLargo, -100, medioAncho);

	quadtex(v0, v1, v2, v3,
		0, _texX, 0, _texX, int(ancho*resXmetro), int(largo*resXmetro));

	glBindTexture(GL_TEXTURE_2D, texturaTecho);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	v0.y += alto;
	v1.y += alto;
	v2.y += alto;
	v3.y += alto;

	quadtex(v0, v1, v2, v3,
		0, _texX, 0, _texX, int(ancho*resXmetro), int(largo*resXmetro));


	//dibujamos las dos paredes del eje x usando un mismo conjunto de vectores
	Point3D x0(-medioLargo, -100, medioAncho);
	Point3D x1(-medioLargo, -100, -medioAncho);
	Point3D x3(-medioLargo, -100 + alto, medioAncho);
	Point3D x2(-medioLargo, -100 + alto, -medioAncho);
	
	glBindTexture(GL_TEXTURE_2D, texturaParednegX);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	quadtex(x0, x1, x2, x3,
		0, _texX, 0, _texX, int(ancho*resXmetro), int(largo*resXmetro));

	x0.x = medioLargo;
	x1.x = medioLargo;
	x2.x = medioLargo;
	x3.x = medioLargo;

	glBindTexture(GL_TEXTURE_2D, texturaParedposX);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	quadtex(x0, x1, x2, x3,
		0, _texX, 0, _texX, int(ancho*resXmetro), int(largo*resXmetro));


	//dibujamos las dos paredes del eje z usando un mismo conjunto de vectores
	Point3D z0(medioLargo, -100, -medioAncho);
	Point3D z1(-medioLargo, -100, -medioAncho);
	Point3D z3(medioLargo, -100 + alto, -medioAncho);
	Point3D z2(-medioLargo, -100 + alto, -medioAncho);

	glBindTexture(GL_TEXTURE_2D, texturaParednegZ);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	quadtex(z0, z1, z2, z3,
		0, _texX, 0, _texX, int(ancho*resXmetro), int(largo*resXmetro));

	z0.z = medioAncho;
	z1.z = medioAncho;
	z2.z = medioAncho;
	z3.z = medioAncho;

	glBindTexture(GL_TEXTURE_2D, texturaParedposZ);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	quadtex(z0, z1, z2, z3,
		0, _texX, 0, _texX, int(ancho*resXmetro), int(largo*resXmetro));


	glPopAttrib();


}

void Entorno::draw(GLuint texturaMesa, GLfloat ancho, GLfloat largo, GLfloat profundo) {

	drawRoom();

	drawDesk(texturaMesa,ancho,largo,profundo);

}
