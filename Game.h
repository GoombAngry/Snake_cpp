#pragma once
#include<iostream>
#include<Windows.h>
#include<vector>
using namespace std;

// Orientacion de cada parte de la serpiente
enum Orientation {
	UP, // Llendo hacia arriba
	DOWN, // Llendo hacia abajo
	LEFT, // Llendo hacia la izquierda
	RIGHT, // Llendo hacia la derecha
	NONE // Valor Nulo
};

// Representa la recompensa de la serpiente
struct Apple {
	int x, y;
	string character;
	Apple();
	Apple(int,int,string);
};

// Cuerpo de la Serpiente
struct Body { 
	int x, y; // Coordenadas de donde se ubica la parte del cuerpo de la serpiente
	int id_body; // Identificador de la parte del cuerpo de la serpiente
	string character; // El caracter que representa la parte de la serpiente
	Orientation orientation; // La orientacion de hacia donde esta llendo la parte de la serpiente
	bool isVisible; // Parametro que indica si es visible la parte del cuerpo
	Body();
	Body(int,int,string,Orientation);
};

// La clase principal del juego
class Game {
private:
	HANDLE console; // Guardamos la consola que ejecute este programa
	CONSOLE_CURSOR_INFO cursor; // Guardamos informacion del cursor del raton
	int x[2], y[2]; // Coordenadas para generar el mapa del juego
	int score; // Variable que guardara la puntuacion del juego
	bool isDead; // Indicara cuando se acaba el juego
	struct Apple apple; // Se referira a la manzana que ira apareciendo de manera aleatoria en el mapa
	vector<Body> snake; // Contendra el cuerpo de la serpiente
public:
	Game();
	Game(int[], int[]);
	void drawCharacter(int,int,string); // Dibujara un caracter en una coordenada pasada como parametro
	bool isOutMap(int, int); // Nos devolvera si una coordenada pasada se encuentra fuera del mapa
	void drawMapGame(); // Dibujara el mapa del juego
	int generatedRandomNumber(int,int); // Generara un numero comprendido en un rango pasado como parametro
	void listenerKeyboard(); // Metedo que detectara la tecla del keyboard para mover la serpiente
	void printSnake(); // Dibujara la serpiente en el mapa
	void runGame(); // Metodo para iniciar el juego
	void clearSnake(); // Metodo para limpiar de la pantalla la snake entera
	void collisionSnake(); // Metodo que detectara colisiones
	bool somethingThere(int,int); // Devolvera si hay algo en esas coordenadas (Se utilizara para generar las nuevas coordenadas de la manzana)
	void generatedNewPositionApple(); // Generara una posicion al azar para la manzana
	void snakeEat(Body*); // Este asignara la nueva parte final de la serpiente cada vez que coma
	void moveSnake(); // Este movera las partes de la serpiente
	void updateScore(); // Este metodo va a actualizar el marcador de puntuacion

};