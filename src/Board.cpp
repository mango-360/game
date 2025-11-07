#include "Board.h"
#include "InputManager.h"
#include "SoundManager.h"
#include "World.h"

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

	m_ground.init();

	initMap();
}

void Board::initMap()
{
	for (int i = 0; i < 24; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			m_map[i][j] = Tile::AIR;
		}
	}

	int sizeMultiplier = 0.2;
	int amplitudeMultiplier = 2;

	for(int x = 0; x < 32; x++)
	{
		amplitudeMultiplier * sizeMultiplier * sin(x * 0.02 * 1 / sizeMultiplier) * 10 +
		amplitudeMultiplier * sizeMultiplier * sin(x * 0.05 * 1 / sizeMultiplier) * 5 +
		amplitudeMultiplier * sizeMultiplier * sin(x * 0.1 * 1 / sizeMultiplier) * 3;
	}
}

void Board::update()
{
	m_player.update();
}

void Board::draw()
{
	drawObject(m_background);
	m_ground.draw();
	drawObject(m_player);

}

void Board::destroy()
{
	SDL_DestroyTexture(m_background);
}