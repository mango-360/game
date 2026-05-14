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
	void updatePrePhysics() override;
	void updatePostPhysics() override;
	void setProjectileSpawner(std::function<void(std::unique_ptr<Projectile>)> spawner);
private:
	void shoot();
	void move() override;
	void moveVertical() override;
	void animateJump();
	void animateFall();
	void animateLand();

	void calculateVelocity() override;
	void countFramesOnGround();

	std::function<void(std::unique_ptr<Projectile>)> m_spawnProjectile;

	int lastKeyPressed = -1;
	int framesOnGround = 0;
	float moveSpeed;
	float2 inputVelocity = { 0, 0 };
};
