#include "TitleScreen.h"
#include "World.h"

extern World world;

TitleScreen::TitleScreen()
{
}

TitleScreen::~TitleScreen()
{
}

void TitleScreen::init()
{
	m_background.rect = { 0, 0, Presenter::m_SCREEN_WIDTH, Presenter::m_SCREEN_HEIGHT };
	m_background.texture = loadTexture("TitleScreen.bmp");
}

void TitleScreen::run()
{
	drawObject(m_background);

	if (InputManager::isMousePressed() || InputManager::isAnyKeyPressed())
	{
		world.m_stateManager.changeState(GAME_STATE::GAME);
		printf("Changing state to GAME\n");
		return;
	}
}

void TitleScreen::destroy()
{
	SDL_DestroyTexture(m_background.texture);
}