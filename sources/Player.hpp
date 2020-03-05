/*
** wolf, 2020
** Player.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

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
		Vector2f predict(m_position + m_direction * speed);
		if (map.isInside(predict.x, m_position.y) && map.at(predict.x, m_position.y) == 0)
			m_position.x = predict.x;
		if (map.isInside(m_position.x, predict.y) && map.at(m_position.x, predict.y) == 0)
			m_position.y = predict.y;
	}

	void moveBackward(const WorldMap &map, double speed)
	{
		Vector2f predict(m_position - m_direction * speed);
		if (map.isInside(predict.x, m_position.y) && map.at(predict.x, m_position.y) == 0)
			m_position.x = predict.x;
		if (map.isInside(m_position.x, predict.y) && map.at(m_position.x, predict.y) == 0)
			m_position.y = predict.y;
	}

	void moveLeft(const WorldMap &map, double speed)
	{
		Vector2f predict(m_position + m_direction.rotated(-M_PI / 2) * speed);
		if (map.isInside(predict.x, m_position.y) && map.at(predict.x, m_position.y) == 0)
			m_position.x = predict.x;
		if (map.isInside(m_position.x, predict.y) && map.at(m_position.x, predict.y) == 0)
			m_position.y = predict.y;
	}

	void moveRight(const WorldMap &map, double speed)
	{
		Vector2f predict(m_position + m_direction.rotated(M_PI / 2) * speed);
		if (map.isInside(predict.x, m_position.y) && map.at(predict.x, m_position.y) == 0)
			m_position.x = predict.x;
		if (map.isInside(m_position.x, predict.y) && map.at(m_position.x, predict.y) == 0)
			m_position.y = predict.y;
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
	Vector2f m_position, m_direction, m_plane;
};