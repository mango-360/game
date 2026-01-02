#pragma once

#include "Entity.h"

class Player : public Entity
{
public:
	Player();
	~Player();
	void init();
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
	bool checkIfWillHitGround();
	void landOnGround(SDL_Rect ground);

	int lastKeyPressed = -1;
	int jumpStrength;
	int2 velocity = { 0, 0 };
	int gravity;

	float moveSpeed;

	bool isOnGround = true;

	DrawableWithSrc hitbox;
	SDL_Rect tmpGroundHitBox; // temporary, for Ground hitbox
	float2 mapCoords;
};