#include "ProfileScreen.h"
#include "InputManager.h"
#include "Account.h"
#include "World.h"

extern World world;

void ProfileScreen::init()
{
	int panelWidth = 1040, panelHeight = 700;
	m_panel = { (Presenter::m_SCREEN_WIDTH - panelWidth) / 2, (Presenter::m_SCREEN_HEIGHT - panelHeight) / 2, panelWidth, panelHeight };

	int left = m_panel.x + 80;
	int top = m_panel.y + 50;

	createText(m_title, "Profile", WHITE, 70, 0, top);
	centerText(m_title, m_panel.x + m_panel.w / 2);

	// Left side: skin preview with < name > under it
	m_skinPreviewFrame = { left, top + 130, 300, 260 };
	m_skinPreview = loadTexture("bob.bmp");

	m_prevSkinButton.init("<", { left, top + 410, 56, 56 }, false, 32);
	m_nextSkinButton.init(">", { left + 300 - 56, top + 410, 56, 56 }, false, 32);
	updateSkin();

	// Right side: info from the database
	string id = to_string(world.m_playerId);

	vector<vector<string>> created = world.m_database.select(
		"SELECT CONVERT(VARCHAR(10), CreatedAt, 104) FROM dbo.Players WHERE Id = " + id); // 104 = dd.mm.yyyy
	vector<vector<string>> items = world.m_database.select(
		"SELECT ISNULL(SUM(Quantity), 0) FROM dbo.Inventory WHERE PlayerId = " + id);

	int infoX = left + 380;
	createRow(m_rows[0], "Name", world.m_playerName, infoX, top + 130);
	createRow(m_rows[1], "Member since", created.empty() ? "?" : created[0][0], infoX, top + 260);
	createRow(m_rows[2], "Items collected", items.empty() ? "0" : items[0][0], infoX, top + 390);

	// Bottom: buttons
	int buttonWidth = (m_panel.w - 160 - 30) / 2;
	int buttonY = m_panel.y + m_panel.h - 130;
	m_backButton.init("BACK", { left, buttonY, buttonWidth, 80 });
	m_logOutButton.init("LOG OUT", { left + buttonWidth + 30, buttonY, buttonWidth, 80 }, false);
}

void ProfileScreen::run()
{
	drawMenuPanel(m_panel);
	drawObject(m_title);

	drawFrame(m_skinPreviewFrame, MENU_BOX, MENU_BOX_BORDER, 3);
	SDL_Rect firstFrame = { 0, 0, 32, 32 }; // the player image is a sprite sheet
	SDL_Rect preview = { m_skinPreviewFrame.x + (m_skinPreviewFrame.w - 192) / 2, m_skinPreviewFrame.y + 34, 192, 192 };
	SDL_RenderCopy(Presenter::m_mainRenderer, m_skinPreview, &firstFrame, &preview);

	m_prevSkinButton.draw();
	m_nextSkinButton.draw();
	drawObject(m_skinName);

	for (InfoRow& row : m_rows)
	{
		drawObject(row.label);
		drawObject(row.value);
	}

	m_backButton.draw();
	m_logOutButton.draw();

	if (m_prevSkinButton.isClicked())
	{
		Account::setSkin((world.m_playerSkin + SKIN_COUNT - 1) % SKIN_COUNT);
		updateSkin();
	}

	if (m_nextSkinButton.isClicked())
	{
		Account::setSkin((world.m_playerSkin + 1) % SKIN_COUNT);
		updateSkin();
	}

	if (m_backButton.isClicked())
	{
		destroy();
		world.m_stateManager.changeState(GAME_STATE::TITLE_SCREEN);
		return;
	}

	if (m_logOutButton.isClicked())
	{
		Account::logOut();
		destroy();
		world.m_stateManager.changeState(GAME_STATE::LOGIN_SCREEN);
		return;
	}
}

void ProfileScreen::destroy()
{
	SDL_DestroyTexture(m_title.texture);
	SDL_DestroyTexture(m_skinName.texture);
	SDL_DestroyTexture(m_skinPreview);

	for (InfoRow& row : m_rows)
	{
		SDL_DestroyTexture(row.label.texture);
		SDL_DestroyTexture(row.value.texture);
		row = {};
	}

	m_prevSkinButton.destroy();
	m_nextSkinButton.destroy();
	m_backButton.destroy();
	m_logOutButton.destroy();

	m_title = m_skinName = {};
	m_skinPreview = nullptr;
}

void ProfileScreen::createRow(InfoRow& row, string label, string value, int x, int y)
{
	createText(row.label, label, WHITE, 28, x, y);
	createText(row.value, value, ORANGE, 48, x, y + 40);
}

void ProfileScreen::updateSkin()
{
	const Skin& skin = SKINS[world.m_playerSkin];

	// Skin name centered between the < and > buttons
	createText(m_skinName, skin.name, WHITE, 30, 0, 0);
	centerText(m_skinName, m_skinPreviewFrame.x + m_skinPreviewFrame.w / 2);
	m_skinName.rect.y = m_prevSkinButton.m_rect.y + (m_prevSkinButton.m_rect.h - m_skinName.rect.h) / 2;

	SDL_SetTextureColorMod(m_skinPreview, skin.r, skin.g, skin.b);
}
