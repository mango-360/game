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
	m_tile.rect = { 0, 0, static_cast<int>(TILE_SIZE * InputManager::getZoom()), static_cast<int>(TILE_SIZE * InputManager::getZoom()) };
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

void Tile::draw(float2 camCoords)
{
	
	m_tile.rect.x = (m_gridCoords.x - camCoords.x) * (TILE_SIZE * InputManager::getZoom());
	m_tile.rect.y = (m_gridCoords.y - camCoords.y) * (TILE_SIZE * InputManager::getZoom());

	//cout << tmp.rect.x << ", " << tmp.rect.y << std::endl;

	drawObject(m_tile);
}

void Tile::destroy()
{
	SDL_DestroyTexture(m_tile.texture);

}

void Tile::updateZoom()
{
	if (InputManager::isZoomChanged())
	{
		m_tile.rect.w = TILE_SIZE * InputManager::getZoom();
		m_tile.rect.h = TILE_SIZE * InputManager::getZoom();
	}
}