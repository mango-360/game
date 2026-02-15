#include "TextField.h"
#include "Presenter.h"

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
	stream >> tmp >> m_background.rect.x >> m_background.rect.y >> m_background.rect.w >> m_background.rect.h;
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

void TextField::setText(string text)
{
	if (m_textString != text)
	{
		m_textString = text;

		auto result = Presenter::m_writer->getText(m_textString, (COLOR)m_color, m_fontSize);

		m_text.rect.x = m_background.rect.x + (m_background.rect.w - result.first.x) / 2;
		m_text.rect.y = m_background.rect.y + (m_background.rect.h - result.first.y) / 2;
		m_text.rect.w = result.first.x;
		m_text.rect.h = result.first.y;

		if (m_text.texture != nullptr)
		{
			SDL_DestroyTexture(m_text.texture);
		}

		m_text.texture = result.second;
	}
}
