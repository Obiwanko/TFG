
#ifndef POINT_H
#define POINT_H
#include <iostream>
#include <cmath> // Biblioteca matematica de C
class Point3D
{
public:
	GLfloat x, y, z;
	Point3D() : x(0), y(0), z(0) {};
	Point3D(float cx, float cy, float cz) : x(cx), y(cy), z(cz) {};
	GLfloat& operator[](int i)
	{
		// Devuelve la coordenada con acceso indexado por referencia
		try {
			if (i == 0)      return x;
			else if (i == 1) return y;
			else if (i == 2) return z;
			else throw 999;
		}
		catch (int error) {
			//cout << "Excepcion en Point3D: " << error;
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
#endif