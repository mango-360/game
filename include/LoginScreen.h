#pragma once

#include "defines.h"
#include "State.h"
#include "Button.h"

// Two separate pages: "Log in" and "Sign up"
class LoginScreen : public State
{
public:
	void init();
	void run();
	void destroy();

private:
	// Text box for typing (name or password)
	struct TextBox
	{
		SDL_Rect rect;
		string value;
		bool hidden; // show * instead of the letters
		Drawable label;
		Drawable text;
	};

	void buildPage(bool signUpPage);
	void destroyPage();
	void submit();
	void finish();

	void typeInto(TextBox& box);
	void updateBoxText(TextBox& box);
	void drawBox(TextBox& box, bool active);
	void setStatus(string text);

	bool m_signUpPage = false;

	SDL_Rect m_panel;
	Drawable m_title = {};
	Drawable m_status = {};
	Drawable m_hint = {};

	// Log in: name, password. Sign up: name, password, repeat password
	TextBox m_boxes[3] = {};
	int m_boxCount = 0;
	int m_activeBox = 0;

	Button m_mainButton;   // LOG IN / CREATE ACCOUNT
	Button m_switchButton; // goes to the other page
};
