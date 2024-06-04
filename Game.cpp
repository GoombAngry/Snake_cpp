#include <iostream>
#include <Windows.h>
#include <thread>
#include <chrono>
#include <random>
#include <conio.h>
#include "Game.h"
#include <string>
using namespace std;
// Definicion de constructores
Apple::Apple() {/* Constructor por defecto */ };
Game::Game() { /* Constructor por defecto */ };
Body::Body() { /* Constructor por defecto */ };
Apple::Apple(int x, int y, string character) {
	this->x = x;
	this->y = y;
	this->character = character;
};
Body::Body(int x,int y,string character,Orientation orientation) {
	this->x = x;
	this->y = y;
	this->character = character;
	this->orientation = orientation;
};
Game::Game(int x[2], int y[2]) {
	// Guardando las coordenadas del Mapa de juego
	this->x[0] = x[0]; // Ancho inicial
	this->x[1] = x[1]; // Ancho final
	this->y[0] = y[0]; // Altura inicial
	this->y[1] = y[1]; // Altura final
	// Generamos la cabeza de la serpiente
	int xAverage = (this->x[1] - ((this->x[1] - this->x[0]) / 2));
	int yAverage = (this->y[1] - ((this->y[1] - this->y[0]) / 2));

	this->snake.push_back(Body(xAverage, yAverage,"o",Orientation::UP));

	// Generamos la manzana
	this->apple = Apple((this->x[1] - 15), (this->y[1] - 13),"*");
	// Guardando informacion de la consola que ejecuta este programa
	this->console = GetStdHandle(STD_OUTPUT_HANDLE);
	// Modificamos el cursor del raton dentro de la consola para que sea invisible o nula-mente invisible
	this->cursor.bVisible = FALSE; // Para que sea invisible el cursor
	this->cursor.dwSize = 1; // El size del cursor (va desde 1 hasta 100)
	SetConsoleCursorInfo(this->console, &cursor); // Seteamos la configuracion del curso

	// Seteamos la variable de la vida
	this->isDead = false;
};
void Game::drawCharacter(int x, int y, string character) {
	COORD coord; // Objeto que indicara las coordenadas de la consola donde se imprimira o dibujara el caracter que deseamos
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(this->console, coord); // Movemos el cursor a la posicion indicada
	cout << character; // Pintamos el caracter que deseamos en dicha posicion
};
bool Game::isOutMap(int x, int y) {
	if ((x >= this->x[0] + 1 && x <= this->x[1] - 1) && (y >= this->y[0] + 1 && y <= this->y[1] - 1)) {
		return false;
	}
	else {
		return true;
	}
};
void Game::drawMapGame() {
	for (int y = this->y[0]; y <= this->y[1]; y++)
	{
		if (y == this->y[0] || y == this->y[1]) { // Niveles superior e inferior
			for (int x = this->x[0]; x <= this->x[1]; x++)
			{
				this->drawCharacter(x, y, "#");
			}
		}
		else {
			this->drawCharacter(this->x[0], y, "#");
			this->drawCharacter(this->x[1], y, "#");

		}
	}
}
int Game::generatedRandomNumber(int min, int max) {
	random_device seed; // Obtenemos una semilla aleatoria
	mt19937 machineNumber(seed()); // Creamos un generador con la semilla generadas anteriormente
	int minimun(min), maximun(max); // Parametros del rango que queremos obtener del numero aleatoriamente
	uniform_int_distribution<int> distribution(minimun, maximun);
	return distribution(machineNumber);
}
void Game::listenerKeyboard() {
    if (_kbhit()) { // Esta funcion devuelve true cuando presionamos una tecla
        int  caracterCode = _getch();
		struct Body* headSnake = &this->snake.at(0);
        if (caracterCode == 119 || caracterCode == 87) { // Arriba
			if (headSnake->orientation != Orientation::DOWN) {
				headSnake->orientation = Orientation::UP;
			}
        }
        else if (caracterCode == 100 || caracterCode == 68) { // Derecha
			if (headSnake->orientation != Orientation::LEFT) {
				headSnake->orientation = Orientation::RIGHT;
			}
        }
        else if (caracterCode == 115 || caracterCode == 83) { // Abajo
			if (headSnake->orientation != Orientation::UP) {
				headSnake->orientation = Orientation::DOWN;
			}
			
		}
        else if (caracterCode == 97 || caracterCode == 65) { // Izquierda
			if (headSnake->orientation != Orientation::RIGHT) {
				headSnake->orientation = Orientation::LEFT;
			}
        }
    }
};
void Game::printSnake() {
	for (int i = 0; i < this->snake.size(); i++)
	{
		struct Body* d = &this->snake.at(i);
		this->drawCharacter(d->x, d->y, d->character);
	}
};
void Game::clearSnake() {
	for (int i = 0; i < this->snake.size(); i++)
	{
		struct Body* d = &this->snake.at(i);
		this->drawCharacter(d->x, d->y," ");
	}
}
void Game::collisionSnake() {
	// Eliminamos las ultimas posiciones hasta de asignar las nuevas a la serpiente
	this->clearSnake();

	// Averiguamos en que direccion esta mirando la serpiente
	struct Body* headSnake = &this->snake.at(0); // Cabeza de la serpiente
	struct Body* tailSnake = &this->snake.at(this->snake.size()-1); // Cola de la serpiente

	int xBefore = headSnake->x, yBefore = headSnake->y;
	Orientation orientation = headSnake->orientation;
	if (orientation == Orientation::UP) {
		if (yBefore > this->y[0] + 1) { // No se sale del mapa
			// Comprobamos si tiene la fruta arriba
			if (yBefore - 1 == this->apple.y && xBefore == this->apple.x) { // Fruta delante 
				this->drawCharacter(this->apple.x, this->apple.y, " "); // Limpiamos la el dibujo de la manzada
				this->score += 5;
				this->updateScore(); // Actualizamos el marcador de puntuacion
				this->snakeEat(tailSnake);
				this->generatedNewPositionApple(); // Generamos nueva posicion de la manzana

			}
			else if (somethingThere(xBefore,yBefore - 1)) { // Significa que en el siguiente movimiento chocara con una parte del cuerpo de la serpiente
				isDead = true;
			}
			this->moveSnake();
		}
		else { 
			isDead = true;
		}
		
	}
	else if (orientation == Orientation::RIGHT) {
		if (xBefore < this->x[1] - 1) { // No se sale del mapa
			// Comprobamos si tiene la fruta hacia la derecha
			if (xBefore + 1 == this->apple.x && yBefore == this->apple.y) { // Fruta en la derecha
				this->drawCharacter(this->apple.x, this->apple.y, " "); // Limpiamos la el dibujo de la manzada
				this->score += 5;
				this->updateScore(); // Actualizamos el marcador de puntuacion
				this->snakeEat(tailSnake);
				this->generatedNewPositionApple(); // Generamos nueva posicion de la manzana

			}
			else if (this->somethingThere(xBefore + 1, yBefore)) {
				isDead = true;
			}
			this->moveSnake();
		}
		else {
			isDead = true;
		}
	}
	else if (orientation == Orientation::LEFT) {
		if (xBefore > this->x[0] + 1) { // No se sale del mapa
			// Comprobamos si tiene la fruta hacia la izquierda
			if (xBefore - 1 == this->apple.x && yBefore == this->apple.y) { // Fruta en la derecha
				this->drawCharacter(this->apple.x, this->apple.y, " "); // Limpiamos la el dibujo de la manzada
				this->score += 5;
				this->updateScore(); // Actualizamos el marcador de puntuacion
				this->snakeEat(tailSnake);
				this->generatedNewPositionApple(); // Generamos nueva posicion de la manzana
			}
			else if (this->somethingThere(xBefore - 1,yBefore)) {
				isDead = true;
			}
			this->moveSnake();
		}
		else {
			isDead = true;
		}
	}
	else {
		if (yBefore < this->y[1] - 1) { // No se sale del mapa
			// Comprobamos si tiene la fruta hacia abajo
			if (yBefore + 1 == this->apple.y && xBefore == this->apple.x) { // Fruta delante 
				this->drawCharacter(this->apple.x, this->apple.y, " "); // Limpiamos la el dibujo de la manzada
				this->score += 5;
				this->updateScore(); // Actualizamos el marcador de puntuacion
				this->snakeEat(tailSnake);
				this->generatedNewPositionApple(); // Generamos nueva posicion de la manzana

			}
			else if (this->somethingThere(xBefore, yBefore + 1)) {
				isDead = true;
			}
			this->moveSnake();
		}
		else {
			isDead = true;
		}
	}
}
void Game::runGame() {
	// Pintamos el mapa del juego
	this->updateScore(); // Pintamos el marcador de puntuacion
	this->drawMapGame();
	this->drawCharacter(this->apple.x, this->apple.y, this->apple.character);
	while (!this->isDead) {
		this->printSnake();
		this->listenerKeyboard();
		this_thread::sleep_for(chrono::milliseconds(150));
		this->collisionSnake();
	}
}
bool Game::somethingThere(int x, int y) {
	for (int i = 0; i < this->snake.size(); i++)
	{
		struct Body body = this->snake.at(i);
		if (body.x == x && body.y == y) {
			return true;
		}
	}
	return false;
}
void Game::generatedNewPositionApple() {
	bool correct = false;
	while (!correct) {
		// Generamos nuevas posiciones
		int Xazar = this->generatedRandomNumber((this->x[0] + 1), (this->x[1] - 1)), Yazar = this->generatedRandomNumber((this->y[0] + 1), (this->y[1] - 1));
		while (Xazar == this->apple.x && Yazar == this->apple.y) {
			Xazar = this->generatedRandomNumber((this->x[0] + 1), (this->x[1] - 1));
			Yazar = this->generatedRandomNumber((this->y[0] + 1), (this->y[1] - 1));
		}
		if (!this->somethingThere(Xazar, Yazar)) { // Comprobamos que no haya nada en esa posicion (Por si spawnea en alguna parte de la serpiente)
			this->apple.x = Xazar;
			this->apple.y = Yazar;
			correct = true;
		}
	}
	this->drawCharacter(this->apple.x, this->apple.y, this->apple.character);
}
void Game::snakeEat(Body* n) {
	struct Body newBody = Body();
	newBody.character = "o";
	newBody.orientation = n->orientation;

	if (n->orientation == Orientation::UP) {
		newBody.y = n->y + 1;
		newBody.x = n->x;
	}
	else if (n->orientation == Orientation::RIGHT) {
		newBody.y = n->y ;
		newBody.x = n->x - 1;
	}
	else if (n->orientation == Orientation::DOWN) {
		newBody.y = n->y - 1;
		newBody.x = n->x;
	}
	else if (n->orientation == Orientation::LEFT) {
		newBody.y = n->y;
		newBody.x = n->x + 1;
	}
	this->snake.push_back(newBody);



}
void Game::moveSnake() {
	Orientation orientationLast = Orientation::NONE;
	for (int i = 0; i < this->snake.size(); i++)
	{
		struct Body* bodyIter = &this->snake.at(i);
		if (bodyIter->orientation == Orientation::UP) {
			bodyIter->y -= 1;
			
		}
		else if (bodyIter->orientation == Orientation::RIGHT) {
			bodyIter->x += 1;

		}
		else if (bodyIter->orientation == Orientation::LEFT) {
			bodyIter->x -= 1;

		}
		else if (bodyIter->orientation == Orientation::DOWN) {
			bodyIter->y += 1;

		}
		if (orientationLast == Orientation::NONE) { // Estamos con la primera parte de la serpiente
			orientationLast = bodyIter->orientation; // Guardamos la orientacion para la siguiente pieza
		}
		else {
			Orientation aux = bodyIter->orientation;
			bodyIter->orientation = orientationLast;
			orientationLast = aux;
		}
		
	}

}
void Game::updateScore() {
	this->drawCharacter(this->x[0], this->y[0] - 1, "                                         ");
	this->drawCharacter(this->x[0], this->y[0] - 1, "Score: " + to_string(this->score));
}