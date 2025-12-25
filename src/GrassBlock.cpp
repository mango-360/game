#include "GrassBlock.h"
#include "ImgManager.h"

GrassBlock::GrassBlock()
{
}

GrassBlock::~GrassBlock()
{
}

void GrassBlock::init(int2 coords)
{
	m_tile.texture = ImgManager::m_grassBlockTexture;
	m_type = TILE_TYPE::GRASSBLOCK;
	m_tile.rect = { 0, 0, TILE_SIZE, TILE_SIZE };
	m_hp = 1;
	m_gridCoords = coords;
	updateZoom();
}
