/*
** wolf, 2020
** State.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

class StateManager;

////////////////////////////////////////////////////////////////////////////////

#include "DrawHelper.hpp"

#include "SDL++/Events.hpp"
#include "SDL++/Render.hpp"

////////////////////////////////////////////////////////////////////////////////

class State
{
public:
	virtual ~State() = default;

	virtual void update(double) = 0;
	virtual void render(SDL::Texture &) const = 0;
	virtual void handleEvent(const SDL::Event &) = 0;
	virtual void staticUpdate(double) {}

	bool fullscreen() const { return m_fullscreen; }
	bool paused() const { return m_paused; }
	void pause(bool p) { m_paused = p; }
	void pause() { m_paused = true; }
	void unpause() { m_paused = false; }

protected:
	explicit State(StateManager &manager)
	: m_manager(manager)
	{}

	StateManager &m_manager;
	bool m_fullscreen = true;
	bool m_paused = false;
};