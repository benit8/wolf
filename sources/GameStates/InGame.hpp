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
	explicit InGame(StateManager &m, WorldMap &&map)
	: State(m)
	, m_worldMap(map)
	, m_player({20.5, 20.5}, {-1, 0})
	{
		SDL_SetRelativeMouseMode(SDL_TRUE);
	}

	~InGame()
	{
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}

	void handleEvent(SDL_Event e)
	{
		switch (e.type) {
		case SDL_MOUSEMOTION:
			m_player.rotate(e.motion.xrel / (200 - MouseSensitivity));
			break;
		case SDL_KEYDOWN:
			handleKeydown(e.key);
			break;
		default:
			break;
		}
	}

	void update(double elapsed)
	{
		const double moveAmount = elapsed * MoveSpeed;
		const double rotationAmount = elapsed * RotationSpeed;

		const Uint8 *state = SDL_GetKeyboardState(NULL);
		if (state[SDL_SCANCODE_W]) m_player.moveForward(m_worldMap, moveAmount);
		if (state[SDL_SCANCODE_S]) m_player.moveBackward(m_worldMap, moveAmount);
		if (state[SDL_SCANCODE_Q]) m_player.moveLeft(m_worldMap, moveAmount);
		if (state[SDL_SCANCODE_E]) m_player.moveRight(m_worldMap, moveAmount);
		if (state[SDL_SCANCODE_A]) m_player.rotate(-rotationAmount);
		if (state[SDL_SCANCODE_D]) m_player.rotate(rotationAmount);
	}

	void render(SDL_Renderer *renderer) const
	{
		Vector2i _size;
		SDL_GetRendererOutputSize(renderer, &_size.x, &_size.y);
		Vector2u rendererSize(_size);

		for (size_t x = 0; x < rendererSize.x; ++x) {
			auto ray = castRay(x, rendererSize.x);
			if (ray.distance < 0)
				continue;

			int lineHeight = std::min(rendererSize.y / ray.distance, (double)rendererSize.y);
			int start = -lineHeight / 2.0 + rendererSize.y / 2.0;
			int end   =  lineHeight / 2.0 + rendererSize.y / 2.0;

			uint8_t c = ray.wallSide ? 128 : 255;
			switch (m_worldMap.at(ray.pos)) {
				case 1:  SDL_SetRenderDrawColor(renderer, c, 0, 0, 255); break;
				case 2:  SDL_SetRenderDrawColor(renderer, 0, c, 0, 255); break;
				case 3:  SDL_SetRenderDrawColor(renderer, 0, 0, c, 255); break;
				case 4:  SDL_SetRenderDrawColor(renderer, c, c, c, 255); break;
				default: SDL_SetRenderDrawColor(renderer, c, c, 0, 255); break;
			}

			SDL_RenderDrawLine(renderer, x, start, x, end);
		}
	}

private:
	void handleKeydown(SDL_KeyboardEvent e)
	{
		switch (e.keysym.sym) {
		case SDLK_ESCAPE:
			pause();
			m_manager.pushState<PauseMenu>();
			break;
		default:
			break;
		}
	}

	Ray castRay(double x, double width) const
	{
		double camera = 2 * x / width - 1.0;
		Ray ray(m_player.direction() + m_player.plane() * camera, m_player.position());
		Vector2f deltaDist(std::abs(1 / ray.dir.x), std::abs(1 / ray.dir.y));
		Vector2i step(ray.dir.x < 0 ? -1 : 1, ray.dir.y < 0 ? -1 : 1);

#define SIDE_DIST(O) (ray.dir.O < 0 ? (m_player.position().O - ray.pos.O) * deltaDist.O : (ray.pos.O + 1.0 - m_player.position().O) * deltaDist.O)
		Vector2f sideDist(SIDE_DIST(x), SIDE_DIST(y));
#undef SIDE_DIST

		bool hit = false;
		while (!hit) {
			if (sideDist.x < sideDist.y) {
				sideDist.x += deltaDist.x;
				ray.pos.x += step.x;
				ray.wallSide = false;
			}
			else {
				sideDist.y += deltaDist.y;
				ray.pos.y += step.y;
				ray.wallSide = true;
			}

			if (!m_worldMap.isInside(ray.pos))
				break;

			hit = m_worldMap.at(ray.pos) > 0;
		}

#define PERP_WALL_DIST(O) ((ray.pos.O - m_player.position().O + (1.0 - step.O) / 2.0) / ray.dir.O)
#define WALL_DIST() (ray.wallSide ? PERP_WALL_DIST(y) : PERP_WALL_DIST(x))
		ray.distance = hit ? WALL_DIST() : -1;
#undef PERP_WALL_DIST
#undef WALL_DIST

		return ray;
	}

private:
	WorldMap m_worldMap;
	Player m_player;
};

////////////////////////////////////////////////////////////////////////////////

}