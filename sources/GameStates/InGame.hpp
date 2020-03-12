/*
** wolf, 2020
** InGame.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include "Player.hpp"
#include "State.hpp"

#include "GameStates/PauseMenu.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace GameStates
{

////////////////////////////////////////////////////////////////////////////////

class InGame : public State
{
	static constexpr double MoveSpeed = 5.0;
	static constexpr double RotationSpeed = 3.0;
	static constexpr double MouseSensitivity = 50.0;

	struct Ray
	{
		Vector2f dir;
		Vector2u pos;
		double distance = 0;
		bool wallSide = false;

		Ray(const Vector2f &d, const Vector2f &p)
		: dir(d)
		, pos(p)
		{}
	};

public:
	explicit InGame(StateManager &m, std::string &&mapFilename);
	~InGame();

	void handleEvent(SDL_Event e);
	void update(double elapsed);
	void render(SDL_Renderer *renderer) const;

private:
	void handleKeydown(SDL_KeyboardEvent e);
	Ray castRay(double x, double width) const;

private:
	WorldMap m_worldMap;
	Player m_player;
};

////////////////////////////////////////////////////////////////////////////////

}