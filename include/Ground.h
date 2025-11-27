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
	void updateZoom();

	DrawableWithSrc m_groundTile;
private:

};