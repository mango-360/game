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

	initMap();

	m_camera.init(&m_player);
}

void Board::initMap()
{
	float sizeMultiplier = 0.2f;
	float amplitudeMultiplier = 2.0f;

	int surfaceLevel = 12;

	// modern RNG, seeded from random_device
	std::mt19937 rng(std::random_device{}());
	std::uniform_int_distribution<int> startDist(0, 120);
	int x = startDist(rng);

	for (int i = 0; i < MAP_WIDTH; ++i)
	{
		float noiseF =
			amplitudeMultiplier * sizeMultiplier * sin(x * 0.02f / sizeMultiplier) * 10.0f +
			amplitudeMultiplier * sizeMultiplier * sin(x * 0.05f / sizeMultiplier) * 5.0f +
			amplitudeMultiplier * sizeMultiplier * sin(x * 0.1f / sizeMultiplier) * 3.0f;

		int noise = static_cast<int>(round(noiseF));

		int y = surfaceLevel - noise;
		if (y >= 0 && y < MAP_HEIGHT) // guard array bounds
		{
			GrassBlock grassBlock;

			grassBlock.init({i, y});

			m_map[y][i] = grassBlock;
		}
		else
		{
			Tile tile;

			tile.init({i, y});

			m_map[y][i] = tile;
		}

		x ++;
	}

	/*for (int i = 0; i < MAP_HEIGHT; i++)   COUT MAP
	{
		for (int j = 0; j < MAP_WIDTH; j++) cout << static_cast<int>(m_map[i][j]) << " ";
		cout << endl;
	}*/
}

void Board::update()
{
	m_player.update();
	m_camera.update();

	updateMap();
}

void Board::draw()
{
	drawObject(m_background);

	m_player.draw( { m_camera.getCameraRect().x, m_camera.getCameraRect().y} ); // draw player based on cammera position

	drawMap();
}

void Board::destroy()
{
	SDL_DestroyTexture(m_background);
}

void Board::updateMap()
{
	if (InputManager::isZoomChanged())
	{
		for (int i = 0; i < MAP_HEIGHT; ++i)
		{
			for (int j = 0; j < MAP_WIDTH; ++j)
			{
				m_map[i][j].update();
			}
		}
	}
}

void Board::drawMap()
{
	SDL_Rect visibleTiles = getStartEndTiles();
	SDL_Rect camRect = m_camera.getCameraRect();

	// Use the proper fields: x/y = startX/startY, w/h = endX/endY
	for (int y = visibleTiles.y; y <= visibleTiles.h; ++y)
	{
		for (int x = visibleTiles.x; x <= visibleTiles.w; ++x)
		{
			m_map[y][x].draw({ camRect.x, camRect.y });
		}
	}
}

SDL_Rect Board::getStartEndTiles()
{
	// Determine visible tile range and update only those tiles
	const int tilePx = TILE_SIZE * InputManager::getZoom();

	// Defensive: if zoom or TILE_SIZE are invalid for any reason, return full map bounds
	if (tilePx <= 0.0f)
	{
		return { 0, 0, MAP_WIDTH - 1, MAP_HEIGHT - 1 };
	}

	// compute integer tile indices and add padding to avoid popping
	const int padding = 1;
	int2 startBlock = { static_cast<int>(floor(m_camera.getCameraRect().x / tilePx)) - padding,
						static_cast<int>(floor(m_camera.getCameraRect().y / tilePx)) - padding };
	int2 endBlock = { static_cast<int>(floor((m_camera.getCameraRect().x + m_camera.getCameraRect().w - 1.0f) / tilePx)) + padding,
					  static_cast<int>(floor((m_camera.getCameraRect().y + m_camera.getCameraRect().h - 1.0f) / tilePx)) + padding };

	// clamp to map bounds
	startBlock = { max(startBlock.x, 0), max(startBlock.y, 0) };
	endBlock = { min(endBlock.x, MAP_WIDTH - 1), min(endBlock.y, MAP_HEIGHT - 1) };

	return { startBlock.x, startBlock.y, endBlock.x, endBlock.y };
}
