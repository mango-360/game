#pragma once

#include "defines.h"
#include "Presenter.h"

// Small helpers and a shared look for the menu screens (log in, sign up, title, profile)

struct Color { Uint8 r, g, b; };

static const Color MENU_BACKGROUND = { 18, 20, 38 };
static const Color MENU_PANEL = { 34, 37, 66 };
static const Color MENU_PANEL_BORDER = { 80, 88, 150 };
static const Color MENU_BOX = { 22, 24, 44 };
static const Color MENU_BOX_BORDER = { 70, 76, 120 };
static const Color MENU_ACCENT = { 245, 185, 40 }; // gold, like the logo

// Creates the text texture (and deletes the old one) and places it at x, y
void createText(Drawable& drawable, string text, COLOR color, int fontSize, int x, int y);

// Moves the text so it is centered horizontally around centerX
void centerText(Drawable& drawable, int centerX);

void fillRect(SDL_Rect rect, Color color);

// Filled rectangle with a border around it
void drawFrame(SDL_Rect rect, Color fill, Color border, int borderSize);

// Clears the screen with the menu background and draws a centered panel
void drawMenuPanel(SDL_Rect panel);

class Button
{
public:
	// primary = gold button for the main action, otherwise an outlined button
	void init(string text, SDL_Rect rect, bool primary = true, int fontSize = 40);
	void draw();
	bool isClicked();
	void destroy();

	SDL_Rect m_rect;

private:
	bool isMouseOver();

	bool m_primary = true;
	Drawable m_text = {};
};
