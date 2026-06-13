#include "Bush.h"
#include "ImgManager.h"
#include "InputManager.h"

Bush::Bush()
{
}

Bush::~Bush()
{
}

void Bush::init(float2 coords)
{
	m_tile.texture = ImgManager::m_bushTexture;
	m_type = TILE_TYPE::BUSH;
	m_drop = DROP_TYPE::STICK;
	m_tile.rect = { 0, 0, static_cast<int>(TILE_SIZE * InputManager::getZoom()), static_cast<int>(TILE_SIZE * InputManager::getZoom()) };
	m_hp = 10;
	m_gridRect = { coords.x, coords.y, 1, 1 };
	m_friction = 4.0f;
	m_isSolid = false;
}