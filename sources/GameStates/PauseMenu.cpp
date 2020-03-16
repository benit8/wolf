/*
** wolf, 2020
** PauseMenu.cpp
*/

#include "GameStates/PauseMenu.hpp"
#include "StateManager.hpp"
#include "Utils/Vector2.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace GameStates
{

////////////////////////////////////////////////////////////////////////////////

PauseMenu::PauseMenu(StateManager &m)
: State(m)
{
	m_fullscreen = false;
}

////////////////////////////////////////////////////////////

void PauseMenu::handleEvent(const SDL::Event &e)
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

void PauseMenu::render(SDL::Texture &framebuffer) const
{
	auto l = framebuffer.lock();
	DrawHelper dh(l);
	dh.fill({l.width() / 4, l.height() / 4, l.width() / 2, l.height() / 2}, {100, 100, 100});
}

////////////////////////////////////////////////////////////////////////////////

}