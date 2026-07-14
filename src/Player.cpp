#include "Player.h"
#include <Presenter.h>

#include "InputManager.h"
#include "SoundManager.h"
#include "ImgManager.h"

#include "Projectile.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::init(Tile(*map)[MAP_WIDTH])
{
	string playerfile = "player.txt", tmp, playerImg, hitboxImg;

	fstream stream;

	stream.open(CONFIG_FOLDER + playerfile);

	stream >> tmp >> playerImg;
	stream >> tmp >> srcRect;
	stream >> tmp >> rect;
	stream >> tmp >> hitboxImg >> hitbox.rect;
	stream >> tmp >> moveSpeed;
	stream >> tmp >> maxInputVelocity;
	stream >> tmp >> jumpStrength;

	texture = loadTexture(playerImg);
	hitbox.texture = loadTexture(hitboxImg);

	stream.close();

	for (int y = 0; y < MAP_HEIGHT; ++y)
	{
		for (int x = 0; x < MAP_WIDTH; ++x)
		{
			m_map[y][x] = &map[y][x];
		}
	} 

	landingStartSpriteFrame = 11;
	NoJumpLandingSpriteFrame = 12;

	initInventory();
}

void Player::updatePrePhysics()
{
	zoomUpdate();

	toggleInventory();

	if (isInvOpen)
	{
		if(!InputManager::changeZoom(INVENTORY_ZOOM)) 
		{
			updateInventory();
		}
	}
	else 
	{
		if(closingInv) 		
		{
			if(!InputManager::changeZoom(prevZoom)) 
				closingInv = false;
		}  
		shoot();
		move();
	}

	animateJump();
	animateFall();
	animateLand();

	calculateVelocity();
}

void Player::updatePostPhysics()
{
	addFriction();

	applyVelocity();

	stopOutOfBounds();

	countFramesOnGround();
}

void Player::setProjectileSpawner(function<void(unique_ptr<Projectile>)> spawner)
{
	m_spawnProjectile = std::move(spawner);
}

void Player::shoot()
{
    if ((InputManager::isKeyClicked(SDL_SCANCODE_P) || InputManager::isMouseClicked()) && m_spawnProjectile)
    {
        auto projectile = std::make_unique<Projectile>();
        projectile->init(this);
        m_spawnProjectile(std::move(projectile)); // hand ownership to Board via callback
    }
}

void Player::move()
{

	moveVertical();

	if ((InputManager::isKeyPressed(SDL_SCANCODE_W) || InputManager::isKeyPressed(SDL_SCANCODE_SPACE)) && isOnGround)
	{
		jump();
	}
}

void Player::moveVertical()
{
	if (InputManager::isKeyPressed(SDL_SCANCODE_D))
	{
		if (InputManager::isKeyPressed(SDL_SCANCODE_A))
		{
			if (lastKeyPressed == SDL_SCANCODE_D)
			{
				inputVelocity.x -= moveSpeed;
				srcRect.y = srcRect.h;
			}
			else
			{
				inputVelocity.x += moveSpeed;
				srcRect.y = 0;
			}
		}
		else
		{
			inputVelocity.x += moveSpeed;
			srcRect.y = 0;
			lastKeyPressed = SDL_SCANCODE_D;
		}
	}
	else 
	{
		if (InputManager::isKeyPressed(SDL_SCANCODE_A))
		{
			inputVelocity.x -= moveSpeed;
			srcRect.y = srcRect.h;
			lastKeyPressed = SDL_SCANCODE_A;
		}
		else inputVelocity.x = 0;
	}
}

void Player::animateJump()
{
	if (isJumping)
	{
		if (isOnGround) srcRect.x = srcRect.w;

		else
		{
			float phase = velocity.y / jumpStrength; // from -1 to 1 and above
			
				 if (phase < -0.83) srcRect.x = srcRect.w * 2;
			else if (phase < -0.66) srcRect.x = srcRect.w * 3;
			else if (phase < -0.50) srcRect.x = srcRect.w * 4;
			else if (phase < -0.33) srcRect.x = srcRect.w * 5;
			else if (phase < -0.16) srcRect.x = srcRect.w * 6;

			else if (phase < 0.25) srcRect.x = srcRect.w * 7;
			else if (phase < 0.50) srcRect.x = srcRect.w * 8;
			else if (phase < 0.75) srcRect.x = srcRect.w * 9;
			else				   srcRect.x = srcRect.w * 10;
		}
		
		landingStartSpriteFrame = 11;
	}
}

void Player::animateFall()
{
	if (!isOnGround && !isJumping)
	{
		float phase = velocity.y / jumpStrength;

		if		(phase < 0.40) srcRect.x = 0;
		else if (phase < 0.75) srcRect.x = srcRect.w * 9;
		else				   srcRect.x = srcRect.w * 10;
	}
}

void Player::animateLand()
{
	if (isOnGround && !isJumping)
	{
		int landingSpriteFrame = framesOnGround / LANDING_SPRITE_FRAME_DURATION;

		if (landingSpriteFrame + landingStartSpriteFrame > 15) srcRect.x = 0;
		else srcRect.x = srcRect.w * (landingStartSpriteFrame + landingSpriteFrame);
	}
}

void Player::toggleInventory()
{
	if(InputManager::isKeyClicked(SDL_SCANCODE_GRAVE))
	{
		isInvOpen = !isInvOpen;
		
		if (isInvOpen)
		{
			if(!closingInv) prevZoom = InputManager::getZoom();
		}
		else closingInv = true;
	}
}

void Player::initDropInInventory(Drop* drop, int index)
{
	drop->m_dropDrawable.rect = m_inventorySlots[index].rect;
	drop->m_dropDrawable.opacity = 0;
}

void Player::addToInventory(unique_ptr<Drop> drop)
{
	int openSlot = -1;

	for(int i = INVENTORY_SIZE - 1; i >= 0; --i)
	{
		if(inventory[i].second == 0)
		{
			openSlot = i;
		}
		else if(inventory[i].first.getDropType() == drop->getDropType() && inventory[i].second < inventory[i].first.getStackSize())
		{
			openSlot = i;
			break;
		}
	}

	if (openSlot != -1)
	{
		inventory[openSlot].first = *drop;
		inventory[openSlot].second++;
		
		initDropInInventory(&inventory[openSlot].first, openSlot);

		SoundManager::playSound(SOUND::ITEM_PICK_UP);
	}
}

void Player::updateInventory()
{
	if (InputManager::isMouseClicked())
	{
		activeSlotIndex = -1;

		for (int i = 0; i < INVENTORY_SIZE; ++i)
		{
			if (isMouseInRect(m_inventorySlots[i].rect))
			{
				activeSlotIndex = i;
			}
		}
	}

	if(InputManager::isMouseReleased())
	{
		for (int i = 0; i < INVENTORY_SIZE; ++i)
		{
			if (isMouseInRect(m_inventorySlots[i].rect) // check for released slot
				&& activeSlotIndex != i && activeSlotIndex != -1 && inventory[activeSlotIndex].second > 0   // check for a different occupied active slot
				&& inventory[i].second < inventory[i].first.getStackSize()) //check for stack size
			{
				if (inventory[i].first.getDropType() != inventory[activeSlotIndex].first.getDropType())
					swap(inventory[i], inventory[activeSlotIndex]);
				else
				{
					if(inventory[activeSlotIndex].second + inventory[i].second <= inventory[i].first.getStackSize())
					{
						inventory[i].second += inventory[activeSlotIndex].second;
						inventory[activeSlotIndex].second = 0;
					}
					else
					{
						int spaceLeft = inventory[i].first.getStackSize() - inventory[i].second;
						inventory[i].second += spaceLeft;
						inventory[activeSlotIndex].second -= spaceLeft;
					}
				}
				initDropInInventory(&inventory[activeSlotIndex].first, activeSlotIndex);
				initDropInInventory(&inventory[i].first, i);

				break;
			}
		}
	}
}

void Player::initInventory()
{
	string tmp;

	int2 startPos;

	int dimentions, gapWidth, rowSpacing;
	int firstRowNum, secondRowNum, thirdRowNum;

	fstream stream;

	stream.open(CONFIG_FOLDER + "inventory.txt");

	stream >> tmp >> startPos;
	stream >> tmp >> dimentions;
	stream >> tmp >> gapWidth;
	stream >> tmp >> rowSpacing;

	stream >> tmp >> firstRowNum;
	stream >> tmp >> secondRowNum;
	stream >> tmp >> thirdRowNum;

	stream.close();

	for (int i = 0; i < INVENTORY_SIZE; ++i)
	{
		m_inventorySlots[i].texture = ImgManager::m_slotImg;
		if(i < firstRowNum) m_inventorySlots[i].rect = { startPos.x + (dimentions + gapWidth) * i, startPos.y, dimentions, dimentions };
		else if(i < firstRowNum + secondRowNum) m_inventorySlots[i].rect = { startPos.x + (dimentions + gapWidth) * (i - firstRowNum - 1), startPos.y + rowSpacing + dimentions, dimentions, dimentions };
		else m_inventorySlots[i].rect = { startPos.x + (dimentions + gapWidth) * (i - firstRowNum - secondRowNum), startPos.y + rowSpacing * 2 + dimentions * 2, dimentions, dimentions };
	}

	//empty inventory
	for (int i = 0; i < INVENTORY_SIZE; ++i)
		inventory[i].second = 0;

	m_inventoryItemCount.init("inventoryTextField.txt");
}

void Player::drawInventory()
{
	if ((isInvOpen || closingInv) && InputManager::getZoom() >= INVENTORY_ZOOM - OPEN_INV_END_MARGIN)
	{
		for (int i = 0; i < INVENTORY_SIZE; ++i)
		{
			m_inventorySlots[i].opacity = (int)((InputManager::getZoom() - INVENTORY_ZOOM + OPEN_INV_END_MARGIN) * (255.0f / OPEN_INV_END_MARGIN));
			inventory[i].first.m_dropDrawable.opacity = m_inventorySlots[i].opacity;

			drawObject(m_inventorySlots[i]);
			drawObject(inventory[i].first.m_dropDrawable);

			if(inventory[i].second > 1)
			{
				m_inventoryItemCount.setText(to_string(inventory[i].second),
					{ (int)(m_inventorySlots[i].rect.x + m_inventorySlots[i].rect.w * INV_ITEM_COUNT_SLOT_RATIO), (int)(m_inventorySlots[i].rect.y + m_inventorySlots[i].rect.h * INV_ITEM_COUNT_SLOT_RATIO) }, 
					true, 
					inventory[i].first.m_dropDrawable.opacity);

				m_inventoryItemCount.draw();
			}
		}
	}
}

void Player::calculateVelocity()
{
	velocity += calculateNetForce();

	if(velocity.x > 0) inputVelocity.x = clamp(inputVelocity.x, -maxInputVelocity.x, max(0.0f, maxInputVelocity.x - velocity.x)); // clamps x input velocity
	else	inputVelocity.x = clamp(inputVelocity.x, min(0.0f, -maxInputVelocity.x - velocity.x), maxInputVelocity.x); 
	//inputVelocity.y = clamp(inputVelocity.y, -maxInputVelocity.y, maxInputVelocity.y); // clamps y input velocity

	velocity += inputVelocity;

	if(velocity.x != 0 && abs(velocity.x) < 0.001f) velocity.x = 0;
	if(velocity.y != 0 && abs(velocity.y) < 0.001f) velocity.y = 0;
}

void Player::countFramesOnGround()
{
	if (!isOnGround) framesOnGround = 0;
	else framesOnGround++;
}
