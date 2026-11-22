#pragma once

#include "Writer.h"

class TextField
{
public:
	TextField();
	~TextField();

	void init(string configFile);
	virtual void update();
	void draw();
	void destroy();

	void setText(const string& text, int2 coords = { INT_MIN, INT_MIN }, bool makeCentered = true, int opacity = 255);
	void setText(const string& text, bool makeCentered, int opacity = 255);
	void setText(const string& text, int2 coords, int opacity);
	void setText(const string& text, int opacity);

	bool m_needToDrawBackground;
protected:
	DrawableWithOpacity m_text;

	int m_fontSize;
	int m_color;

	string m_textString;

	DrawableWithOpacity m_background;
};