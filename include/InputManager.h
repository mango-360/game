#pragma once

#include "defines.h"

class InputManager
{
public:
	InputManager();
	~InputManager();

	static int2 m_mouseCoor;

	static const Uint8* m_keyboardState;

	void handleInput();
	void setMouseMultiply(float2 multiplier);

	static bool isMousePressed();
	static bool isAnyKeyPressed();
	static bool isKeyPressed(SDL_Scancode code);

private:
	SDL_Event m_event;

	float2 m_mouseMultiplier;

	static bool m_mousePressed;
}; 
