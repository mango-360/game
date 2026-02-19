#include "Statistics.h"
#include "World.h"
extern World world;


Statistics::Statistics()
{
}

Statistics::~Statistics()
{
}

void Statistics::init()
{
	string tmp, positionInit, keyboardInit;
	SDL_Rect tmpRect1, tmpRect2;

	fstream stream;

	stream.open(CONFIG_FOLDER + STATISTICS_FOLDER + "statistics.txt");

	stream >> positionInit;
	stream >> keyboardInit;

	stream.close();
	m_position.init(STATISTICS_FOLDER + positionInit);

	stream.open(CONFIG_FOLDER + STATISTICS_FOLDER + keyboardInit);

	stream >> tmp >> tmpRect1;
	stream >> tmp >> tmpRect2;

	if(!InputManager::isKeyPressed(SDL_SCANCODE_W)) m_keyboard[0].srcRect = tmpRect1;
	else m_keyboard[0].srcRect = tmpRect2;
	if(!InputManager::isKeyPressed(SDL_SCANCODE_A)) m_keyboard[1].srcRect = tmpRect1;
	else m_keyboard[1].srcRect = tmpRect2;
	if(!InputManager::isKeyPressed(SDL_SCANCODE_S)) m_keyboard[2].srcRect = tmpRect1;
	else m_keyboard[2].srcRect = tmpRect2;
	if(!InputManager::isKeyPressed(SDL_SCANCODE_D)) m_keyboard[3].srcRect = tmpRect1;
	else m_keyboard[3].srcRect = tmpRect2;


	for (int i = 0; i < 4; ++i)
	{
		stream >> m_keyboard[i].img;
		stream >> m_keyboard[i].rect;

		m_keyboard[i].texture = loadTexture(m_keyboard[i].img);
	}

	stream.close();


}

void Statistics::update()
{
	m_position.setText("Player position: " + to_string(world.m_stateManager.m_game->m_board.m_player.getMapRect().x) + ", " + to_string(world.m_stateManager.m_game->m_board.m_player.getMapRect().y));
	m_position.update();

	if (InputManager::isKeyClicked(SDL_SCANCODE_W)) m_keyboard[0].srcRect.x = 32;
	else if (InputManager::isKeyUnclicked(SDL_SCANCODE_W)) m_keyboard[0].srcRect.x = 0;

	if (InputManager::isKeyClicked(SDL_SCANCODE_A)) m_keyboard[1].srcRect.x = 32;
	else if (InputManager::isKeyUnclicked(SDL_SCANCODE_A)) m_keyboard[1].srcRect.x = 0;

	if (InputManager::isKeyClicked(SDL_SCANCODE_S)) m_keyboard[2].srcRect.x = 32;
	else if (InputManager::isKeyUnclicked(SDL_SCANCODE_S)) m_keyboard[2].srcRect.x = 0;

	if (InputManager::isKeyClicked(SDL_SCANCODE_D)) m_keyboard[3].srcRect.x = 32;
	else if (InputManager::isKeyUnclicked(SDL_SCANCODE_D)) m_keyboard[3].srcRect.x = 0;
}

void Statistics::draw()
{
	m_position.draw();
	
	Presenter::drawObject(m_keyboard[0]);
	Presenter::drawObject(m_keyboard[1]);
	Presenter::drawObject(m_keyboard[2]);
	Presenter::drawObject(m_keyboard[3]);
}

void Statistics::destroy()
{
	m_position.destroy();
}