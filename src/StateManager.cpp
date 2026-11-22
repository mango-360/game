#include "StateManager.h"

StateManager::StateManager()
{
	m_currentState = GAME_STATE::NONE;
	m_state = nullptr;
}

StateManager::~StateManager()
{
	m_currentState = GAME_STATE::NONE;
	m_state = nullptr;
}

void StateManager::changeState(GAME_STATE newState)
{
	m_currentState = newState;

	initNewState();
}

void StateManager::initNewState()
{
	switch (m_currentState)
	{
	case GAME_STATE::NONE:
		//WORLD
		m_state = nullptr;
		break;
	case GAME_STATE::TITLE_SCREEN:
		m_state = m_titleScreen;
		break;
	case GAME_STATE::WIN_SCREEN:
		m_state = m_WinScreen;
		break;
	case GAME_STATE::GAME:
		m_state = m_game;
		break;
	case GAME_STATE::LOGIN_SCREEN:
		m_state = m_loginScreen;
		break;
	case GAME_STATE::PROFILE_SCREEN:
		m_state = m_profileScreen;
		break;
	default:
		break;
	}
	
	m_state->init();
}

void StateManager::init(GAME_STATE state)
{
	m_titleScreen = new TitleScreen();
	m_game = new Game();
	m_WinScreen = new WinScreen();
	m_loginScreen = new LoginScreen();
	m_profileScreen = new ProfileScreen();

	m_currentState = state;

	initNewState();
}

void StateManager::run()
{
	m_state->run();
}

void StateManager::destroy()
{
	m_state->destroy();
}