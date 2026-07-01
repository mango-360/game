#include "Board.h"
#include "InputManager.h"
#include "SoundManager.h"
#include "Drop.h"
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

	m_player.init(m_map);

	m_mob.init(m_map, "player.txt");
	m_mob.setPlayer(&m_player);

	m_entities.push_back(&m_mob);
	m_entities.push_back(&m_player);

  // register spawner after player init: set a projectile spawner that also
	// assigns a drop-spawner on the projectile so projectiles can hand drops
	// back to the Board when a tile breaks.
	m_player.setProjectileSpawner([this](unique_ptr<Projectile> p) {

		// set how this projectile should hand spawned drops to the board
		p->setDropSpawner([this](unique_ptr<Drop> d) {
			m_drops.push_back(std::move(d));
		});

		m_projectiles.push_back(std::move(p));
	});

	initMap();

	m_camera.init(&m_player);

	m_statistics.init();

	m_dialog.init("dialog.txt", m_mob.getMapRectPtr(), &m_player);
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

	for(int j = 0; j < MAP_HEIGHT; ++j)
	{
		for(int i = 0; i < MAP_WIDTH; ++i)
		{
			Tile tile;
			tile.init({ static_cast<float>(i),static_cast<float>(j) });
			m_map[j][i] = tile;
		}
	}
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

			grassBlock.init({ static_cast<float>(i),static_cast<float>(y) });

			m_map[y][i] = grassBlock;

			// bush generation

			uniform_int_distribution<int> bushChance(1, 10);

			if(bushChance(rng) == 1)
			{
				Bush bush;
				bush.init({ static_cast<float>(i),static_cast<float>(y - 1) });
				m_map[y - 1][i] = bush; // place bush on top of grass block
			}

			for (++y; y < MAP_HEIGHT; ++y) // fill downwards with grass blocks
			{
				DirtBlock dirtBlock;

				dirtBlock.init({ static_cast<float>(i),static_cast<float>(y) });

				m_map[y][i] = dirtBlock;
			}
		}

		x ++;
	}
}

void Board::update()
{
	for (Entity* entity : m_entities)
	{
		entity->updatePrePhysics();
	}

	handleCollisions();

	for (auto& projectile : m_projectiles) projectile->update();

	for (Entity* entity : m_entities)
	{
		entity->updatePostPhysics();
	}

	destroyProjectiles();

	// update drops
	for (auto& drop : m_drops) 
		drop->update();

	playerPickUpDrop();

	m_camera.update();

	updateMap();

	toggleStatistics();
	if(drawStatistics) m_statistics.update();

	m_dialog.update();
	
}

void Board::draw()
{
	drawObject(m_background);

	drawMap();

	for (auto& projectile : m_projectiles) projectile->draw({ m_camera.getCameraRect().x, m_camera.getCameraRect().y });
	for (auto& drop : m_drops) drop->draw({ m_camera.getCameraRect().x, m_camera.getCameraRect().y });
	for (auto& entity : m_entities) entity->draw({ m_camera.getCameraRect().x, m_camera.getCameraRect().y });

	if (drawStatistics) m_statistics.draw();

	m_dialog.draw();
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
	Camera_Rect camRect = m_camera.getCameraRect();

	for (int y = floor(camRect.y); y <= floor(camRect.y + camRect.h); ++y)
	{
		for (int x = floor(camRect.x); x <= floor(camRect.x + camRect.w); ++x)
		{
			if(y < 0 || y >= MAP_HEIGHT || x < 0 || x >= MAP_WIDTH) continue; // skip out-of-bounds tiles

			m_map[y][x].draw({ camRect.x, camRect.y });
		}
	}
}

void Board::toggleStatistics()
{
	if(InputManager::isKeyClicked(SDL_SCANCODE_F3)) drawStatistics = !drawStatistics;
}

void Board::destroyProjectiles()
{
	auto it = m_projectiles.begin();
	while (it != m_projectiles.end())
	{
		if (!(*it)->isAlive) it = m_projectiles.erase(it);
		else ++it;
	}
}

void Board::handleCollisions()
{
	handleEntityTileCollisions();

	handleEntityEntityCollisions();

	handleEntityProjectileCollisions();

	handleProjectileTileCollisions();
}

void Board::handleEntityTileCollisions()
{
	for(Entity* entity : m_entities)
	{
		bool hitsGround = false;
		float2 friction = { 0, 0 };

		float2 cp, cn;
		float t = 0, min_t = INFINITY, prevFriction = 0;
		vector<pair<int2, float>> collsList;

		// Work out collision point, add it to vector along with rect ID
		for (int i = floor(entity->getMapRect().y) - 1; i <= ceil(entity->getMapRect().y + entity->getMapRect().h); ++i)
		{
			for (int j = floor(entity->getMapRect().x) - 1; j <= ceil(entity->getMapRect().x + entity->getMapRect().w); ++j)
			{
				if (i < 0 || i >= MAP_HEIGHT || j < 0 || j >= MAP_WIDTH)
				{
					continue;
				}

				if (!m_map[i][j].getIsSolid())
				{
					continue;
				}
				const SDL_FRect& mapRect = entity->getMapRect(); // bind to local ref

				if (DynamicRectVsRect(&mapRect, entity->getVelocity(), m_map[i][j].getTileGridRect(), cp, cn, t))
				{
					collsList.push_back({ {i, j}, t });

					if (cn.y == -1)
					{
						hitsGround = true;

						if (m_map[i][j].getFriction() > prevFriction) prevFriction = m_map[i][j].getFriction();
					}
				}
			}
		}

		SDL_FRect FutureEntityRect = { entity->getMapRect() };

		for (int x = floor(FutureEntityRect.x); x <= ceil(FutureEntityRect.x + FutureEntityRect.w); ++x) //check nonsolid blocks for friction
		{
			for (int y = floor(FutureEntityRect.y); y <= ceil(FutureEntityRect.y + FutureEntityRect.h); ++y)
			{
				if (y < 0 || y >= MAP_HEIGHT || x < 0 || x >= MAP_WIDTH)
				{
					continue;
				}

				if (!m_map[y][x].getIsSolid() && m_map[y][x].getFriction() > prevFriction && FcollRectRect(FutureEntityRect, m_map[y][x].getTileGridRect()))
				{
					prevFriction = m_map[y][x].getFriction();
				}
			}
		}

		// Do the sort
		sort(collsList.begin(), collsList.end(), [](const pair<int2, float>& a, const pair<int2, float>& b)
			{
				return a.second < b.second;
			});

		// Now resolve the collision in correct order 
		for (auto j : collsList)
		{
			// Avoid taking the address of a temporary returned by getTileGridRect():
			SDL_FRect tileRect = m_map[j.first.x][j.first.y].getTileGridRect();
			entity->resolveCollision(tileRect);
		}


		entity->calculateFriction(prevFriction);
		entity->isOnGround = hitsGround;
		if (entity->isOnGround) entity->isJumping = false;
		else if (!entity->isJumping) entity->landingStartSpriteFrame = entity->NoJumpLandingSpriteFrame;
	}
	
}

void Board::handleEntityEntityCollisions()
{
}

void Board::handleEntityProjectileCollisions()
{
}

void Board::handleProjectileTileCollisions()
{
	for(unique_ptr<Projectile>& projectile : m_projectiles)
	{
		projectile->calculateVelocity();

		projectile->collision();
	}
}

void Board::playerPickUpDrop()
{
	//cout << m_drops.size() << endl;

	for (auto it = m_drops.begin(); it != m_drops.end(); )
	{
		if (FcollRectRect(m_player.getMapRect(), (*it)->getGridRect()))
		{
			//SoundManager::playSound("pickup.wav");
			// transfer ownership of the drop to the player
			m_player.addToInventory(std::move(*it));
			// remove null unique_ptr from board
			it = m_drops.erase(it);
			return;
		}
		else ++it;
	}
}
