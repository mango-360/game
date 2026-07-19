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

    m_gridRect = { static_cast<float>(coords.x), static_cast<float>(coords.y), 1.0f, 1.0f };
    m_dropDrawable.rect = { 0, 0, static_cast<int>(TILE_SIZE * InputManager::getZoom()), static_cast<int>(TILE_SIZE * InputManager::getZoom()) };
}


void Drop::update()
{
    zoomUpdate();
}


void Drop::draw(float2 camCoords)
{
    m_dropDrawable.rect.x = static_cast<int>(round((m_gridRect.x - camCoords.x) * (TILE_SIZE * InputManager::getZoom())));
    m_dropDrawable.rect.y = static_cast<int>(round((m_gridRect.y - camCoords.y) * (TILE_SIZE * InputManager::getZoom())));

    drawObject(m_dropDrawable);
}

void Drop::destroy()
{
    m_dropDrawable.texture = nullptr;
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