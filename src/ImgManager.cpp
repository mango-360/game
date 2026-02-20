#include "ImgManager.h"
#include "Presenter.h"

SDL_Texture* ImgManager::m_grassBlockTexture = nullptr;
SDL_Texture* ImgManager::m_dirtBlockTexture = nullptr;

ImgManager::ImgManager()
{
}

ImgManager::~ImgManager()
{
}

void ImgManager::loadAllTextures()
{
	string ImgsFile = "ImgsFile.txt", grassBlockImg, dirtBlockImg;
	fstream stream;
	stream.open(CONFIG_FOLDER + ImgsFile);

	stream >> grassBlockImg;
	stream >> dirtBlockImg;

	stream.close();

	m_grassBlockTexture = loadTexture(TILES_FOLDER + grassBlockImg);
	m_dirtBlockTexture = loadTexture(TILES_FOLDER + dirtBlockImg);
}

void ImgManager::destroyAllTextures()
{
	SDL_DestroyTexture(m_grassBlockTexture);
	SDL_DestroyTexture(m_dirtBlockTexture);
}
