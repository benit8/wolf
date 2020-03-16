/*
** wolf, 2020
** InGame.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include "Player.hpp"
#include "State.hpp"

#include "GameStates/PauseMenu.hpp"

#include "SDL++/Surface.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace GameStates
{

////////////////////////////////////////////////////////////////////////////////

class InGame : public State
{
	static constexpr double MoveSpeed = 5.0;
	static constexpr double RotationSpeed = 3.0;
	static constexpr double MouseSensitivity = 50.0;

	const Vector2u m_textureSize {64, 64};

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

	void handleEvent(const SDL::Event &e);
	void update(double elapsed);
	void render(SDL::Texture &framebuffer) const;

private:
	void handleKeydown(const SDL_KeyboardEvent &e);
	Ray castRay(double x, double width) const;
	void renderWall(const SDL::Texture::Lock &fb, int x, const Ray &ray, int lineHeight, int start, int end, const SDL::Surface::Lock &wl) const;

private:
	WorldMap m_worldMap;
	Player m_player;
	SDL::Surface m_walls;
};

////////////////////////////////////////////////////////////////////////////////

}