/*
** wolf, 2020
** Game.hpp
*/

#pragma once

#define FULL_CAPACITY

////////////////////////////////////////////////////////////////////////////////

#include "StateManager.hpp"
#include "WorldMap.hpp"

#include "GameStates/InGame.hpp"

#include "SDL++/SDL.hpp"

#include <string>
#include <vector>

////////////////////////////////////////////////////////////////////////////////

class Game : public StateManager
{
public:
	Game(int argc, char **argv);
	Game(const Game &) = delete;
	Game(Game &&) = delete;
	~Game();

	int run();

	Game &operator =(const Game &) = delete;
	Game &operator =(Game &&) = delete;

private:
	void loop();
	void handleEvents();
	void renderFrame();

private:
	std::string m_execName;
	std::vector<std::string> m_args;

	SDL::Root m_sdlRoot;
	SDL::Window m_window;
	SDL::Renderer m_renderer;
	SDL::Texture m_framebuffer;
	bool m_running = false;

	const double frameTime = 1.0 / 60.0;
};