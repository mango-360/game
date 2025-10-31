#include "WinScreen.h"
#include "World.h"

extern World world;

WinScreen::WinScreen()
{

}

WinScreen::~WinScreen()
{

}

void WinScreen::init()
{
	fstream stream;

	string tmp, WinScreenBackground;

	stream.open(CONFIG_FOLDER + "WinScreen.txt");

	stream >> tmp >> WinScreenBackground;

	stream.close();

	m_background = loadTexture(WinScreenBackground);
}

void WinScreen::run()
{
	drawObject(m_background);
	if (InputManager::isMousePressed() || InputManager::isAnyKeyPressed())
	{
		world.m_stateManager.changeState(GAME_STATE::GAME);
	}
}

void WinScreen::destroy()
{
	SDL_DestroyTexture(m_background);
}