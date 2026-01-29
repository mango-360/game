#pragma once

#include "Entity.h"
#include "Tile.h"

class Player : public Entity
{
public:
	Player();
	~Player();
	void init(Tile(*map)[MAP_WIDTH]);
	void update();
	void draw(float2 camCoords); // camera-aware draw
	int2 getRealCoords();
	SDL_FRect getMapRect() const { return hitbox.rect; };

private:
	void zoomUpdate();
	void move();
	void jump();
	void moveVertical();
	void drawHitBox(float2 camCoords); // for debugging

	void collision();
	void checkForGround();
	void landOnGround(float groundY);
	void landOnWall(SDL_FRect wall, bool isOnLeftWall);
	void calculateVelocity();
	void applyVelocity();

	int lastKeyPressed = -1;
	float jumpStrength;
	float gravity;
	float moveSpeed;
	float2 velocity = { 0, 0 };
	float2 gameVelocity = { 0, 0 };
	float2 inputVelocity = { 0, 0 };
	float2 maxInputVelocity;
	
	bool isOnGround = false;
	bool isLeftWall = false;
	bool isOnWall = false;
	bool onCeiling = false;

	FDrawable hitbox;
	Tile* m_map[MAP_HEIGHT][MAP_WIDTH];
};