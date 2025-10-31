#pragma once

#include "Game.h"
#include "TitleScreen.h"
#include "WinScreen.h"
#include "defines.h"

class StateManager
{
public:
	StateManager();
	~StateManager();

	GAME_STATE m_currentState;

	Game* m_game;
	TitleScreen* m_titleScreen;
	WinScreen* m_WinScreen;

	State* m_state;

	void init(GAME_STATE state);
	void run();
	void destroy();

	void changeState(GAME_STATE newState);

private:
	void initNewState();
};