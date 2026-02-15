#include "Statistics.h"
#include "World.h"
extern World world;


Statistics::Statistics()
{
}

Statistics::~Statistics()
{
}

void Statistics::init()
{
	string positionInit;

	fstream stream;

	stream.open(CONFIG_FOLDER + STATISTICS_FOLDER + "statistics.txt");

	stream >> positionInit;

	stream.close();
	m_position.init(STATISTICS_FOLDER + positionInit);

}

void Statistics::update()
{
	m_position.setText("Player position: " + to_string(world.m_stateManager.m_game->m_board.m_player.getMapRect().x) + ", " + to_string(world.m_stateManager.m_game->m_board.m_player.getMapRect().y));
	m_position.update();
}

void Statistics::draw()
{
	m_position.draw();
}

void Statistics::destroy()
{
	m_position.destroy();
}