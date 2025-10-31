#pragma once

#include "Writer.h"

class TextField
{
public:
	TextField();
	~TextField();

	void init(string configFile);
	void update();
	void draw();
	void destroy();

	void setText(string text);

	bool m_needToDrawBackground;
private:
	Drawable m_text;

	int m_fontSize;
	int m_color;

	string m_textString;

	Drawable m_background;
};