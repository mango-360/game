#include "ground.h"
#include "Presenter.h"

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

//void Ground::draw() // how to do this better?
//{
//	for(int i = 0; i < 1920; i += m_groundTile.rect.w)
//	{
//		m_groundTile.rect.x = i;
//		drawObject(m_groundTile);
//	}
//}

void Ground::destroy()
{
	SDL_DestroyTexture(m_groundTile.texture);
}