#include "ImgManager.h"
#include "Presenter.h"

SDL_Texture* ImgManager::m_grassBlockTexture = nullptr;
SDL_Texture* ImgManager::m_dirtBlockTexture = nullptr;
SDL_Texture* ImgManager::m_bushTexture = nullptr;
SDL_Texture* ImgManager::m_stickTexture = nullptr;
SDL_Texture* ImgManager::m_slotImg = nullptr;
SDL_Texture* ImgManager::m_activeSlotImg = nullptr;

ImgManager::ImgManager()
{
}

ImgManager::~ImgManager()
{
}

void ImgManager::loadAllTextures()
{
	string ImgsFile = "ImgsFile.txt", tmp, grassBlockImg, dirtBlockImg, bushImg, slotImg, activeSlotImg;
	string stickImg;
	fstream stream;
	stream.open(CONFIG_FOLDER + ImgsFile);

	stream >> tmp;

	stream >> grassBlockImg;
	stream >> dirtBlockImg;
	stream >> bushImg;

	stream >> tmp;

	stream >> stickImg;

	stream >> tmp >> slotImg;
	stream >> tmp >> activeSlotImg;

	stream.close();

	m_grassBlockTexture = loadTexture(TILES_FOLDER + grassBlockImg);
	m_dirtBlockTexture = loadTexture(TILES_FOLDER + dirtBlockImg);
	m_bushTexture = loadTexture(TILES_FOLDER + bushImg);

	m_stickTexture = loadTexture(TILES_FOLDER + DROPS_FOLDER + stickImg);

	m_slotImg = loadTexture(slotImg);
	m_activeSlotImg = loadTexture(activeSlotImg);
}

void ImgManager::destroyAllTextures()
{
	SDL_DestroyTexture(m_grassBlockTexture);
	SDL_DestroyTexture(m_dirtBlockTexture);
	SDL_DestroyTexture(m_bushTexture);

	SDL_DestroyTexture(m_stickTexture);

	SDL_DestroyTexture(m_slotImg);
	SDL_DestroyTexture(m_activeSlotImg);
}
