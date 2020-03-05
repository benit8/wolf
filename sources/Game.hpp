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
#include <unistd.h>
#include <vector>

////////////////////////////////////////////////////////////////////////////////

class Game : public StateManager
{
public:
	Game(int argc, char **argv)
	: m_invokedAs(argv[0])
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
	}

	~Game()
	{
		SDL_DestroyRenderer(m_renderer);
		SDL_DestroyWindow(m_window);
	}

	int run()
	{
		m_running = true;
		loop();
		return m_running == false;
	}

private:
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

			handleEvent(e);
		}
	}

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

			if (shouldRender) {
				SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
				SDL_RenderClear(m_renderer);
				render(m_renderer);
				SDL_RenderPresent(m_renderer);
			}
		}
#else
		while (m_running) {
			double now = Time::now();

			update(frameTime);
			handleEvents();

			SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
			SDL_RenderClear(m_renderer);
			render(m_renderer);
			SDL_RenderPresent(m_renderer);

			// Reduces CPU usage
			double pauseTime = frameTime - (Time::now() - now);
			if (pauseTime > DBL_EPSILON)
				usleep(pauseTime * 1e6);
		}
#endif
	}

private:
	std::string m_invokedAs;
	std::vector<std::string> m_args;

	SDL_Window *m_window = NULL;
	SDL_Renderer *m_renderer = NULL;
	bool m_running = false;

	const double frameTime = 1.0 / 60.0;
};