#include "DirtBlock.h"
#include "ImgManager.h"
#include "InputManager.h"

DirtBlock::DirtBlock()
{
}

DirtBlock::~DirtBlock()
{
}

void DirtBlock::init(float2 coords)
{
	m_tile.texture = ImgManager::m_dirtBlockTexture;
	m_type = TILE_TYPE::DIRTBLOCK;
	m_tile.rect = { 0, 0, static_cast<int>(TILE_SIZE * InputManager::getZoom()), static_cast<int>(TILE_SIZE * InputManager::getZoom()) };
	m_hp = 1;
	m_gridRect = { coords.x, coords.y, 1, 1 };
	m_friction = 0.5f;
}