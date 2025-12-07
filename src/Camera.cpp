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

	prevZoom = InputManager::getZoom();
	prevTileSize = static_cast<int>(TILE_SIZE * prevZoom);
}

void Camera::update()
{
	move();

	if (InputManager::getZoom() != prevZoom) zoom(); //if zoom changed

	updateTileSize();
}

void Camera::draw()
{
	drawMap();
}

void Camera::move()
{

}

void Camera::zoom()
{
	float halfWidth = static_cast<float>(Presenter::m_SCREEN_WIDTH) / 2.0f;
	 
	position.x += halfWidth / prevTileSize * (InputManager::getZoom() - prevZoom);
	position.y += halfWidth / prevTileSize * (InputManager::getZoom() - prevZoom);

	prevZoom = InputManager::getZoom();
}

void Camera::updateTileSize()
{
	prevTileSize = static_cast<int>(TILE_SIZE * InputManager::getZoom());
}

void Camera::drawMap()
{
	int startingX = -1 * (fmod(position.x, 1.0) * prevTileSize); // starting pixel x position
	int startingY = -1 * (fmod(position.y, 1.0) * prevTileSize); // starting pixel y position

	for(int i = startingY; i <= Presenter::m_SCREEN_HEIGHT; i += prevTileSize)
	{
		for(int j = startingX; j <= Presenter::m_SCREEN_WIDTH; j += prevTileSize)
		{
			m_ground->m_groundTile.rect.y = i;
			m_ground->m_groundTile.rect.x = j;
			
			drawObject(m_ground->m_groundTile);
		}
	}
}
