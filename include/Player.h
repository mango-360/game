#pragma once

#include "Entity.h"

class Player : public Entity
{
public:
	Player();
	~Player();
	void init();
	void update();
	void draw();
	int2 getRealCoords();

private:
	void move();
	void jump();
	void moveVertical();
	void moveSprite();

	void gravityEffect();
	bool checkIfWillHitGround();
	void landOnGround(SDL_Rect ground);

	int lastKeyPressed = -1;
	int jumpStrength;
	int2 velocity = { 0, 0 };
	int gravity;
	int2 hitBoxOffset;

	float moveSpeed;

	bool isOnGround = false;

	SDL_Rect hitBox;
	SDL_Rect tmpGroundHitBox; // temporary, for Ground hitbox
	float2 mapCoords;
};