#pragma once

#include "Presenter.h"
#include "InputManager.h"
#include "StateManager.h"
#include "SoundManager.h"
#include "ImgManager.h"

class World
{
public:
	World();
	~World();

	Presenter m_presenter;
	SoundManager m_soundManager;
	InputManager m_inputManager;
	StateManager m_stateManager;
	ImgManager m_imgManager;

	void init();
	void run();
	void destroy();

	bool isRunning();

private:
	bool m_running;
};