#pragma once

#include "Engine.h"
#include "TextField.h"
#include "Player.h"
#include <string>

class DialogPopUp : public TextField
{
public:
	DialogPopUp();
	~DialogPopUp();
	void init(string configFile, SDL_FRect* rect, Player* player);
	void update();
	void draw();
	bool getState() { return isTriggered; }
private:
	void updateTriggerRect();
	void getNextLine();
	void triggerDialog();

	SDL_FRect triggerRect;
	SDL_FRect* objectRect;

	Player* m_player;

	vector<string> dialog;

	int dialogLine;

	bool isTriggered;
};