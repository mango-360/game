#pragma once

#include "defines.h"
#include "Tile.h"
#include <algorithm>

class Entity : public DrawableWithSrc
{
public:
	Entity();
	~Entity();
	virtual void init(Tile(*map)[MAP_WIDTH]);
	virtual void update();
	virtual void draw(float2 camCoords); // camera-aware draw
	int2 getRealCoords();
	SDL_FRect getMapRect() const { return hitbox.rect; };

protected:
	void zoomUpdate();
	virtual void move();
	void jump();
	virtual void moveVertical() = 0;
	void drawHitBox(float2 camCoords); // for debugging

	virtual void collision();
	virtual void calculateVelocity();
	float2 calculateNetForce();
	void applyVelocity();
	void stopOutOfBounds();

	float jumpStrength;
	float moveSpeed;
	float2 gravity = { 0.0f, 0.005f };
	float2 velocity = { 0, 0 };
	float2 netForce;

	bool isOnGround = false;
	bool isLeftWall = false;
	bool isOnWall = false;
	bool onCeiling = false;

	FDrawable hitbox;
	static Tile* m_map[MAP_HEIGHT][MAP_WIDTH];
};