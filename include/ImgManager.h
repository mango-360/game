#pragma once

#include "defines.h"

class ImgManager
{
public:

	ImgManager();
	~ImgManager();

	static SDL_Texture* m_grassBlockTexture;
	static SDL_Texture* m_dirtBlockTexture;

	void loadAllTextures();
	void destroyAllTextures();
};