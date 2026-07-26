#pragma once

#include "Mob.h"

class Drop;

class Projectile : public DrawableWithSrc
{
public:
	Projectile();
	~Projectile();

	void init(Entity* owner);
	void update();
	void draw(float2 camCoords);

	void calculateVelocity();
	
	void setDropSpawner(function<void(unique_ptr<Drop>)> spawner);

	SDL_FRect getMapRect() { return hitbox.rect; };
	bool getFirstFrame() { return firstFrame; };
	int getDamage() { return damage; };
	float2 getVelocity() { return velocity; };

	bool isAlive = true;
	bool firstFrame = true;

	TILE_TYPE canBreak[1] = { TILE_TYPE::BUSH };
private:
	void zoomUpdate();
	float2 calculateNetForce();
	void applyVelocity();
	void stopOutOfBounds();
	void firstFrameColl();

	std::function<void(unique_ptr<Drop>)> m_spawnDrop;

	Entity* m_owner;

	float2 velocity = { 0, 0 };
	float2 netForce;

	FDrawable hitbox;

	vector<float2> normalDirs;


	int damage;
};