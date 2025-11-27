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

void Camera::init(Player* player, Ground* ground)
{
	string tmp, cameraFile = "camera.txt";

	fstream stream;

	stream.open(CONFIG_FOLDER + cameraFile);

	stream >> tmp >> moveSpeed;

	stream.close();

	// Ensure ground tile size is consistent with zoom after init
	/*if (m_ground)
	{
		int tileSize = max(1, static_cast<int>(TILE_SIZE * zoom));
		m_ground->m_groundTile.rect.w = tileSize;
		m_ground->m_groundTile.rect.h = tileSize;
	}*/

	m_player = player;
	m_ground = ground;
}

void Camera::update()
{
	move();
}

void Camera::draw()
{
	
	
}

void Camera::move()
{

}

void Camera::updateMap()
{
	m_ground->updateZoom();
	int tileSize = static_cast<int>(TILE_SIZE * InputManager::getZoom());

	int firstTileX = static_cast<int>(floor(position.x / static_cast<double>(tileSize)));
	int firstTileY = static_cast<int>(floor(position.y / static_cast<double>(tileSize)));

	// How many tiles are needed to fill the screen (add 2 for margin)
	int tilesX = (1920 / tileSize) + 1;
	int tilesY = (1080 / tileSize) + 1;
}

void Camera::drawMap()
{
	

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
