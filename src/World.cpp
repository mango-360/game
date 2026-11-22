#include "World.h"
#include "Account.h"

World::World()
{
	m_running = true;
}

World::~World()
{
	m_running = false;
}

void World::init()
{
	m_presenter.init();
	m_database.connect("database.txt");
	m_database.executeScript("create_test_db.sql"); // creates any missing tables
	// Skip the log in screen if the player logged in last time
	m_stateManager.init(Account::tryAutoLogIn() ? GAME_STATE::TITLE_SCREEN : GAME_STATE::LOGIN_SCREEN);
	m_soundManager.init();
	m_imgManager.loadAllTextures();
}

void World::run()
{
	m_inputManager.handleInput();

	if (InputManager::isKeyPressed(SDL_SCANCODE_ESCAPE)) m_running = false;
	m_stateManager.run();

	m_presenter.draw();
}

void World::destroy()
{
	SDL_DestroyRenderer(Presenter::m_mainRenderer);
	SDL_DestroyWindow(Presenter::m_mainWindow);
	m_soundManager.destroy();
	m_imgManager.destroyAllTextures();
	m_database.disconnect();
}

bool World::isRunning()
{
	return m_running;
}