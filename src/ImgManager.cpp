#include "ImgManager.h"
#include "Presenter.h"

SDL_Texture* ImgManager::m_grassBlockTexture = nullptr;

ImgManager::ImgManager()
{
}

ImgManager::~ImgManager()
{
}

void ImgManager::loadAllTextures()
{
	string ImgsFile = "ImgsFile.txt", grassBlockImg;
	fstream stream;
	stream.open(CONFIG_FOLDER + ImgsFile);

	stream >> grassBlockImg;

	stream.close();

	m_grassBlockTexture = loadTexture(grassBlockImg);

	if (m_grassBlockTexture) {
		SDL_SetTextureScaleMode(m_grassBlockTexture, SDL_ScaleModeNearest); // prevents linear filtering for this texture
	}
}

void ImgManager::destroyAllTextures()
{
	SDL_DestroyTexture(m_grassBlockTexture);
}
