#include "World.h"

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
	m_stateManager.init(GAME_STATE::TITLE_SCREEN);
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
}

bool World::isRunning()
{
	return m_running;
}