#pragma once

#include "Entity.h"
#include "Tile.h"
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

	void setProjectileSpawner(std::function<void(std::unique_ptr<Projectile>)> spawner);
private:
	void shoot();

	void moveVertical() override;
	void move() override;

	void calculateVelocity() override;

	void animateJump();
	void animateFall();
	void animateLand();
	void countFramesOnGround();

	function<void(std::unique_ptr<Projectile>)> m_spawnProjectile;

	int lastKeyPressed = -1;
	int framesOnGround = 0;
	float moveSpeed;
	float2 inputVelocity = { 0, 0 };
};