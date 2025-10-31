#pragma once

#include "Board.h"
#include "State.h"

class Game : public State
{
public: 
	Game();
	~Game();

	Board m_board;

	void init();
	void run();
	void destroy();
};