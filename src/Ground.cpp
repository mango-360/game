#include "ground.h"
#include "Presenter.h"
#include "InputManager.h"

Ground::Ground()
{
}

Ground::~Ground()
{
}

void Ground::init()
{
	string groundFile = "ground.txt", tmp, groundImg;
	fstream stream;
	stream.open(CONFIG_FOLDER + groundFile);

	stream >> tmp >> groundImg;
	stream >> tmp >> m_groundTile.srcRect.x >> m_groundTile.srcRect.y >> m_groundTile.srcRect.w >> m_groundTile.srcRect.h;
	stream >> tmp >> m_groundTile.rect.x >> m_groundTile.rect.y >> m_groundTile.rect.w >> m_groundTile.rect.h;
	m_groundTile.texture = loadTexture(groundImg);

	if (m_groundTile.texture) {
		SDL_SetTextureScaleMode(m_groundTile.texture, SDL_ScaleModeNearest); // prevents linear filtering for this texture
	}

	stream.close();
}

void Ground::update()
{
	updateZoom();
}	

void Ground::destroy()
{
	SDL_DestroyTexture(m_groundTile.texture);
}

void Ground::updateZoom()
{
	m_groundTile.rect.w = m_groundTile.srcRect.w * InputManager::getZoom();
	m_groundTile.rect.h = m_groundTile.srcRect.h * InputManager::getZoom();
}