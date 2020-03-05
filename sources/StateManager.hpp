/*
** wolf, 2020
** StateManager.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include "State.hpp"

#include <deque>

////////////////////////////////////////////////////////////////////////////////

class StateManager
{
protected:
	StateManager() = default;

public:
	~StateManager()
	{
		for (auto it = m_states.rbegin(); it != m_states.rend(); ++it)
			delete *it;
	}

	template <typename T, typename... Args>
	void pushState(Args&&... args)
	{
		auto s = new T(*this, std::forward<Args>(args)...);
		m_states.push_back(s);
	}

	void popState()
	{
		m_states.pop_back();
	}


	void update(double delta)
	{
		topState()->update(delta);
	}

	void handleEvent(SDL_Event e)
	{
		topState()->handleEvent(e);
	}

	void render(SDL_Renderer *renderer)
	{
		for (auto it = m_states.rbegin(); it != m_states.rend(); ++it)
			(*it)->render(renderer);
	}

	void staticUpdate(double delta)
	{
		topState()->staticUpdate(delta);
	}



	const State *topState() const { return m_states.back(); }
	State *topState() { return m_states.back(); }

private:
	std::deque<State *> m_states;
};