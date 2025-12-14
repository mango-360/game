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

			grassBlock.setPosition({ i * TILE_SIZE, y * TILE_SIZE });

			m_map[y][i] = grassBlock;
		}
		else
		{
			Tile tile;

			tile.init({i, y});

			tile.setPosition({ i * TILE_SIZE, y * TILE_SIZE });

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
	drawObject(m_player);
	m_camera.draw();
	drawMap();
}

void Board::destroy()
{
	SDL_DestroyTexture(m_background);
}

void Board::updateMap()
{
	SDL_Rect visibleTiles = getStartEndTiles();

	// Use the proper fields: x/y = startX/startY, w/h = endX/endY
	for (int y = visibleTiles.y; y <= visibleTiles.h; ++y)
	{
		for (int x = visibleTiles.x; x <= visibleTiles.w; ++x)
		{
			m_map[y][x].update();
		}
	}
}

void Board::drawMap()
{
	SDL_Rect visibleTiles = getStartEndTiles();

	// Use the proper fields: x/y = startX/startY, w/h = endX/endY
	for (int y = visibleTiles.y; y <= visibleTiles.h; ++y)
	{
		for (int x = visibleTiles.x; x <= visibleTiles.w; ++x)
		{
			m_map[y][x].draw();
		}
	}
}

SDL_Rect Board::getStartEndTiles()
{
	// Determine visible tile range and update only those tiles
	const float zoom = InputManager::getZoom();
	const float tilePx = TILE_SIZE * zoom;
	cout << "Zoom: " << zoom << ", TilePx: " << tilePx << endl;

	// Defensive: if zoom or TILE_SIZE are invalid for any reason, return full map bounds
	if (tilePx <= 0.0f)
	{
		return { 0, 0, MAP_WIDTH - 1, MAP_HEIGHT - 1 };
	}

	float2 camPos = m_camera.getPosition(); // camera position is in tile units (center)

	// how many tiles fit half-screen in each direction (from center)
	const float halfTilesX = (float)Presenter::m_SCREEN_WIDTH / (2.0f * tilePx);
	const float halfTilesY = (float)Presenter::m_SCREEN_HEIGHT / (2.0f * tilePx);

	// compute integer tile indices and add padding to avoid popping
	const int padding = 2;
	int startX = static_cast<int>(floor(camPos.x - halfTilesX)) - padding;
	int endX = static_cast<int>(floor(camPos.x + halfTilesX)) + padding;
	int startY = static_cast<int>(floor(camPos.y - halfTilesY)) - padding;
	int endY = static_cast<int>(floor(camPos.y + halfTilesY)) + padding;

	// clamp to map bounds
	startX = max(startX, 0);
	startY = max(startY, 0);
	endX = min(endX, MAP_WIDTH - 1);
	endY = min(endY, MAP_HEIGHT - 1);

	// defensive: if rounding/positioning made start > end, fix by clamping to full bounds
	if (startX > endX) { startX = 0; endX = MAP_WIDTH - 1; }
	if (startY > endY) { startY = 0; endY = MAP_HEIGHT - 1; }

	// return in SDL_Rect: x=startX, y=startY, w=endX, h=endY
	return { startX, startY, endX, endY };
}
