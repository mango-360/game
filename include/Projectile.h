#pragma once

#include "Mob.h"

class Projectile : public DrawableWithSrc
{
public:
	Projectile();
	~Projectile();

	void init(Entity* owner);
	static void setMobs(vector<Mob>* mobs) { m_mobs = mobs; }
	void update();
	void draw(float2 camCoords);

	bool isAlive = true;
private:
	void zoomUpdate();
	void collision();
	void calculateVelocity();
	float2 calculateNetForce();
	void applyVelocity();
	void stopOutOfBounds();

	Entity* m_owner;

	float2 velocity = { 0, 0 };
	float2 netForce;

	FDrawable hitbox;

	vector<float2> normalDirs;

	int damage;

	static vector<Mob>* m_mobs;
};