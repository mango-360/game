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
};