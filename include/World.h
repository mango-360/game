#pragma once

#include "Presenter.h"
#include "InputManager.h"
#include "StateManager.h"
#include "SoundManager.h"
#include "ImgManager.h"
#include "Database.h"

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
	Database m_database;

	// Id of the logged in player in dbo.Players, -1 if nobody is logged in
	int m_playerId = -1;

	void init();
	void run();
	void destroy();

	bool isRunning();

private:
	bool m_running;
};