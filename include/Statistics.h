#pragma once 

#include "defines.h"
#include "TextField.h"

class Statistics
{
public:
	Statistics();
	~Statistics();
	void init();
	void update();
	void draw();
	static void drawHitboxes(static DrawableWithOpacity& drawable);
	void destroy();

	void toggleStatistics();

	bool drawStatistics = false;
private:
	void toggleHitboxDraw();

	TextField m_position;

	SDL_Rect m_keyboardRect;
	DrawableWithSrc m_keyboard[4];

	static bool drawHitbox;
};