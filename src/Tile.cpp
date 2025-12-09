#include "Tile.h"
#include "InputManager.h"
#include "Presenter.h"

Tile::Tile()
{
}

Tile::~Tile()
{
}

void Tile::init()
{
	m_tile.texture = nullptr;
	m_tile.rect = { 0, 0, TILE_SIZE, TILE_SIZE };
	m_hp = 0;
	m_type = TILE_TYPE::NONE_TYPE;
}

void Tile::update()
{
	updateZoom();
}

void Tile::draw()
{
	drawObject(m_tile);
}

void Tile::destroy()
{
	SDL_DestroyTexture(m_tile.texture);
}

void Tile::setPosition(int2 coords) { m_tile.rect.x = coords.x; m_tile.rect.y = coords.y; };

void Tile::updateZoom()
{
	m_tile.rect.w = TILE_SIZE * InputManager::getZoom();
	m_tile.rect.h = TILE_SIZE * InputManager::getZoom();
}