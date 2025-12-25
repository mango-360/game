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

	cameraRect = 
	{ static_cast<int>(m_player->getRealCoords().x + m_player->rect.w / 2 - Presenter::m_SCREEN_WIDTH / 2),		// x
		static_cast<int>(m_player->getRealCoords().y + m_player->rect.h / 2 - Presenter::m_SCREEN_HEIGHT / 2),	// y
		Presenter::m_SCREEN_WIDTH, Presenter::m_SCREEN_HEIGHT };												// w, h
}

void Camera::update()
{
	int2 playerCenteredPos = 
	  { static_cast<int>(m_player->getRealCoords().x + m_player->rect.w / 2 - Presenter::m_SCREEN_WIDTH / 2),
		static_cast<int>(m_player->getRealCoords().y + m_player->rect.h / 2 - Presenter::m_SCREEN_HEIGHT / 2) };  //cameraPos where player is centered

	int n = 5; //temporary follow speed factor

	cameraRect.x += (playerCenteredPos.x - cameraRect.x) / n; //smooth camera follow
	cameraRect.y += (playerCenteredPos.y - cameraRect.y) / n;

	//cout << "Camera coords: " << cameraRect.x << ", " << cameraRect.y << std::endl;
}
