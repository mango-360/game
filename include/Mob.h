#pragma once

#include "defines.h"
#include "Player.h"

class Mob : public Entity
{
public:
	Mob();
	~Mob();
	void setPlayer(Player* player) { m_player = player; }
	void update() override;

private:
	void move() override;
	void moveVertical() override;

	static Player* m_player;
};