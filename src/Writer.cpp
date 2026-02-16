#include "Writer.h"
#include "Presenter.h"

Writer::Writer()
{
}

Writer::~Writer()
{
}

void Writer::init()
{
	m_fontLocation = FONT_FOLDER + "JellyDonuts-Aja7.ttf";

	for (int i = 8; i <= 300; i ++) // Font sizes from 8 to 300
	{
		m_fonts[i] = TTF_OpenFont(m_fontLocation.c_str(), i);;
	}

	SDL_Color _color;
	_color.a = 255;

	//Black color
	_color.r = 0;
	_color.g = 0;
	_color.b = 0;
	m_colors[BLACK] = _color;

	//White color
	_color.r = 255;
	_color.g = 255;
	_color.b = 255;
	m_colors[WHITE] = _color;

	//Red color
	_color.r = 255;
	_color.g = 0;
	_color.b = 0;
	m_colors[RED] = _color;

	//Orange color
	_color.r = 232;
	_color.g = 150;
	_color.b = 17;
	m_colors[ORANGE] = _color;
}

pair<int2, SDL_Texture*> Writer::getText(string text, COLOR color, int fontSize)
{
	TTF_Font* _font = m_fonts[fontSize];

	SDL_Surface* _surface = TTF_RenderText_Solid(_font, text.c_str(), m_colors[color]);
	SDL_Texture* _texture = SDL_CreateTextureFromSurface(Presenter::m_mainRenderer, _surface);

	int2 _size = { _surface->w, _surface->h };

	SDL_FreeSurface(_surface);

	return {_size, _texture};
}
