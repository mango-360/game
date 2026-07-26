#pragma once

#include "defines.h"
#include "Tile.h"
#include <algorithm>

class Projectile;

class Entity : public DrawableWithSrc
{

public:
	Entity();
	~Entity();
	virtual void init(Tile(*map)[MAP_WIDTH] , string configFile);
	virtual void update();
	virtual void draw(float2 camCoords); // camera-aware draw

	virtual void calculateVelocity();
	void applyVelocity();

	void resolveCollision(SDL_FRect tileRect);
	void calculateFriction(float frictionValue);
	void stopOutOfBounds();

	int2 getIntCoords();
	SDL_FRect getMapRect() const { return hitbox.rect; }
	SDL_FRect* getMapRectPtr() { return &hitbox.rect; }
	float2 getVelocity() const { return velocity; }

	friend class Projectile;

	bool isOnGround = false;
	bool isLeftWall = false;
	bool isOnWall = false;
	bool onCeiling = false;
	bool isJumping = false;

	int landingStartSpriteFrame = 0;
	int NoJumpLandingSpriteFrame = 0;

protected:
	void zoomUpdate();
	virtual void move();
	void jump();
	virtual void moveVertical();
	void drawHitBox(float2 camCoords); // for debugging

	void addFriction();
	float2 calculateNetForce();

	float jumpStrength;
	float2 maxInputVelocity;
	float2 velocity = { 0, 0 };
	float2 netForce;
	float2 friction;

	FDrawableWithOpacity hitbox;

	vector<float2> normalDirs;

	static Tile* m_map[MAP_HEIGHT][MAP_WIDTH];
};
