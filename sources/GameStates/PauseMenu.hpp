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
	void handleEvent(const SDL::Event &e);
	void render(SDL::Texture &framebuffer) const;
};

}