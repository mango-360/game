#pragma once

#include "Mob.h"

class Projectile : public DrawableWithSrc
{
public:
	Projectile();
	~Projectile();

	void init(Entity* owner);
	void update();
	void draw(float2 camCoords);

	void calculateVelocity();
	void collision();

	bool isAlive = true;
private:
	void zoomUpdate();
	float2 calculateNetForce();
	void applyVelocity();
	void stopOutOfBounds();
	void firstFrameColl();

	void dealDamageToTile(int x, int y);

	Entity* m_owner;

	float2 velocity = { 0, 0 };
	float2 netForce;

	FDrawable hitbox;

	vector<float2> normalDirs;

	int damage;

	bool firstFrame = true;
};