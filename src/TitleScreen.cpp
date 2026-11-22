#include "TitleScreen.h"
#include "World.h"

extern World world;

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

	m_dbButton = { 40, 40, 420, 90 };

	pair<int2, SDL_Texture*> _text = Presenter::m_writer->getText("INSERT TEST DATA", WHITE, 40);
	m_dbButtonText.texture = _text.second;
	m_dbButtonText.rect = { m_dbButton.x + (m_dbButton.w - _text.first.x) / 2,
		m_dbButton.y + (m_dbButton.h - _text.first.y) / 2, _text.first.x, _text.first.y };

	m_dbStatusText.texture = nullptr;
	setStatusText(world.m_database.isConnected() ? "DB connected" : "DB not connected",
		world.m_database.isConnected() ? WHITE : RED);
}

void TitleScreen::run()
{
	drawObject(m_background);

	SDL_SetRenderDrawColor(Presenter::m_mainRenderer, 40, 90, 160, 255);
	SDL_RenderFillRect(Presenter::m_mainRenderer, &m_dbButton);
	SDL_SetRenderDrawColor(Presenter::m_mainRenderer, 0, 0, 0, 255);
	drawObject(m_dbButtonText);
	drawObject(m_dbStatusText);

	if (InputManager::isMousePressed())
	{
		SDL_Point _mouse = { InputManager::m_mouseCoor.x, InputManager::m_mouseCoor.y };

		if (SDL_PointInRect(&_mouse, &m_dbButton))
		{
			bool _ok = world.m_database.execute("INSERT INTO dbo.TestData (PlayerName, Score) VALUES ('TestPlayer', "
				+ to_string(rand() % 1000) + ")");

			if (_ok)
			{
				vector<vector<string>> _rows = world.m_database.select("SELECT COUNT(*) FROM dbo.TestData");
				setStatusText("Inserted test row, total rows: " + _rows[0][0], WHITE);
			}
			else
			{
				setStatusText("Insert failed (see console)", RED);
			}

			return;
		}
	}

	if (InputManager::isMousePressed() || InputManager::isAnyKeyPressed())
	{
		world.m_stateManager.changeState(GAME_STATE::GAME);
		return;
	}
}

void TitleScreen::destroy()
{
	SDL_DestroyTexture(m_background.texture);
	SDL_DestroyTexture(m_dbButtonText.texture);
	SDL_DestroyTexture(m_dbStatusText.texture);
}

void TitleScreen::setStatusText(string text, COLOR color)
{
	if (m_dbStatusText.texture != nullptr) SDL_DestroyTexture(m_dbStatusText.texture);

	pair<int2, SDL_Texture*> _text = Presenter::m_writer->getText(text, color, 30);
	m_dbStatusText.texture = _text.second;
	m_dbStatusText.rect = { m_dbButton.x, m_dbButton.y + m_dbButton.h + 10, _text.first.x, _text.first.y };
}