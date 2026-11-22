#pragma once

#include "defines.h"

class Drop
{
public:
	Drop();
	~Drop();
	void init(int2 coords, DROP_TYPE dropType);
	void update();
	void draw(float2 camCoords);
	void destroy();

	void calculateVelocity();
	void applyVelocity();
	void calculateFriction(float frictionValue);
	void resolveCollision(SDL_FRect tileRect);
	void stopOutOfBounds();

	void reset();
	
	int getStackSize() const { return stackSize; }
	float2 getVelocity() const { return velocity; }
	DROP_TYPE getDropType() const { return m_dropType; }

	SDL_FRect getHitbox() const { return m_hitbox; }

	DrawableWithOpacity m_dropDrawable;
protected:
	void zoomUpdate();

	int stackSize = 5;
	
	float2 velocity = { 0.0f, 0.0f };
	float2 friction;

	DROP_TYPE m_dropType;
	TILE_TYPE m_dropTileType;

	SDL_FRect m_hitbox;
};