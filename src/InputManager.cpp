#include "InputManager.h"

bool InputManager::m_mousePressed = false;
int2 InputManager::m_mouseCoor = {0, 0};
const Uint8* InputManager::m_keyboardState = nullptr;
float InputManager::m_zoom = 1;

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}

void InputManager::handleInput()
{
	m_mousePressed = false;

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
			}
			else if(m_event.wheel.y < 0) // Toward me
			{
				m_zoom -= zoomStep; // Zoom out
				if (m_zoom < minZoom) m_zoom = minZoom;
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
	//cout << "Button code: " << code;
	return m_keyboardState[code];
}
