#include "Board.h"
#include "InputManager.h"
#include "SoundManager.h"
#include "World.h"
#include <cmath>
#include <random>

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

	m_camera.init(&m_ground);
}

void Board::initMap()
{
	for (int i = 0; i < 24; i++) for (int j = 0; j < 32; j++) m_map[i][j] = Tile::AIR;

	float sizeMultiplier = 0.2f;
	float amplitudeMultiplier = 2.0f;

	int surfaceLevel = 12;

	// modern RNG, seeded from random_device
	std::mt19937 rng(std::random_device{}());
	std::uniform_int_distribution<int> startDist(0, 120);
	int x = startDist(rng);

	for (int i = 0; i < 32; ++i)
	{
		float noiseF =
			amplitudeMultiplier * sizeMultiplier * std::sin(x * 0.02f / sizeMultiplier) * 10.0f +
			amplitudeMultiplier * sizeMultiplier * std::sin(x * 0.05f / sizeMultiplier) * 5.0f +
			amplitudeMultiplier * sizeMultiplier * std::sin(x * 0.1f / sizeMultiplier) * 3.0f;

		int noise = static_cast<int>(round(noiseF));

		int y = surfaceLevel - noise;
		if (y >= 0 && y < 24) // guard array bounds
			m_map[y][i] = Tile::GRASSBLOCK;

		cout << noise << " ";

		x ++;
	}

	for (int i = 0; i < 24; i++)
	{
		for (int j = 0; j < 32; j++) cout << static_cast<int>(m_map[i][j]) << " ";
		cout << endl;
	}
}

void Board::update()
{
	m_player.update();
	m_camera.update();
}

void Board::draw()
{
	drawObject(m_background);
	//m_ground.draw();
	drawObject(m_player);
	m_camera.draw();
}

void Board::destroy()
{
	SDL_DestroyTexture(m_background);
}