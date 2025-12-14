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
	static float getZoom() { return m_zoom; };

private:
	SDL_Event m_event;

	float2 m_mouseMultiplier;

	static bool m_mousePressed;

	const float zoomStep = 0.125f;
	const float minZoom = 1.0f;
	const float maxZoom = 5.0f;
	
	static float m_zoom;
}; 
