#include "GrassBlock.h"
#include "ImgManager.h"
#include "InputManager.h"

GrassBlock::GrassBlock()
{
}

GrassBlock::~GrassBlock()
{
}

void GrassBlock::init(float2 coords)
{
	m_tile.texture = ImgManager::m_grassBlockTexture;
	m_type = TILE_TYPE::GRASSBLOCK;
	m_tile.rect = { 0, 0, static_cast<int>(TILE_SIZE * InputManager::getZoom()), static_cast<int>(TILE_SIZE * InputManager::getZoom()) };
	m_hp = 1;
	m_gridRect = { coords.x, coords.y, 1, 1 };
	updateZoom();
}
