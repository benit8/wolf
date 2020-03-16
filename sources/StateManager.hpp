/*
** wolf, 2020
** StateManager.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include "State.hpp"

#include <algorithm>
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
		if (!m_states.empty())
			m_states.pop_back();

		if (m_states.empty())
			throw std::runtime_error("No game states left");

		m_states.back()->unpause();
	}


	void updateStates(double delta)
	{
		auto it = m_states.end();
		for (--it; it != m_states.begin() && !(*it)->paused(); --it);
		if (m_states.size() > 1)
			++it;

		for (; it != m_states.end(); ++it)
			(*it)->update(delta);
	}

	void handleStatesEvent(const SDL::Event &e)
	{
		m_states.back()->handleEvent(e);
	}

	void renderStates(SDL::Texture &framebuffer)
	{
		// Search for the first fullscreen state from the top of the stack
		auto it = m_states.end();
		for (--it; it != m_states.begin() && !(*it)->fullscreen(); --it);

		// Render, going back to the top
		for (; it != m_states.end(); ++it) {
			(*it)->render(framebuffer);

			if (it != m_states.end() - 1) {
				auto l = framebuffer.lock();
				DrawHelper dh(l);
				dh.fill({0, 0, 0, 191});
			}
		}
	}

	void staticUpdateStates(double delta)
	{
		auto it = m_states.end();
		for (--it; it != m_states.begin() && !(*it)->paused(); --it);
		if (m_states.size() > 1)
			++it;

		for (; it != m_states.end(); ++it)
			(*it)->staticUpdate(delta);
	}


	bool hasStates() const { return m_states.empty(); }

private:
	std::deque<State *> m_states;
};