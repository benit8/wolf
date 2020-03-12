/*
** wolf, 2020
** main.cpp
*/

#include "Game.hpp"

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
	Game game(argc, argv);
	game.pushState<GameStates::InGame>(argc > 1 ? argv[1] : "maps/test0.w3d");
	return game.run();
}