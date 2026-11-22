#include "Drop.h"
#include "Presenter.h"
#include "InputManager.h"
#include "ImgManager.h"

Drop::Drop()
{
}

Drop::~Drop()
{
}

void Drop::init(int2 coords, DROP_TYPE dropType)
{
    m_dropType = dropType;

    // map drop type to a texture (reuse tile textures for now)
    switch (m_dropType)
    {
    case DROP_TYPE::GRASSBLOCK_DROP:
        m_dropDrawable.texture = ImgManager::m_grassBlockTexture;
        m_dropTileType = TILE_TYPE::GRASSBLOCK;
        break;
    case DROP_TYPE::DIRTBLOCK_DROP:
        m_dropDrawable.texture = ImgManager::m_dirtBlockTexture;
        m_dropTileType = TILE_TYPE::DIRTBLOCK;
        break;
    case DROP_TYPE::STICK:
        m_dropDrawable.texture = ImgManager::m_stickTexture;
        m_dropTileType = TILE_TYPE::NONE_TYPE;
        break;
	case DROP_TYPE::LEAF:
        m_dropDrawable.texture = ImgManager::m_leafTexture;
        m_dropTileType = TILE_TYPE::NONE_TYPE;
		break;
    default:
        m_dropDrawable.texture = nullptr;
        m_dropTileType = TILE_TYPE::NONE_TYPE;
        break;
    }

    m_hitbox = { static_cast<float>(coords.x), static_cast<float>(coords.y), 0.8f, 0.8f };
    m_dropDrawable.rect = { 0, 0, static_cast<int>(TILE_SIZE * InputManager::getZoom()), static_cast<int>(TILE_SIZE * InputManager::getZoom()) };

    float angle = normal_distribution<float> { 0.0f, DROP_SPREAD_ANGLE / 3}(rng);
    angle = clamp(angle, -DROP_SPREAD_ANGLE, DROP_SPREAD_ANGLE);

    float speed = uniform_real_distribution<float>{ DROP_INITIAL_SPEED - DROP_SPEED_DEVIATION, DROP_INITIAL_SPEED + DROP_SPEED_DEVIATION }(rng);
    velocity = { speed * sin(angle) , -speed * cos(angle)};
}

void Drop::update()
{
    zoomUpdate();
}

void Drop::draw(float2 camCoords)
{
    m_dropDrawable.rect.x = static_cast<int>(round((m_hitbox.x - camCoords.x) * (TILE_SIZE * InputManager::getZoom())) - (m_dropDrawable.rect.w - m_hitbox.w * TILE_SIZE * InputManager::getZoom()) / 2);
    m_dropDrawable.rect.y = static_cast<int>(round((m_hitbox.y - camCoords.y) * (TILE_SIZE * InputManager::getZoom())) - (m_dropDrawable.rect.h - m_hitbox.h * TILE_SIZE * InputManager::getZoom()) / 2);

    drawObject(m_dropDrawable);
}

void Drop::destroy()
{
    m_dropDrawable.texture = nullptr;
}

void Drop::calculateVelocity()
{
    velocity += GRAVITY;

    if (velocity.x != 0 && abs(velocity.x) < 0.001f) velocity.x = 0;
    if (velocity.y != 0 && abs(velocity.y) < 0.001f) velocity.y = 0;
}

void Drop::applyVelocity()
{

    velocity += friction;

    m_hitbox.x += velocity.x;
    m_hitbox.y += velocity.y;
}

void Drop::calculateFriction(float frictionValue)
{
    friction.x = min(abs(velocity.x), abs(GRAVITY.y * frictionValue));
    friction.y = min(abs(velocity.y), abs(GRAVITY.y * frictionValue * 0.1f));

    if (velocity.x > 0) friction.x *= -1;
    if (velocity.y > 0) friction.y *= -1;
}

void Drop::resolveCollision(SDL_FRect tileRect)
{
    ResolveDynamicRectVsRect(&m_hitbox, velocity, &tileRect);
}

void Drop::stopOutOfBounds()
{
    if (m_hitbox.x < 0)
    {
        m_hitbox.x = 0;
        velocity.x = 0;
    }
    else if (m_hitbox.x + m_hitbox.w > MAP_WIDTH)
    {
        m_hitbox.x = MAP_WIDTH - m_hitbox.w;
        velocity.x = 0;
    }
    if (m_hitbox.y < 0)
    {
        m_hitbox.y = 0;
        velocity.y = 0;
    }
    else if (m_hitbox.y + m_hitbox.h > MAP_HEIGHT)
    {
        m_hitbox.y = MAP_HEIGHT - m_hitbox.h;
        velocity.y = 0;
    }
}

void Drop::reset()
{
    m_dropType = DROP_TYPE::NONE_DROP;
    m_dropTileType = TILE_TYPE::NONE_TYPE;
    m_dropDrawable.rect = { -1, -1, 0, 0 };
    m_dropDrawable.texture = nullptr;
}

void Drop::zoomUpdate()
{
    if (InputManager::isZoomChanged())
    {
        m_dropDrawable.rect.w = static_cast<int>(TILE_SIZE * InputManager::getZoom());
        m_dropDrawable.rect.h = static_cast<int>(TILE_SIZE * InputManager::getZoom());
    }
}