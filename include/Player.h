#pragma once

#include "Entity.h"

class Player : public Entity
{
public:
	Player();
	~Player();
	void init();
	void update();

	float2 mapCoords;
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
	int VelocityX = 0;
	int VelocityY = 0;
	int Gravity;
	int hitBoxOffsetX;
	int hitBoxOffsetY;

	float moveSpeed;

	bool isOnGround = false;

	SDL_Rect hitBox;
	SDL_Rect tmpGroundHitBox; // temporary, for Ground hitbox
};