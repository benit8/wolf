/*
** wolf, 2020
** PauseMenu.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include "State.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace GameStates
{

class PauseMenu : public State
{
public:
	explicit PauseMenu(StateManager &m)
	: State(m)
	{
		m_fullscreen = false;
	}

	~PauseMenu() = default;

	void update(double) {}

	void handleEvent(SDL_Event e)
	{
		switch (e.type) {
		case SDL_KEYDOWN:
			if (e.key.keysym.sym == SDLK_ESCAPE)
				m_manager.popState();
			break;
		default:
			break;
		}
	}

	void render(SDL_Renderer *renderer) const
	{
		Vector2i size;
		SDL_GetRendererOutputSize(renderer, &size.x, &size.y);
		SDL_Rect rect = {size.x / 4, size.y / 4, size.x / 2, size.y / 2};

		SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
		SDL_RenderFillRect(renderer, &rect);
	}
};

}