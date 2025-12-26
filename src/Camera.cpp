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

	cameraRect = 
	{ static_cast<int>(m_player->getRealCoords().x + m_player->rect.w / 2 - Presenter::m_SCREEN_WIDTH / 2),		// x
		static_cast<int>(m_player->getRealCoords().y + m_player->rect.h / 2 - Presenter::m_SCREEN_HEIGHT / 2),	// y
		Presenter::m_SCREEN_WIDTH, Presenter::m_SCREEN_HEIGHT };												// w, h

	prevZoom = InputManager::getZoom();
}

void Camera::update()
{
	updateZoom();

	int2 playerCenteredPos = //cameraPos where player is centered
	  { static_cast<int>(m_player->getRealCoords().x + m_player->rect.w / 2 - Presenter::m_SCREEN_WIDTH / 2),
		static_cast<int>(m_player->getRealCoords().y + m_player->rect.h / 2 - Presenter::m_SCREEN_HEIGHT / 2) };  

	int n = 5; //temporary follow speed factor

	cameraRect.x += (playerCenteredPos.x - cameraRect.x) / n; //smooth camera follow
	cameraRect.y += (playerCenteredPos.y - cameraRect.y) / n;

	//cout << "Camera coords: " << cameraRect.x << ", " << cameraRect.y << std::endl;
}

void Camera::updateZoom()
{
	const float newZoom = InputManager::getZoom();

	// guard against invalid previous zoom (avoid div by zero / nonsense)
	if (prevZoom <= 0.0f)
	{
		prevZoom = newZoom;
		return;
	}

	if (prevZoom != newZoom)
	{
		// Scale factor between new and old zoom
		const float scale = newZoom / prevZoom;

		// Camera center in pixels (world space)
		const float camCenterX = static_cast<float>(cameraRect.x) + Presenter::m_SCREEN_WIDTH * 0.5f;
		const float camCenterY = static_cast<float>(cameraRect.y) + Presenter::m_SCREEN_HEIGHT * 0.5f;

		// Scale camera center so relative offsets (including the small smooth-follow offset)
		// remain proportional when zoom changes. This avoids snapping the camera to the player
		// while preventing the "overshoot" caused by incrementally modifying cameraRect using
		// values that were already mutated.
		const float newCamCenterX = camCenterX * scale;
		const float newCamCenterY = camCenterY * scale;

		// Convert back to top-left camera rect
		cameraRect.x = static_cast<int>(std::round(newCamCenterX - Presenter::m_SCREEN_WIDTH * 0.5f));
		cameraRect.y = static_cast<int>(std::round(newCamCenterY - Presenter::m_SCREEN_HEIGHT * 0.5f));

		prevZoom = newZoom;
	}
}