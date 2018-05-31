/***************************************************
ISGI::Crcuito estatico
Samuel Sáez', 2018 (v2.0)

Clases para el control de la camara que usaremos durante la construccion del circuito

Clases:
Camera: Sistema de camara libre tipo FPS(primera persona)

Dependencias:
+glm.hpp
+Utilidades.h
+
***************************************************/
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream> // Biblioteca de entrada salida
#include <sstream> // Biblioteca de manejo de strings
#include <cmath> // Biblioteca matematica de C
#include <Utilidades.h> // Biblioteca de Utilidades
#include <vector>

// Definicion de los tipos de movimientos disponibles a realizar con el teclado, de esta forma es mas sencillo llamarlos desde el main de la aplicacion
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// valores por defecto
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 1.0f;
const float SENSITIVITY = 0.3f;
const float ZOOM = 45.0f;


//Camara basada en los angulos de euler.
class Camera
{
public:
	// Atributos necesarios para determinar la posicion, el vector de direccion de la camara, etc.
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// Angulos de euler
	float Yaw;
	float Pitch;
	// Ajustes de la camara
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;
	//booleano para guardar la configuracion de la camara, por defecto no invertida.
	BOOLEAN inverted = false;

	//Constructores, uno con vectores y el otro con valores escalares.
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	// Devuelve la matriz view la cual usaremos en el modelo MVP
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}
	
	// Para no tener que utilizar shaders utilizamos los vectores de la camara directamente sobre el gluLookat
	void SetGluLookUp()
	{
		glm::vec3 Center = Position + Front;
		gluLookAt(Position[0], Position[1], Position[2], Center[0], Center[1], Center[2], Up[0], Up[1], Up[2]);
	}


	// Metodo auxiliar para la conversion de los enums declarados previamente al movimiento definitivo de la camara
	void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
		float velocity = MovementSpeed * deltaTime;
		if (direction == FORWARD)
			Position += Front * velocity;
		if (direction == BACKWARD)
			Position -= Front * velocity;
		if (direction == LEFT)
			Position -= Right * velocity;
		if (direction == RIGHT)
			Position += Right * velocity;
	}

	// Metodo que será llamado desde el metodo de escucha del raton para procesar los datos y actualizar los vectores de la camara.
	void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;
		//Para que el valor de Yaw no tienda a infinito lo reducimos en modulo 360 para que unicamente tome valores entre 0 y 360
		Yaw = std::fmod((Yaw + xoffset), (GLfloat)360.0f);
		//aqui se puede invertir el eje vertical.
		if (inverted)
		{
			Pitch += yoffset;
		}
		else
		{
			Pitch -= yoffset;
		}


		// Hay que tener en cuenta que si el valor de Pitch pasa de 89 la camara se dará la vuelta y dejara de funcionar como toca
		if (constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}

		// Actualizamos los vectores de la camara con los datos recibidos desde el raton
		updateCameraVectors();
	}

	// Metodo auxiliar para procesar los movimientos de la rueda del raton
	void ProcessMouseScroll(float yoffset)
	{
		//los valores solo se moveran entre 1 y 45
		if (Zoom >= 1.0f && Zoom <= 45.0f)
			Zoom -= yoffset;
		if (Zoom <= 1.0f)
			Zoom = 1.0f;
		if (Zoom >= 45.0f)
			Zoom = 45.0f;
	}

	void ReverseYAxis() {
		inverted = !inverted;
	}

private:
	// Metodo para actualizar los vectores que mueven la camara
	void updateCameraVectors()
	{
		// Calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		// Calculamos los vectores derecho y el vector Up
		Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalizamos los vectores para evitar comportamientos no deseados cuando estos se acercan a 0.
		Up = glm::normalize(glm::cross(Right, Front));
	}
};
#endif