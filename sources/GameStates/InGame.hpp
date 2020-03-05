/*
** wolf, 2020
** InGame.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include "Player.hpp"
#include "State.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace GameStates
{

////////////////////////////////////////////////////////////////////////////////

class InGame : public State
{
	static constexpr double MoveSpeed = 5.0;
	static constexpr double RotationSpeed = 3.0;

public:
	explicit InGame(StateManager &m, WorldMap &&map)
	: State(m)
	, m_worldMap(map)
	, m_player({20.5, 20.5}, {-1, 0})
	{}

	~InGame()
	{}

	void handleEvent(SDL_Event) {}

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
			double camera = 2 * x / (double)rendererSize.x - 1.0;
			Vector2f rayDir(m_player.direction() + m_player.plane() * camera);
			Vector2u mapPos(m_player.position());
			Vector2f deltaDist(std::abs(1 / rayDir.x), std::abs(1 / rayDir.y));

			Vector2i step(rayDir.x < 0 ? -1 : 1, rayDir.y < 0 ? -1 : 1);
			Vector2f sideDist(
				rayDir.x < 0 ? (m_player.position().x - mapPos.x) * deltaDist.x : (mapPos.x + 1.0 - m_player.position().x) * deltaDist.x,
				rayDir.y < 0 ? (m_player.position().y - mapPos.y) * deltaDist.y : (mapPos.y + 1.0 - m_player.position().y) * deltaDist.y
			);

			bool hit = false, side = false;
			while (!hit) {
				if (sideDist.x < sideDist.y) {
					sideDist.x += deltaDist.x;
					mapPos.x += step.x;
					side = false;
				}
				else {
					sideDist.y += deltaDist.y;
					mapPos.y += step.y;
					side = true;
				}

				if (!m_worldMap.isInside(mapPos))
					break;

				hit = m_worldMap.at(mapPos) > 0;
			}

			if (!hit)
				continue;

#define PERP_WALL_DIST(O) ((mapPos.O - m_player.position().O + (1.0 - step.O) / 2.0) / rayDir.O)
			double perpWallDist = side ? (PERP_WALL_DIST(y)) : (PERP_WALL_DIST(x));
#undef PERP_WALL_DIST

			int lineHeight = std::min(rendererSize.y / perpWallDist, (double)rendererSize.y);
			int start = -lineHeight / 2.0 + rendererSize.y / 2.0;
			int end   =  lineHeight / 2.0 + rendererSize.y / 2.0;

			uint8_t c = side ? 128 : 255;
			switch (m_worldMap.at(mapPos)) {
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
	WorldMap m_worldMap;
	Player m_player;
};

////////////////////////////////////////////////////////////////////////////////

}