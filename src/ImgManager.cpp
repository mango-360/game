#include "ImgManager.h"
#include "Presenter.h"

SDL_Texture* ImgManager::m_grassBlockTexture = nullptr;
SDL_Texture* ImgManager::m_dirtBlockTexture = nullptr;
SDL_Texture* ImgManager::m_bushTexture = nullptr;

ImgManager::ImgManager()
{
}

ImgManager::~ImgManager()
{
}

void ImgManager::loadAllTextures()
{
	string ImgsFile = "ImgsFile.txt", grassBlockImg, dirtBlockImg, bushImg;
	fstream stream;
	stream.open(CONFIG_FOLDER + ImgsFile);

	stream >> grassBlockImg;
	stream >> dirtBlockImg;
	stream >> bushImg;

	stream.close();

	m_grassBlockTexture = loadTexture(TILES_FOLDER + grassBlockImg);
	m_dirtBlockTexture = loadTexture(TILES_FOLDER + dirtBlockImg);
	m_bushTexture = loadTexture(TILES_FOLDER + bushImg);
}

void ImgManager::destroyAllTextures()
{
	SDL_DestroyTexture(m_grassBlockTexture);
	SDL_DestroyTexture(m_dirtBlockTexture);
	SDL_DestroyTexture(m_bushTexture);
}
