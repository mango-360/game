#include "Board.h"
#include "InputManager.h"
#include "SoundManager.h"
#include "World.h"
#include "Player.h"

extern World world;

Board::Board()
{
}

Board::~Board()
{
}

void Board::init()
{
	string configFile = "board.txt";
	
	string tmp, backgroundImg;

	fstream stream;

	stream.open(CONFIG_FOLDER + configFile);

	stream >> tmp >> backgroundImg;

	stream.close();

	m_background = loadTexture(backgroundImg);

	m_player.init();
}

void Board::update()
{
	m_player.update();
}

void Board::draw()
{
	drawObject(m_background);
	drawObject(m_player);

}

void Board::destroy()
{
	SDL_DestroyTexture(m_background);
}