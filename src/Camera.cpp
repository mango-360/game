#include "Camera.h"
#include "Presenter.h"
#include "InputManager.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::init(Player* player)
{
	m_player = player;

	const float width = Presenter::m_SCREEN_WIDTH / (TILE_SIZE * InputManager::getZoom());
	const float height = Presenter::m_SCREEN_HEIGHT / (TILE_SIZE * InputManager::getZoom());

	rect = { m_player->getMapRect().x - width / 2, m_player->getMapRect().y - height / 2, width, height};
}

void Camera::update()
{
	updateZoom();

	smoothFollow();

	cout << "Camera coords: " << rect.x << ", " << rect.y << ", " << rect.w << ", " << rect.h << endl;
}

void Camera::updateZoom()
{
	if (InputManager::isZoomChanged())
	{
		const float width = Presenter::m_SCREEN_WIDTH / (TILE_SIZE * InputManager::getZoom());
		const float height = Presenter::m_SCREEN_HEIGHT / (TILE_SIZE * InputManager::getZoom());

		rect.x += (rect.w - width) / 2; 
		rect.y += (rect.h - height) / 2;
		rect.w = width;
		rect.h = height;
	}
}

void Camera::smoothFollow()
{
	float2 playerCenteredPos = //cameraPos where player is centered
	{ m_player->getMapRect().x + 0.5f - rect.w / 2,
	  m_player->getMapRect().y + 0.5f - rect.h / 2 };

	int n = 5; //temporary follow speed factor

	rect.x += (playerCenteredPos.x - rect.x) / n; //smooth camera follow
	rect.y += (playerCenteredPos.y - rect.y) / n;
}
