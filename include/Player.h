#pragma once

#include "Entity.h"
#include "Tile.h"
#include "Drop.h"
#include <functional>
#include <utility>
#include <memory>

class Projectile;

class Player : public Entity
{
public:
	Player();
	~Player();
	void init(Tile(*map)[MAP_WIDTH]);
	void update() override;
	void updatePrePhysics() override;
	void updatePostPhysics() override;
	void setProjectileSpawner(function<void(unique_ptr<Projectile>)> spawner);
	
	void addToInventory(unique_ptr<Drop> drop);
private:
	void shoot();
	void move() override;
	void moveVertical() override;
	void animateJump();
	void animateFall();
	void animateLand();

	void toggleInventory();

	void calculateVelocity() override;
	void countFramesOnGround();

	std::function<void(std::unique_ptr<Projectile>)> m_spawnProjectile;

	bool isInvOpen = false;

	int lastKeyPressed = -1;
	int framesOnGround = 0;
	float moveSpeed;
	float2 inputVelocity = { 0, 0 };

	std::pair<Drop, int> inventory[INVENTORY_SIZE];
};
