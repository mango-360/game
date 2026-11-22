#include "LoginScreen.h"
#include "InputManager.h"
#include "Account.h"
#include "World.h"

extern World world;

static const int MAX_TEXT_LENGTH = 20;
static const int PANEL_WIDTH = 760;
static const int PANEL_PADDING = 80;

void LoginScreen::init()
{
	buildPage(false);
}

void LoginScreen::run()
{
	drawMenuPanel(m_panel);
	drawObject(m_title);

	for (int i = 0; i < m_boxCount; i++)
		drawBox(m_boxes[i], i == m_activeBox);

	drawObject(m_status);
	m_mainButton.draw();
	m_switchButton.draw();
	drawObject(m_hint);

	// Choose which box to type in: click on it or press Tab
	SDL_Point mouse = { InputManager::m_mouseCoor.x, InputManager::m_mouseCoor.y };

	if (InputManager::isMouseClicked())
	{
		for (int i = 0; i < m_boxCount; i++)
			if (SDL_PointInRect(&mouse, &m_boxes[i].rect)) m_activeBox = i;
	}

	if (InputManager::isKeyClicked(SDL_SCANCODE_TAB))
		m_activeBox = (m_activeBox + 1) % m_boxCount;

	typeInto(m_boxes[m_activeBox]);

	if (m_mainButton.isClicked() || InputManager::isKeyClicked(SDL_SCANCODE_RETURN))
	{
		submit();
	}
	else if (m_switchButton.isClicked())
	{
		destroyPage();
		buildPage(!m_signUpPage);
	}
}

void LoginScreen::destroy()
{
	destroyPage();
}

// Places everything of the page from top to bottom inside the panel
void LoginScreen::buildPage(bool signUpPage)
{
	m_signUpPage = signUpPage;
	m_boxCount = signUpPage ? 3 : 2;
	m_activeBox = 0;

	int boxHeight = 75;
	int fieldHeight = 150;
	int panelHeight = 50 + 120 + m_boxCount * fieldHeight + 50 + 110 + 70 + 45;

	m_panel = { (Presenter::m_SCREEN_WIDTH - PANEL_WIDTH) / 2, (Presenter::m_SCREEN_HEIGHT - panelHeight) / 2, PANEL_WIDTH, panelHeight };

	int centerX = m_panel.x + m_panel.w / 2;
	int left = m_panel.x + PANEL_PADDING;
	int width = m_panel.w - PANEL_PADDING * 2;
	int y = m_panel.y + 50;

	createText(m_title, signUpPage ? "Sign up" : "Log in", WHITE, 70, 0, y);
	centerText(m_title, centerX);
	y += 120;

	string labels[3] = { "Name", "Password", "Repeat password" };

	for (int i = 0; i < m_boxCount; i++)
	{
		TextBox& box = m_boxes[i];
		box = {};
		box.hidden = (i > 0);
		createText(box.label, labels[i], WHITE, 32, left, y);
		box.rect = { left, y + 45, width, boxHeight };
		updateBoxText(box);
		y += fieldHeight;
	}

	m_status = {};
	createText(m_status, " ", ORANGE, 28, 0, y);
	y += 50;

	m_mainButton.init(signUpPage ? "CREATE ACCOUNT" : "LOG IN", { left, y, width, 85 });
	y += 110;

	m_switchButton.init(signUpPage ? "BACK TO LOG IN" : "No account? SIGN UP", { left, y, width, 70 }, false, 30);

	createText(m_hint, "Tab - next field     Enter - confirm", WHITE, 24, 0, m_panel.y + m_panel.h + 25);
	centerText(m_hint, centerX);
}

void LoginScreen::destroyPage()
{
	SDL_DestroyTexture(m_title.texture);
	SDL_DestroyTexture(m_status.texture);
	SDL_DestroyTexture(m_hint.texture);

	for (int i = 0; i < m_boxCount; i++)
	{
		SDL_DestroyTexture(m_boxes[i].label.texture);
		SDL_DestroyTexture(m_boxes[i].text.texture);
		m_boxes[i] = {};
	}

	m_mainButton.destroy();
	m_switchButton.destroy();

	m_title = m_status = m_hint = {};
}

void LoginScreen::submit()
{
	string name = m_boxes[0].value;
	string password = m_boxes[1].value;
	string error;

	if (m_signUpPage)
	{
		if (password != m_boxes[2].value) setStatus("The passwords don't match");
		else if (Account::signUp(name, password, error)) finish();
		else setStatus(error);
	}
	else
	{
		if (Account::logIn(name, password, error)) finish();
		else setStatus(error);
	}
}

// Logged in successfully, go to the title screen
void LoginScreen::finish()
{
	destroyPage();
	world.m_stateManager.changeState(GAME_STATE::TITLE_SCREEN);
}

// Letters (Shift for capital), digits and Backspace
void LoginScreen::typeInto(TextBox& box)
{
	bool shift = InputManager::isKeyPressed(SDL_SCANCODE_LSHIFT) || InputManager::isKeyPressed(SDL_SCANCODE_RSHIFT);
	bool changed = false;

	for (int key = SDL_SCANCODE_A; key <= SDL_SCANCODE_Z; key++)
	{
		if (InputManager::isKeyClicked((SDL_Scancode)key) && box.value.size() < MAX_TEXT_LENGTH)
		{
			box.value += (char)((shift ? 'A' : 'a') + key - SDL_SCANCODE_A);
			changed = true;
		}
	}

	for (int key = SDL_SCANCODE_1; key <= SDL_SCANCODE_0; key++)
	{
		if (InputManager::isKeyClicked((SDL_Scancode)key) && box.value.size() < MAX_TEXT_LENGTH)
		{
			box.value += (key == SDL_SCANCODE_0) ? '0' : (char)('1' + key - SDL_SCANCODE_1);
			changed = true;
		}
	}

	if (InputManager::isKeyClicked(SDL_SCANCODE_BACKSPACE) && !box.value.empty())
	{
		box.value.pop_back();
		changed = true;
	}

	if (changed) updateBoxText(box);
}

void LoginScreen::updateBoxText(TextBox& box)
{
	string shown = box.hidden ? string(box.value.size(), '*') : box.value;

	// A space keeps the texture valid when the box is empty
	createText(box.text, shown + " ", WHITE, 40, box.rect.x + 20, 0);
	box.text.rect.y = box.rect.y + (box.rect.h - box.text.rect.h) / 2;
}

void LoginScreen::drawBox(TextBox& box, bool active)
{
	// The active box has a gold border
	drawFrame(box.rect, MENU_BOX, active ? MENU_ACCENT : MENU_BOX_BORDER, 3);

	drawObject(box.label);
	drawObject(box.text);
}

void LoginScreen::setStatus(string text)
{
	createText(m_status, text, ORANGE, 28, 0, m_status.rect.y);
	centerText(m_status, m_panel.x + m_panel.w / 2);
}
