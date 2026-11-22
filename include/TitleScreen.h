#pragma once

#include "defines.h"
#include "State.h"
#include "Presenter.h"
#include "InputManager.h"

class TitleScreen : public State
{
public:
	TitleScreen();
	~TitleScreen();

	void init();
	void run();
	void destroy();

private:
	Drawable m_background;

	// Test button that inserts a row in dbo.TestData
	SDL_Rect m_dbButton;
	Drawable m_dbButtonText;
	Drawable m_dbStatusText;

	void setStatusText(string text, COLOR color);
};