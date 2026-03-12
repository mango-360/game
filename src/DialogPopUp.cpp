
#include "DialogPopUp.h"
#include "Presenter.h"
#include "InputManager.h"

DialogPopUp::DialogPopUp()
{
}

DialogPopUp::~DialogPopUp()
{
}

void DialogPopUp::init(string configFile, SDL_FRect* rect, Player* player)
{
	string tmp, background, dialogFolder = "dialog\\";

	fstream stream;

	stream.open(CONFIG_FOLDER + dialogFolder + configFile);

	stream >> tmp >> m_fontSize;
	stream >> tmp >> m_background.rect; // conv rect 
	stream >> tmp >> m_color;
	stream >> tmp >> background;
	stream >> tmp >> triggerRect.w >> triggerRect.h;

	stream >> tmp;

	string line;
	while (getline(stream, line))  // reads one line at a time until EOF
	{
		dialog.push_back(line);
	}

	stream.close();

	m_background.texture = loadTexture(background);
	m_needToDrawBackground = true;

	dialogLine = 0;
	isTriggered = false;

	objectRect = rect;
	m_player = player;

	updateTriggerRect();
}

void DialogPopUp::update()
{
	triggerDialog();

	if (isTriggered)
	{
		updateTriggerRect();

		getNextLine();

		TextField::update();
	}

}

void DialogPopUp::draw()
{
	if (isTriggered) TextField::draw();
}

void DialogPopUp::updateTriggerRect()
{
	triggerRect.x = objectRect->x + (objectRect->w / 2) - (triggerRect.w / 2);
	triggerRect.y = objectRect->y + (objectRect->h / 2) - (triggerRect.h / 2);
}

void DialogPopUp::getNextLine()
{
	if(InputManager::isMousePressed())
	{
		if (dialogLine < dialog.size() - 1)
		{
			dialogLine++;
			setText(dialog[dialogLine]);
		}
		else
		{
			setText("");
			dialogLine = 0;

			isTriggered = false;
		}
	}
}

void DialogPopUp::triggerDialog()
{
	if (!isTriggered && FcollRectRect(triggerRect, m_player->getMapRect())) isTriggered = true;
	else if (isTriggered && !FcollRectRect(triggerRect, m_player->getMapRect())) isTriggered = false;	
}
