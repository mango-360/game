#include "TextField.h"
#include "Presenter.h"
#include "InputManager.h"

TextField::TextField()
{
}

TextField::~TextField()
{
}

void TextField::init(string configFile)
{
	string tmp, background;

	fstream stream;

	stream.open(CONFIG_FOLDER + configFile);

	stream >> tmp >> m_fontSize;
	stream >> tmp >> m_background.rect;
	stream >> tmp >> m_color;
	stream >> tmp >> background;

	stream.close();

	m_background.texture = loadTexture(background);

	m_needToDrawBackground = true;
}

void TextField::update()
{
	if (m_textString.length() > 0)
	{
		auto text = Presenter::m_writer->getText(m_textString, (COLOR)m_color, m_fontSize);

		if(m_text.texture != nullptr)
		{
			SDL_DestroyTexture(m_text.texture);
		}

		m_text.texture = text.second;
		m_text.rect.w = text.first.x;
		m_text.rect.h = text.first.y;
	}

	if (InputManager::isZoomChanged()) setText(m_textString);
}

void TextField::draw()
{
	if(m_needToDrawBackground)
		drawObject(m_background);
	
	if (m_textString.length() > 0)
		drawObject(m_text);
}

void TextField::destroy()
{
	SDL_DestroyTexture(m_text.texture);
	SDL_DestroyTexture(m_background.texture);
	m_textString = "";
}

void TextField::setText(const string& text, int2 coords, bool makeCentered, int opacity)
{
	if (m_textString != text || InputManager::isZoomChanged() || coords != int2{ INT_MIN , INT_MIN })
	{
		m_textString = text;
		m_text.opacity = opacity;

		auto result = Presenter::m_writer->getText(m_textString, (COLOR)m_color, m_fontSize);

		if (coords == int2{ INT_MIN , INT_MIN })
		{
			if (makeCentered)
			{
				m_text.rect.x = m_background.rect.x + (m_background.rect.w - result.first.x) / 2;
				m_text.rect.y = m_background.rect.y + (m_background.rect.h - result.first.y) / 2;
			}
			else
			{
				m_text.rect.x = m_background.rect.x;
				m_text.rect.y = m_background.rect.y;
			}
		}
		else
		{
			m_text.rect.x = coords.x;
			m_text.rect.y = coords.y;
		}
		
		m_text.rect.w = result.first.x;
		m_text.rect.h = result.first.y;

		if (m_text.texture != nullptr)
		{
			SDL_DestroyTexture(m_text.texture);
		}

		m_text.texture = result.second;
	}
}

void TextField::setText(const string& text, bool makeCentered, int opacity)
{
	setText(text, { INT_MIN, INT_MIN }, makeCentered, opacity);
}

void TextField::setText(const string& text, int2 coords, int opacity)
{
	setText(text, coords, true, opacity);
}

void TextField::setText(const string& text, int opacity)
{
	setText(text, { INT_MIN, INT_MIN }, true, opacity);
}
