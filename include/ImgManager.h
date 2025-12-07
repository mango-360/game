#pragma once

#include "defines.h"

class ImgManager
{
public:

	ImgManager();
	~ImgManager();

	static SDL_Texture* m_grassBlockTexture;

	void loadAllTextures();
	void destroyAllTextures();
};