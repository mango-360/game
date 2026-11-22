#include "TitleScreen.h"
#include "World.h"

extern World world;

static const int MAX_NAME_LENGTH = 20;

TitleScreen::TitleScreen()
{
}

TitleScreen::~TitleScreen()
{
}

void TitleScreen::init()
{
	m_background.rect = { 0, 0, Presenter::m_SCREEN_WIDTH, Presenter::m_SCREEN_HEIGHT };
	m_background.texture = loadTexture("TitleScreen.bmp");

	m_playerName = "";
	m_nameText.texture = nullptr;
	updateNameText();
}

void TitleScreen::run()
{
	drawObject(m_background);
	drawObject(m_nameText);

	typeName();

	if (InputManager::isKeyClicked(SDL_SCANCODE_RETURN) && !m_playerName.empty())
	{
		logIn();
		world.m_stateManager.changeState(GAME_STATE::GAME);
		return;
	}
}

void TitleScreen::destroy()
{
	SDL_DestroyTexture(m_background.texture);
	SDL_DestroyTexture(m_nameText.texture);
}

// Letters, digits and Backspace change the name
void TitleScreen::typeName()
{
	bool changed = false;

	for (int key = SDL_SCANCODE_A; key <= SDL_SCANCODE_Z; key++)
	{
		if (InputManager::isKeyClicked((SDL_Scancode)key) && m_playerName.size() < MAX_NAME_LENGTH)
		{
			m_playerName += (char)('A' + key - SDL_SCANCODE_A);
			changed = true;
		}
	}

	for (int key = SDL_SCANCODE_1; key <= SDL_SCANCODE_0; key++)
	{
		if (InputManager::isKeyClicked((SDL_Scancode)key) && m_playerName.size() < MAX_NAME_LENGTH)
		{
			m_playerName += (key == SDL_SCANCODE_0) ? '0' : (char)('1' + key - SDL_SCANCODE_1);
			changed = true;
		}
	}

	if (InputManager::isKeyClicked(SDL_SCANCODE_BACKSPACE) && !m_playerName.empty())
	{
		m_playerName.pop_back();
		changed = true;
	}

	if (changed) updateNameText();
}

void TitleScreen::updateNameText()
{
	if (m_nameText.texture != nullptr) SDL_DestroyTexture(m_nameText.texture);

	pair<int2, SDL_Texture*> text = Presenter::m_writer->getText("Name: " + m_playerName + "_   (Enter to play)", WHITE, 40);
	m_nameText.texture = text.second;
	m_nameText.rect = { 40, 40, text.first.x, text.first.y };
}

// Finds the player's profile by name, or creates it if it doesn't exist yet
void TitleScreen::logIn()
{
	// The name has only letters and digits, so it is safe to put it straight in the SQL
	string name = "'" + m_playerName + "'";

	world.m_database.execute(
		"IF NOT EXISTS (SELECT 1 FROM dbo.Players WHERE Name = " + name + ") "
		"	INSERT INTO dbo.Players (Name) VALUES (" + name + ")");

	vector<vector<string>> rows = world.m_database.select("SELECT Id FROM dbo.Players WHERE Name = " + name);

	if (!rows.empty())
	{
		world.m_playerId = stoi(rows[0][0]);
		cout << "Logged in as " << m_playerName << " (Id " << world.m_playerId << ")" << endl;
	}
	else
	{
		world.m_playerId = -1;
		cout << "Playing without a profile, nothing will be saved" << endl;
	}
}
