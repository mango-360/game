#pragma once

#include "defines.h"

// Skins are colors that tint the player's texture
struct Skin
{
	string name;
	Uint8 r, g, b;
};

static const Skin SKINS[] =
{
	{ "Default", 255, 255, 255 },
	{ "Red",     255, 110, 110 },
	{ "Green",   120, 255, 120 },
	{ "Blue",    120, 160, 255 },
	{ "Gold",    255, 210,  80 },
};
static const int SKIN_COUNT = sizeof(SKINS) / sizeof(SKINS[0]);

// Log in, sign up and log out. The logged in player is stored in world (m_playerId, m_playerName, m_playerSkin)
namespace Account
{
	// On error returns false and puts the reason in error
	bool signUp(string name, string password, string& error);
	bool logIn(string name, string password, string& error);

	// Logs in with the token saved in config\session.txt from the last log in
	bool tryAutoLogIn();
	void logOut();

	void setSkin(int skin);
}
