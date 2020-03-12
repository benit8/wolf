/*
** wolf, 2020
** Player.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include "WorldMap.hpp"
#include "Utils/Vector2.hpp"

////////////////////////////////////////////////////////////////////////////////

class Player
{
public:
	explicit Player(const Vector2f &pos, const Vector2f &dir)
	: m_position(pos)
	, m_direction(dir)
	, m_plane(dir.rotated(M_PI / 2))
	{}

	~Player() = default;

	void moveForward(const WorldMap &map, double speed)
	{
		moveTo(m_position + m_direction * speed, map);
	}

	void moveBackward(const WorldMap &map, double speed)
	{
		moveTo(m_position - m_direction * speed, map);
	}

	void moveLeft(const WorldMap &map, double speed)
	{
		moveTo(m_position + m_direction.rotated(-M_PI / 2) * speed, map);
	}

	void moveRight(const WorldMap &map, double speed)
	{
		moveTo(m_position + m_direction.rotated(M_PI / 2) * speed, map);
	}

	void rotate(double speed)
	{
		m_direction.rotate(speed);
		m_plane.rotate(speed);
	}


	const Vector2f &position() const { return m_position; }
	const Vector2f &direction() const { return m_direction; }
	const Vector2f &plane() const { return m_plane; }

private:
	void moveTo(const Vector2f &nextPos, const WorldMap &map)
	{
		if (map.isInside(nextPos.x, m_position.y) && map.at(nextPos.x, m_position.y) == 0)
			m_position.x = nextPos.x;
		if (map.isInside(m_position.x, nextPos.y) && map.at(m_position.x, nextPos.y) == 0)
			m_position.y = nextPos.y;
	}

private:
	Vector2f m_position, m_direction, m_plane;
};