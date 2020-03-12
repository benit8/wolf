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
	explicit PauseMenu(StateManager &m);
	~PauseMenu() = default;

	void update(double) {}
	void handleEvent(SDL_Event e);
	void render(SDL_Renderer *renderer) const;
};

}