#pragma once

#include "defines.h"
#include "State.h"
#include "Presenter.h"
#include "InputManager.h"

class WinScreen : public State
{
public:
	WinScreen();
	~WinScreen();

	void init();
	void run();
	void destroy();

private:
	SDL_Texture* m_background;

	int m_winner;
};