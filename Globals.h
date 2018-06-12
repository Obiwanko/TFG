/***************************************************
ISGI::Crcuito estatico
Samuel Sáez', 2018 (v2.0)

Variables globales que puedan ser utilizadas en diferentes clases.

Clases:
Camera: Sistema de camara libre tipo FPS(primera persona)

Dependencias:
+Camera.h
***************************************************/
#ifndef GLOBALS_H
#define GLOBALS_H
#include "Tramos.h"
#include "Camera.h" // Encabezado donde esta definida la camara.

/*
Variables que indican los diferentes directorios
*/
extern string saveFolder;
extern string textureFolder;

/*
Variable para definir el fichero a cargar/guardar
*/
extern string fileMap;

/*
Variables para la resolucion y la repeticion de texturas, vendrán cargadas desde el fichero de opciones.
//TODO quitar valor por defecto una vez implementada funcion de cargado.
*/
extern GLint resolucion;
extern GLint repeticionTex;
extern GLboolean fullscreen;

/*
**********************************************************************
Variables de usadas para el funcionamiento de la camara
**********************************************************************
*/



/*

Variables de atencion al HUD en el modo de creacion de circuitos

*/
//si el tramo esta seleccionado o no(ya se ha pulsado enter)
extern GLboolean seleccionado;

// indica que tramo tenenemos enfocado, en caso de pulsar enter dibujaremos dicho tramo
extern GLint tramoactual;

//control de la rotacion de las piezas en el HUD
extern GLfloat rotacion_pieza;



/* 

Variables de atencion a la lista de tramos que tenenemos actualmente en memoria

*/

//total de piezas diferentes disponibles
extern GLint const totalTramos;

//Vector de Tramos cargados actualmente en memoria
extern std::vector<Tramo*> vectorTramosEnMemoria;

//Variables de la posicion actual donde se colocará el ultimo tramo.
//extern Point3D posicionUltimoTramo;

#endif