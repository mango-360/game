#include "InputManager.h"

bool InputManager::m_mousePressed = false;
bool InputManager::hasZoomChanged = false;
int2 InputManager::m_mouseCoor = {0, 0};
const Uint8* InputManager::m_keyboardState = nullptr;
float InputManager::m_zoom = 3;

const Uint8* InputManager::m_prevKeyboardState = nullptr;  // ADD THIS
Uint8 InputManager::m_keyboardStateCopy[SDL_NUM_SCANCODES] = { 0 };  // ADD THIS

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}

void InputManager::handleInput()
{
	// NEW CODE: copy the current keyboard state to the previous keyboard state before polling new events
	if (m_keyboardState != nullptr)
	{
		memcpy(m_keyboardStateCopy, m_keyboardState, SDL_NUM_SCANCODES);
		m_prevKeyboardState = m_keyboardStateCopy;
	}

	m_mousePressed = false;
	hasZoomChanged = false;

	while (SDL_PollEvent(&m_event))
	{
		switch (m_event.type)
		{
		case SDL_MOUSEMOTION:
			SDL_GetMouseState(&m_mouseCoor.x, &m_mouseCoor.y);

			m_mouseCoor *= m_mouseMultiplier;
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (m_event.button.button == SDL_BUTTON_LEFT)
				m_mousePressed = true;
			break;
		case SDL_MOUSEWHEEL:
			if (m_event.wheel.y > 0) // Toward Screen
			{
				m_zoom += zoomStep; // Zoom in
				if (m_zoom > maxZoom) m_zoom = maxZoom;
				else hasZoomChanged = true;
			}
			else if(m_event.wheel.y < 0) // Toward me
			{
				m_zoom -= zoomStep; // Zoom out
				if (m_zoom < minZoom) m_zoom = minZoom;
				else hasZoomChanged = true;
			}
			break;
		default:
			break;
		}
	}

	m_keyboardState = SDL_GetKeyboardState(NULL);
}

void InputManager::setMouseMultiply(float2 multiplier)
{
	m_mouseMultiplier = multiplier;
}

bool InputManager::isMousePressed()
{
	return m_mousePressed;
}

bool InputManager::isAnyKeyPressed()
{
	int numKeys = 322;

	for(int i = 0; i < numKeys; i++)
	{
		if(m_keyboardState[i])
			return true;
	}

	return false;
}

bool InputManager::isKeyPressed(SDL_Scancode code)
{
	return m_keyboardState[code];
}

bool InputManager::isKeyJustPressed(SDL_Scancode code)
{
	return m_keyboardState[code] && !m_prevKeyboardState[code];
}
