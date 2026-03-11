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
	void update();
	void setProjectileSpawner(std::function<void(std::unique_ptr<Projectile>)> spawner);
private:
	void shoot();
	void move();
	void moveVertical();
	void animateJump();
	void animateFall();
	void animateLand();

	void collision();
	void calculateFriction(int2 coords);
	void calculateVelocity();
	void addFriction();
	void countFramesOnGround();

	std::function<void(std::unique_ptr<Projectile>)> m_spawnProjectile;

	int lastKeyPressed = -1;
	int framesOnGround = 0;
	int landingStartSpriteFrame = 11;
	float moveSpeed;
	float2 friction;
	float2 inputVelocity = { 0, 0 };
};