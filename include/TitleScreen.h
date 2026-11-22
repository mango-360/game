#pragma once

#include "defines.h"
#include "State.h"
#include "Presenter.h"
#include "InputManager.h"
#include "Button.h"

class TitleScreen : public State
{
public:
	TitleScreen();
	~TitleScreen();

	void init();
	void run();
	void destroy();

private:
	Drawable m_background = {};
	Drawable m_welcomeText = {};

	Button m_profileButton; // top left, opens the profile page
};
