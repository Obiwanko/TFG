/***************************************************
Variables globales que puedan ser utilizadas en diferentes clases.
***************************************************/
#ifndef GLOBALS_H
#define GLOBALS_H

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
extern GLint graficos;
extern GLboolean fullscreen;




#endif