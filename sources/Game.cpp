/*
** wolf, 2020
** Game.cpp
*/

#include "Game.hpp"
#include "Utils/Time.hpp"

#include <stdexcept>

////////////////////////////////////////////////////////////////////////////////

Game::Game(int argc, char **argv)
: m_execName(argv[0])
, m_args(argv + 1, argv + argc)
, m_sdlRoot()
, m_window("Wolf3D", {1280, 720}, SDL_WINDOW_RESIZABLE)
, m_renderer(m_window.makeRenderer())
, m_framebuffer(m_renderer.ptr(), m_window.size(), SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING)
{
	m_renderer.setBlendMode(SDL_BLENDMODE_BLEND);
}

Game::~Game()
{
}

int Game::run()
{
	if (hasStates())
		throw std::logic_error("No game states available at startup");

	m_running = true;
	loop();
	return m_running == false;
}

////////////////////////////////////////////////////////////////////////////////

void Game::loop()
{
#ifdef FULL_CAPACITY
	double last = Time::now(), updateTimer = 0;
	while (m_running) {
		double now = Time::now();
		double elapsed = now - last;
		last = now;

		updateTimer += elapsed;
		updateStates(elapsed);

		bool shouldRender = false;
		while (updateTimer >= frameTime) {
			handleEvents();
			staticUpdateStates(elapsed);
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
			SDL::delay(pauseTime * 1e3);
	}
#endif
}

void Game::handleEvents()
{
	SDL::Event e;
	while (e.poll()) {
		switch (e.type) {
		case SDL_QUIT:
			m_running = false;
			break;
		case SDL_KEYDOWN:
			if (e.key.keysym.sym == SDLK_F11)
				m_window.toggleFullscreen();
			break;
		default:
			break;
		}

		handleStatesEvent(e);
	}
}

void Game::renderFrame()
{
	{
		auto l = m_framebuffer.lock();
		::memset(l.pixels(), 0, l.height() * l.pitch());
	}

	renderStates(m_framebuffer);

	m_renderer.clear();
	m_renderer.copy(m_framebuffer);
	m_renderer.present();
}