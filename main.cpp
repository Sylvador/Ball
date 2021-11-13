#include "Game.h"

int main()
{
	//Init random seed
	srand(static_cast<unsigned>(time(0)));

	//initialize game object
	Game game;

	//game loop
	while (game.running())
	{
		game.update();
		game.render();
	}

	return 0;
}