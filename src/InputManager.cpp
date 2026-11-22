#include "InputManager.h"

bool InputManager::m_mouseClicked = false;
bool InputManager::m_mouseReleased = false;
bool InputManager::m_mouseDown = false;
bool InputManager::m_mouseRightClicked = false;
bool InputManager::hasZoomChanged = false;
int2 InputManager::m_mouseCoor = {0, 0};
const Uint8* InputManager::m_keyboardState = nullptr;
float InputManager::m_zoom = 3;

const Uint8* InputManager::m_prevKeyboardState = nullptr;  
Uint8 InputManager::m_keyboardStateCopy[SDL_NUM_SCANCODES] = { 0 }; 

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}

void InputManager::handleInput()
{
	//copy the current keyboard state to the previous keyboard state before polling new events
	if (m_keyboardState != nullptr)
	{
		memcpy(m_keyboardStateCopy, m_keyboardState, SDL_NUM_SCANCODES);
		m_prevKeyboardState = m_keyboardStateCopy;
	}

	m_mouseClicked = false;
	m_mouseReleased = false;
	m_mouseRightClicked = false;
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
			{
				m_mouseClicked = true;
				m_mouseDown = true;
			}

			if (m_event.button.button == SDL_BUTTON_RIGHT)
				m_mouseRightClicked = true;

			break;
		case SDL_MOUSEBUTTONUP:
			if (m_event.button.button == SDL_BUTTON_LEFT)
			{
				m_mouseReleased = true;
				m_mouseDown = false;
			}
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

bool InputManager::isMouseClicked()
{
	return m_mouseClicked;
}

bool InputManager::isMouseReleased()
{
	return m_mouseReleased;
}

bool InputManager::isMouseDown()
{
	return m_mouseDown;
}

bool InputManager::isMouseRightClicked()
{
	return m_mouseRightClicked;
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

bool InputManager::isKeyClicked(SDL_Scancode code)
{
	return m_keyboardState[code] && !m_prevKeyboardState[code];
}

bool InputManager::isKeyUnclicked(SDL_Scancode code)
{
	return !m_keyboardState[code] && m_prevKeyboardState[code];
}

bool InputManager::changeZoom(float newZoom)
{
	if (newZoom != m_zoom)
	{
		if (abs(newZoom - m_zoom) < 5 * zoomStep)
		{
			m_zoom = newZoom;
			hasZoomChanged = true;
			return true;
		}

		m_zoom += 5 * zoomStep * (newZoom > m_zoom ? 1 : -1);
		hasZoomChanged = true;
		return true;

	}
	return false;
}
