#include "Button.h"
#include "InputManager.h"

void createText(Drawable& drawable, string text, COLOR color, int fontSize, int x, int y)
{
	if (drawable.texture != nullptr) SDL_DestroyTexture(drawable.texture);

	pair<int2, SDL_Texture*> result = Presenter::m_writer->getText(text, color, fontSize);
	drawable.texture = result.second;
	drawable.rect = { x, y, result.first.x, result.first.y };
}

void centerText(Drawable& drawable, int centerX)
{
	drawable.rect.x = centerX - drawable.rect.w / 2;
}

void fillRect(SDL_Rect rect, Color color)
{
	SDL_SetRenderDrawColor(Presenter::m_mainRenderer, color.r, color.g, color.b, 255);
	SDL_RenderFillRect(Presenter::m_mainRenderer, &rect);
	SDL_SetRenderDrawColor(Presenter::m_mainRenderer, 0, 0, 0, 255);
}

void drawFrame(SDL_Rect rect, Color fill, Color border, int borderSize)
{
	fillRect(rect, border);
	fillRect({ rect.x + borderSize, rect.y + borderSize, rect.w - borderSize * 2, rect.h - borderSize * 2 }, fill);
}

void drawMenuPanel(SDL_Rect panel)
{
	fillRect({ 0, 0, Presenter::m_SCREEN_WIDTH, Presenter::m_SCREEN_HEIGHT }, MENU_BACKGROUND);
	drawFrame(panel, MENU_PANEL, MENU_PANEL_BORDER, 4);
}

void Button::init(string text, SDL_Rect rect, bool primary, int fontSize)
{
	m_rect = rect;
	m_primary = primary;

	// Dark text on the gold button, white text on the outlined one
	createText(m_text, text, primary ? BLACK : WHITE, fontSize, 0, 0);
	m_text.rect.x = m_rect.x + (m_rect.w - m_text.rect.w) / 2;
	m_text.rect.y = m_rect.y + (m_rect.h - m_text.rect.h) / 2;
}

void Button::draw()
{
	bool hover = isMouseOver();

	if (m_primary)
	{
		Color fill = hover ? Color{ 255, 210, 90 } : MENU_ACCENT;
		fillRect(m_rect, fill);
	}
	else
	{
		Color fill = hover ? Color{ 55, 60, 100 } : MENU_PANEL;
		drawFrame(m_rect, fill, hover ? MENU_ACCENT : MENU_PANEL_BORDER, 3);
	}

	drawObject(m_text);
}

bool Button::isClicked()
{
	return InputManager::isMouseClicked() && isMouseOver();
}

void Button::destroy()
{
	SDL_DestroyTexture(m_text.texture);
	m_text.texture = nullptr;
}

bool Button::isMouseOver()
{
	SDL_Point mouse = { InputManager::m_mouseCoor.x, InputManager::m_mouseCoor.y };

	return SDL_PointInRect(&mouse, &m_rect);
}
