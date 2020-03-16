/*
** wolf, 2020
** InGame.cpp
*/

#include "GameStates/InGame.hpp"
#include "StateManager.hpp"

#include "SDL++/Mouse.hpp"
#include "SDL++/Keyboard.hpp"

#include <iostream>

////////////////////////////////////////////////////////////////////////////////

namespace GameStates
{

////////////////////////////////////////////////////////////////////////////////

InGame::InGame(StateManager &m, std::string &&mapFilename)
: State(m)
, m_worldMap(std::move(mapFilename))
, m_player(m_worldMap.initialPlayerPos(), {-1, 0})
, m_walls("assets/textures/walls.png")
{
	std::cout << "Playing on map '" << m_worldMap.title() << "'" << std::endl;
	SDL::Mouse::setRelative(true);
}

InGame::~InGame()
{
	SDL::Mouse::setRelative(false);
}

////////////////////////////////////////////////////////////////////////////////

void InGame::handleEvent(const SDL::Event &e)
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

void InGame::update(double elapsed)
{
	const double moveAmount = elapsed * MoveSpeed;
	const double rotationAmount = elapsed * RotationSpeed;

	const Uint8 *state = SDL::Keyboard::state();
	if (state[SDL_SCANCODE_W]) m_player.moveForward(m_worldMap, moveAmount);
	if (state[SDL_SCANCODE_S]) m_player.moveBackward(m_worldMap, moveAmount);
	if (state[SDL_SCANCODE_Q]) m_player.moveLeft(m_worldMap, moveAmount);
	if (state[SDL_SCANCODE_E]) m_player.moveRight(m_worldMap, moveAmount);
	if (state[SDL_SCANCODE_A]) m_player.rotate(-rotationAmount);
	if (state[SDL_SCANCODE_D]) m_player.rotate(rotationAmount);
}

void InGame::render(SDL::Texture &framebuffer) const
{
	auto fb = framebuffer.lock();
	auto wl = m_walls.lock();
	DrawHelper dh(fb);

	for (int x = 0; x < fb.width(); ++x) {
		auto ray = castRay(x, fb.width());
		if (ray.distance < 0)
			continue;

		assert(m_worldMap.at(ray.pos) > 0);

		int lineHeight = fb.height() / ray.distance;
		int start = std::max(-lineHeight / 2.0 + fb.height() / 2.0, 0.0);
		int end   = std::min( lineHeight / 2.0 + fb.height() / 2.0, (double)fb.height() - 1);

		renderWall(fb, x, ray, lineHeight, start, end, wl);
	}
}

////////////////////////////////////////////////////////////

void InGame::handleKeydown(const SDL_KeyboardEvent &e)
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

InGame::Ray InGame::castRay(double x, double width) const
{
	double camera = 2 * x / width - 1.0;
	InGame::Ray ray(m_player.direction() + m_player.plane() * camera, m_player.position());
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

void InGame::renderWall(const SDL::Texture::Lock &fb, int x, const Ray &ray, int lineHeight, int start, int end, const SDL::Surface::Lock &wl) const
{
	size_t textureIndex = m_worldMap.at(ray.pos) - 1;
	double wallHitX = ray.wallSide ? m_player.position().x + ray.distance * ray.dir.x : m_player.position().y + ray.distance * ray.dir.y;
	wallHitX -= floor(wallHitX);

	Vector2i texPoint(wallHitX * (double)m_textureSize.x, 0);
	if (!ray.wallSide && ray.dir.x > 0)
		texPoint.x = m_textureSize.x - texPoint.x - 1;
	if (ray.wallSide && ray.dir.y < 0)
		texPoint.x = m_textureSize.x - texPoint.x - 1;

	double step = 1.0 * m_textureSize.y / lineHeight;
	double texPos = (start - fb.height() / 2.0 + lineHeight / 2.0) * step;
	for (int y = start; y < end; y++) {
		texPoint.y = (int)texPos & (m_textureSize.y - 1);
		texPos += step;

		auto color = wl.at(textureIndex * m_textureSize.x + texPoint.x, texPoint.y).color();
		if (ray.wallSide) {
			color.r /= 2;
			color.g /= 2;
			color.b /= 2;
		}

		fb.at(x, y) = color;
	}
}

}