/*
** wolf, 2020
** WorldMap.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include "Utils/Vector2.hpp"

#include <iostream>
#include <vector>

////////////////////////////////////////////////////////////////////////////////

class WorldMap
{
	typedef uint8_t Tile;

public:
	WorldMap(unsigned width, unsigned height)
	: m_size(width, height)
	, m_map(width * height, Tile())
	{}

	explicit WorldMap(unsigned width, unsigned height, std::vector<Tile> &&map)
	: m_size(width, height)
	, m_map(map)
	{}

	~WorldMap() = default;


	bool isInside(const Vector2u &pos) const {
		return pos.x < m_size.x && pos.y < m_size.y;
	}

	bool isInside(const Vector2f &pos) const {
		return pos.x >= 0 && pos.x < m_size.x && pos.y >= 0 && pos.y < m_size.y;
	}

	bool isInside(float x, float y) const {
		return isInside(Vector2f(x, y));
	}

	const Tile &at(const Vector2u &pos) const
	{
		if (!isInside(pos))
			std::cerr << "WorldMap: getting an out-of-bounds tile, at [" << pos.x << "," << pos.y << "]" << std::endl;
		return m_map.at(pos.y * m_size.x + pos.x);
	}

	const Tile &at(unsigned x, unsigned y) const {
		return at(Vector2u(x, y));
	}

	const Vector2u size() const { return m_size; }
	unsigned width() const { return m_size.x; }
	unsigned height() const { return m_size.y; }

private:
	Vector2u m_size;
	std::vector<Tile> m_map;

	// std::list<Entities> m_entities;
	// std::list<GameObjects> m_gameObjects;
};