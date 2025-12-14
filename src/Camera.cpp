#include "Camera.h"
#include "Presenter.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::init(Player* player)
{
	m_player = player;

	cameraRect = { 0, 0, Presenter::m_SCREEN_WIDTH, Presenter::m_SCREEN_HEIGHT };
}

void Camera::update()
{
	cameraRect.x = static_cast<int>(m_player->getRealCoords().x + m_player->rect.w / 2 - Presenter::m_SCREEN_WIDTH / 2);
	cameraRect.y = static_cast<int>(m_player->getRealCoords().y + m_player->rect.h / 2- Presenter::m_SCREEN_HEIGHT / 2);
	//cout << "Camera coords: " << cameraRect.x << ", " << cameraRect.y << std::endl;
}
