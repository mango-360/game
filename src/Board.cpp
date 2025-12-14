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
	m_player.draw();
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

	// Defensive: if zoom or TILE_SIZE are invalid for any reason, return full map bounds
	if (tilePx <= 0.0f)
	{
		return { 0, 0, MAP_WIDTH - 1, MAP_HEIGHT - 1 };
	}

	// Camera now provides a pixel rect (top-left in world pixels, width/height = screen px).
	SDL_Rect camRect = m_camera.getCameraRect();

	// Convert camera pixel bounds to tile indices.
	// Use floor for left/top and floor((right-1)/tilePx) for right/bottom so edges map to correct tiles.
	const float camLeftPx = static_cast<float>(camRect.x);
	const float camTopPx = static_cast<float>(camRect.y);
	const float camRightPx = camLeftPx + static_cast<float>(camRect.w);
	const float camBottomPx = camTopPx + static_cast<float>(camRect.h);

	// compute integer tile indices and add padding to avoid popping
	const int padding = 2;
	int startX = static_cast<int>(floor(camLeftPx / tilePx)) - padding;
	int endX = static_cast<int>(floor((camRightPx - 1.0f) / tilePx)) + padding;
	int startY = static_cast<int>(floor(camTopPx / tilePx)) - padding;
	int endY = static_cast<int>(floor((camBottomPx - 1.0f) / tilePx)) + padding;

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
