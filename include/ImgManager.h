#pragma once

#include "defines.h"

class ImgManager
{
public:

	ImgManager();
	~ImgManager();

	static SDL_Texture* m_grassBlockTexture;
	static SDL_Texture* m_dirtBlockTexture;
	static SDL_Texture* m_bushTexture;

	static SDL_Texture* m_stickTexture;

	static SDL_Texture* m_slotImg;
	static SDL_Texture* m_activeSlotImg;

	void loadAllTextures();
	void destroyAllTextures();
};