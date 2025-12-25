#include "Tile.h"
#include "InputManager.h"
#include "Presenter.h"

Tile::Tile()
{
}

Tile::~Tile()
{
}

void Tile::init(int2 coords)
{
	m_tile.texture = nullptr;
	m_tile.rect = { 0, 0, TILE_SIZE, TILE_SIZE };
	m_hp = 0;
	m_type = TILE_TYPE::NONE_TYPE;
	m_gridCoords = coords;
	updateZoom();
}

void Tile::update()
{
	updateZoom();
}

/*void Tile::draw()
{
	// legacy: draw using the tile rect as-is (world coords). Useful if camera not applied.
	drawObject(m_tile);
}*/

void Tile::draw(int2 camOffset)
{
	Drawable tmp = m_tile;
	tmp.rect.x = m_tile.rect.x - camOffset.x;
	tmp.rect.y = m_tile.rect.y - camOffset.y;

	//cout << tmp.rect.x << ", " << tmp.rect.y << std::endl;

	drawObject(tmp);
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

	m_tile.rect.x = m_gridCoords.x * m_tile.rect.w;
	m_tile.rect.y = m_gridCoords.y * m_tile.rect.h;
}