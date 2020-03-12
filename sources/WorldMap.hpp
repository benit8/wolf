/*
** wolf, 2020
** WorldMap.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include "Utils/Vector2.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

////////////////////////////////////////////////////////////////////////////////

class WorldMap
{
	typedef uint8_t Tile;

public:
	explicit WorldMap(std::string &&filename)
	{
		if (!loadFromFile(std::move(filename)))
			throw std::runtime_error("Failed to load WorldMap");
	}

	~WorldMap() = default;


	bool loadFromFile(std::string &&filename)
	{
		uint8_t titleLength = 0;
		FILE *fp = fopen(filename.c_str(), "rb");
		if (fp == NULL) {
			fprintf(stderr, "Error opening WorldMap: %m\n");
			return false;
		}

		if (fread(&m_size.x, sizeof(uint16_t), 1, fp) != 1)
			goto error;
		if (fread(&m_size.y, sizeof(uint16_t), 1, fp) != 1)
			goto error;

		if (fread(&titleLength, sizeof(uint8_t), 1, fp) != 1)
			goto error;

		m_title.assign(titleLength + 1, '\0');
		if (fread(&m_title[0], sizeof(char), titleLength, fp) != titleLength)
			goto error;

		m_map.assign(m_size.x * m_size.y, Tile());
		if (fread(&m_map[0], sizeof(Tile), m_size.x * m_size.y, fp) != m_size.x * m_size.y)
			goto error;

		if (fread(&m_initialPlayerPos.x, sizeof(uint16_t), 1, fp) != 1)
			goto error;
		if (fread(&m_initialPlayerPos.y, sizeof(uint16_t), 1, fp) != 1)
			goto error;

error:
		fclose(fp);
		return true;
	}


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

	const Tile &at(uint16_t x, uint16_t y) const {
		return at(Vector2u(x, y));
	}

	const Vector2<uint16_t> size() const { return m_size; }
	uint16_t width() const { return m_size.x; }
	uint16_t height() const { return m_size.y; }
	const std::string &title() const { return m_title; }
	const Vector2f initialPlayerPos() const { return { m_initialPlayerPos.x + 0.5f, m_initialPlayerPos.y + 0.5f }; }

private:
	WorldMap() = default;

private:
	Vector2<uint16_t> m_size;
	std::string m_title;
	std::vector<Tile> m_map;
	Vector2<uint16_t> m_initialPlayerPos;

	// std::list<Entities> m_entities;
	// std::list<GameObjects> m_gameObjects;
};