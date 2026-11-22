#pragma once

#include "defines.h"
#include "State.h"
#include "Button.h"

// Separate page with the logged in player's info, skin choice and log out
class ProfileScreen : public State
{
public:
	void init();
	void run();
	void destroy();

private:
	// One line of info: small label and a bigger value under it
	struct InfoRow
	{
		Drawable label;
		Drawable value;
	};

	void createRow(InfoRow& row, string label, string value, int x, int y);
	void updateSkin();

	SDL_Rect m_panel;
	Drawable m_title = {};
	InfoRow m_rows[3] = {}; // name, member since, items collected

	// The player's texture, tinted with the chosen skin
	SDL_Texture* m_skinPreview = nullptr;
	SDL_Rect m_skinPreviewFrame;
	Drawable m_skinName = {};

	Button m_prevSkinButton;
	Button m_nextSkinButton;
	Button m_backButton;
	Button m_logOutButton;
};
