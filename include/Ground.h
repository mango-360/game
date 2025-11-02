#pragma once

#include "defines.h"

class Ground
{
	public:
	Ground();
	~Ground();
	void init();
	void draw();
	void destroy();
private:
	DrawableWithSrc m_groundTile;
};