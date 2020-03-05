/*
** wolf, 2020
** Game.hpp
*/

#pragma once

// #define FULL_CAPACITY

////////////////////////////////////////////////////////////////////////////////

#include "StateManager.hpp"
#include "WorldMap.hpp"
#include "Utils/Time.hpp"

#include "GameStates/InGame.hpp"

#include <SDL2/SDL.h>

#include <stdexcept>
#include <string>
#include <vector>

////////////////////////////////////////////////////////////////////////////////

class Game : public StateManager
{
public:
	Game(int argc, char **argv)
	: m_execName(argv[0])
	, m_args(argv + 1, argv + argc)
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
			throw std::runtime_error(SDL_GetError());
		atexit(SDL_Quit);

		m_window = SDL_CreateWindow("Wolf3D", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_RESIZABLE);
		if (!m_window)
			throw std::runtime_error(SDL_GetError());

		m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (!m_renderer) {
			SDL_DestroyWindow(m_window);
			throw std::runtime_error(SDL_GetError());
		}

		if (SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND) != 0)
			std::cerr << "Failed to activate alpha blending: " << SDL_GetError() << std::endl;
	}

	~Game()
	{
		SDL_DestroyRenderer(m_renderer);
		SDL_DestroyWindow(m_window);
	}

	int run()
	{
		if (hasStates())
			throw std::logic_error("No game states available at startup");

		m_running = true;
		loop();
		return m_running == false;
	}

private:
	void loop()
	{
#ifdef FULL_CAPACITY
		double last = Time::now(), updateTimer = 0;
		while (m_running) {
			double now = Time::now();
			double elapsed = now - last;
			last = now;

			updateTimer += elapsed;
			update(elapsed);

			bool shouldRender = false;
			while (updateTimer >= frameTime) {
				handleEvents();
				staticUpdate(elapsed);
				updateTimer -= frameTime;
				shouldRender = true;
			}

			if (shouldRender)
				renderFrame();
		}
#else
		while (m_running) {
			double start = Time::now();

			handleEvents();
			updateStates(frameTime);
			staticUpdateStates(frameTime);
			renderFrame();

			double pauseTime = frameTime - (Time::now() - start);
			if (pauseTime > DBL_EPSILON)
				SDL_Delay(pauseTime * 1e3);
		}
#endif
	}

	void handleEvents()
	{
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				m_running = false;
				break;
			default:
				break;
			}

			handleStatesEvent(e);
		}
	}

	void renderFrame()
	{
		SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
		SDL_RenderClear(m_renderer);
		renderStates(m_renderer);
		SDL_RenderPresent(m_renderer);
	}

private:
	std::string m_execName;
	std::vector<std::string> m_args;

	SDL_Window *m_window = NULL;
	SDL_Renderer *m_renderer = NULL;
	bool m_running = false;

	const double frameTime = 1.0 / 60.0;
};