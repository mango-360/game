#pragma once

#include "defines.h"
#include <map>

class Writer
{
public:
	Writer();
	~Writer();

	void init();

	pair<int2, SDL_Texture*> getText(string text, COLOR color, int fontSize);

private:
	map<int, TTF_Font*> m_fonts; // int - font size
	map<COLOR, SDL_Color> m_colors;

	string m_fontLocation;
};