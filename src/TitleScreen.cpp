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

	m_profileButton.init("PROFILE", { 30, 30, 240, 70 }, false, 34);

	createText(m_welcomeText, "Hi, " + world.m_playerName + "! Click or press Enter to play", WHITE, 40, 0, Presenter::m_SCREEN_HEIGHT - 120);
	m_welcomeText.rect.x = Presenter::m_SCREEN_WIDTH / 2 - m_welcomeText.rect.w / 2;
}

void TitleScreen::run()
{
	drawObject(m_background);
	drawObject(m_welcomeText);
	m_profileButton.draw();

	if (m_profileButton.isClicked())
	{
		destroy();
		world.m_stateManager.changeState(GAME_STATE::PROFILE_SCREEN);
		return;
	}

	if (InputManager::isMouseClicked() || InputManager::isKeyClicked(SDL_SCANCODE_RETURN))
	{
		destroy();
		world.m_stateManager.changeState(GAME_STATE::GAME);
		return;
	}
}

void TitleScreen::destroy()
{
	SDL_DestroyTexture(m_background.texture);
	SDL_DestroyTexture(m_welcomeText.texture);
	m_profileButton.destroy();

	m_background = m_welcomeText = {};
}
