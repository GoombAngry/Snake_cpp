#include <iostream>
#include <Windows.h>
#include "Game.h"
#include <conio.h>
using namespace std;
int main()
{
	int x[]{ 4, 40 }, y[]{4,28};
	Game game(x,y);
	game.runGame();

}

