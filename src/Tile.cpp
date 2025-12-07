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
	m_tile.rect = { 0, 0, static_cast<int>(TILE_SIZE * InputManager::getZoom()), static_cast<int>(TILE_SIZE * InputManager::getZoom()) };
	m_hp = 0;
	m_type = TILE_TYPE::NONE;
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

void Tile::updateZoom()
{
	m_tile.rect.w = TILE_SIZE * InputManager::getZoom();
	m_tile.rect.h = TILE_SIZE * InputManager::getZoom();
}