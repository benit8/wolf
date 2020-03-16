/*
** wolf, 2020
** DrawHelper.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include "SDL++/Texture.hpp"

////////////////////////////////////////////////////////////////////////////////

class DrawHelper
{
public:
	explicit DrawHelper(SDL::Texture::Lock &lock)
	: m_lock(lock)
	{}

	~DrawHelper() = default;

	////////////////////////////////////////////////////////////////////////////

	void plot(const SDL::Vec2i &pos, const SDL::Color &color)
	{
		m_lock.at(pos) = color;
	}

	void fill(const SDL::Color &color = SDL::Color::Black)
	{
		SDL::Vec2i p;
		for (p.y = 0; p.y < m_lock.height(); ++p.y)
			for (p.x = 0; p.x < m_lock.width(); ++p.x)
				plot(p, color);
	}

	void fill(const SDL::Rect &area, const SDL::Color &color)
	{
		SDL::Vec2i p;
		for (p.y = area.y1(); p.y < area.y2(); ++p.y)
			for (p.x = area.x1(); p.x < area.x2(); ++p.x)
				plot(p, color);
	}

	void drawLine(const SDL::Vec2i &from, const SDL::Vec2i &to, const SDL::Color &color)
	{
		SDL::Vec2i diff = to - from;
		size_t steps = std::max(abs(diff.x), abs(diff.y));
		SDL::Vec2f inc(diff.x / (float)steps, diff.y / (float)steps);
		SDL::Vec2f start(from.x, from.y);
		for (size_t i = 0; i < steps; ++i) {
			plot({start.x, start.y}, color);
			start += inc;
		}
	}

private:
	SDL::Texture::Lock &m_lock;
};