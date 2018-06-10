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

#include "Camera.h" // Encabezado donde esta definida la camara.
/*
Variables que indican los diferentes directorios
*/
extern string const saveFolder = "./savefiles/";
extern string const textureFolder = "./textures/";

/*
Variables para la resolucion y la repeticion de texturas, vendrán cargadas desde el fichero de opciones.
//TODO quitar valor por defecto una vez implementada funcion de cargado.
*/
extern GLint resolucion = 20;
extern GLint repeticionTex = 1;


/*
**********************************************************************
Variables de usadas para el funcionamiento de la camara
**********************************************************************
*/
extern Camera camaraflotante;
extern GLfloat velocidadcamara = 1.0;
extern GLfloat lastx, lasty;


/*

Variables de atencion al HUD en el modo de creacion de circuitos

*/
//si el tramo esta seleccionado o no(ya se ha pulsado enter)
extern GLboolean seleccionado = false;

// indica que tramo tenenemos enfocado, en caso de pulsar enter dibujaremos dicho tramo
extern GLint tramoactual = 0;

//control de la rotacion de las piezas en el HUD
extern GLfloat rotacion_pieza = 0;



/* 

Variables de atencion a la lista de tramos que tenenemos actualmente en memoria

*/

//total de piezas diferentes disponibles
extern GLint const totalTramos = 7;

//Vector de Tramos cargados actualmente en memoria
extern std::vector<Tramo*> vectorTramosEnMemoria;
#endif