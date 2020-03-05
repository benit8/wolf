/*
** wolf, 2020
** State.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

class StateManager;

////////////////////////////////////////////////////////////////////////////////

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>

////////////////////////////////////////////////////////////////////////////////

class State
{
public:
	virtual ~State() = default;

	virtual void update(double) = 0;
	virtual void render(SDL_Renderer *) const = 0;
	virtual void handleEvent(SDL_Event) = 0;
	virtual void staticUpdate(double) {}

protected:
	explicit State(StateManager &manager)
	: m_manager(manager)
	{}

	StateManager &m_manager;
};