#include "World.h"
#include "InputManager.h"
#include <cmath>
#include <algorithm>

extern World world;

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::init(Ground* ground)
{
	string tmp, cameraFile = "camera.txt";

	fstream stream;

	stream.open(CONFIG_FOLDER + cameraFile);

	stream >> tmp >> position.x >> position.y;
	stream >> tmp >> zoom;
	stream >> tmp >> moveSpeed;

	stream.close();

	m_ground = ground;

	// Ensure ground tile size is consistent with zoom after init
	if (m_ground)
	{
		int tileSize = std::max(1, static_cast<int>(32 * zoom));
		m_ground->m_groundTile.rect.w = tileSize;
		m_ground->m_groundTile.rect.h = tileSize;
	}
}

void Camera::update()
{
	zoomUpdate();
	move();
}

void Camera::draw()
{
	
	int tileSize = static_cast<int>(32 * zoom);

	int firstTileX = static_cast<int>(floor(position.x / static_cast<double>(tileSize)));
	int firstTileY = static_cast<int>(floor(position.y / static_cast<double>(tileSize)));

	// How many tiles are needed to fill the screen (add 2 for margin)
	int tilesX = (1920 / tileSize) + 1;
	int tilesY = (1080 / tileSize) + 1;

	for (int tx = firstTileX; tx <= firstTileX + tilesX; ++tx)
	{
		for (int ty = firstTileY; ty <= firstTileY + tilesY; ++ty)
		{
			if (world.m_stateManager.m_game->m_board.m_map[ty][tx] == Tile::AIR) continue;

			int destX = tx * tileSize - position.x;
			int destY = ty * tileSize - position.y;

			m_ground->m_groundTile.rect.x = destX;
			m_ground->m_groundTile.rect.y = destY;

			drawObject(m_ground->m_groundTile);
		}
	}
}

void Camera::move()
{
	if (InputManager::isKeyPressed(SDL_SCANCODE_D))
	{
		if (InputManager::isKeyPressed(SDL_SCANCODE_A))
		{
			if (lastKeyPressed == SDL_SCANCODE_D)
			{
				position.x -= static_cast<int>(moveSpeed * zoom);
			}
			else
			{
				position.x += static_cast<int>(moveSpeed * zoom);
			}
		}
		else
		{
			position.x += static_cast<int>(moveSpeed * zoom);
			lastKeyPressed = SDL_SCANCODE_D;
		}
	}
	else if (InputManager::isKeyPressed(SDL_SCANCODE_A))
	{
		position.x -= static_cast<int>(moveSpeed * zoom);
		lastKeyPressed = SDL_SCANCODE_A;
	}
}

void Camera::zoomUpdate()
{
	if (!m_ground) return;

	const float zoomStep = 0.1f;
	const float minZoom = 0.25f;
	const float maxZoom = 4.0f;

	if (InputManager::isKeyPressed(SDL_SCANCODE_Q))
	{
		zoom = std::min(maxZoom, zoom + zoomStep);
	}
	else if (InputManager::isKeyPressed(SDL_SCANCODE_E))
	{
		zoom = std::max(minZoom, zoom - zoomStep);
	}

	// Ensure ground tile size is updated when zoom changes
	int tileSize = std::max(1, static_cast<int>(32 * zoom));
	m_ground->m_groundTile.rect.w = tileSize;
	m_ground->m_groundTile.rect.h = tileSize;
}

int2 Camera::getPosition()
{
	return position;
}

