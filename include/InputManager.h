#pragma once

#include "defines.h"

class InputManager
{
public:
	InputManager();
	~InputManager();

	static int2 m_mouseCoor;

	static const Uint8* m_keyboardState;
	static const Uint8* m_prevKeyboardState;  


	void handleInput();
	void setMouseMultiply(float2 multiplier);

	static bool isMouseClicked();
	static bool isMouseReleased();
	static bool isAnyKeyPressed();
	static bool isKeyPressed(SDL_Scancode code);
	static bool isKeyClicked(SDL_Scancode code); 
	static bool isKeyUnclicked(SDL_Scancode code); 
	static bool isZoomChanged() { return hasZoomChanged; };
	static float getZoom() { return m_zoom; };
	static bool changeZoom(float newZoom);

private:
	SDL_Event m_event;

	float2 m_mouseMultiplier;

	static bool m_mouseClicked;
	static bool m_mouseReleased;
	static bool hasZoomChanged;

	static float m_zoom;
	static constexpr float zoomStep = 0.125f;
	static constexpr float minZoom = 1.0f;
	static constexpr float maxZoom = 5.0f;
	
	static Uint8 m_keyboardStateCopy[SDL_NUM_SCANCODES];  // ADD THIS
}; 
