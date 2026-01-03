#pragma once

#include "Entity.h"
#include "Tile.h"

class Player : public Entity
{
public:
	Player();
	~Player();
	void init(Tile* map[MAP_HEIGHT][MAP_WIDTH]);
	void update();
	void draw(float2 camCoords); // camera-aware draw
	int2 getRealCoords();
	float2 getMapCoords() const { return mapCoords; };

private:
	void zoomUpdate();
	void move();
	void jump();
	void moveVertical();
	void moveSprite();
	void drawHitBox(); // for debugging

	void gravityEffect();
	void checkForGround();
	void landOnGround(SDL_Rect ground);
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
	
	bool isOnGround = true;

	DrawableWithSrc hitbox;
	SDL_Rect tmpGroundHitBox; // temporary, for Ground hitbox
	float2 mapCoords;

	Tile* m_map[MAP_HEIGHT][MAP_WIDTH];
};