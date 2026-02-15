#include "Presenter.h"
#include "World.h"

extern World world;

SDL_Window* Presenter::m_mainWindow = nullptr;
SDL_Renderer* Presenter::m_mainRenderer = nullptr;
Writer* Presenter::m_writer = new Writer();
int Presenter::m_SCREEN_WIDTH = 0;
int Presenter::m_SCREEN_HEIGHT = 0;

Presenter::Presenter()
{
}

Presenter::~Presenter()
{
}

void Presenter::init()
{
	m_SCREEN_WIDTH = 1920;
	m_SCREEN_HEIGHT = 1080;

	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	m_mainWindow = SDL_CreateWindow("SDL Template Project", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		m_SCREEN_WIDTH, m_SCREEN_HEIGHT, 0);
	
	m_mainRenderer = SDL_CreateRenderer(m_mainWindow, -1, SDL_RENDERER_PRESENTVSYNC);
	
	improveRederer();
	m_writer->init();
}

void Presenter::draw()
{
	SDL_RenderPresent(m_mainRenderer);

	SDL_RenderClear(m_mainRenderer);
}

void Presenter::drawObject(SDL_Texture* texture)
{
	SDL_RenderCopy(m_mainRenderer, texture, NULL, NULL);
}

void Presenter::drawObject(Drawable& drawable)
{
	SDL_RenderCopy(m_mainRenderer, drawable.texture, NULL, &drawable.rect);
}

void Presenter::drawObject(DrawableWithSrc& drawableWithSrc)
{
	SDL_RenderCopy(m_mainRenderer, drawableWithSrc.texture, &drawableWithSrc.srcRect, &drawableWithSrc.rect);
}

void Presenter::improveRederer()
{
	SDL_DisplayMode DM;
	SDL_GetCurrentDisplayMode(0, &DM);

	auto desctopWidth = DM.w;
	auto desctopHeight = DM.h;

	float2 mouseMultiplier = { (float)m_SCREEN_WIDTH / (float)desctopWidth, (float)m_SCREEN_HEIGHT / (float)desctopHeight };

	if (SDL_SetWindowFullscreen(m_mainWindow, SDL_WINDOW_FULLSCREEN_DESKTOP) < 0)
	{
		cout << "Failed to set fullscreen mode: " << endl;
	}

	world.m_inputManager.setMouseMultiply(mouseMultiplier);

	SDL_RenderSetLogicalSize(m_mainRenderer, m_SCREEN_WIDTH, m_SCREEN_HEIGHT);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
}

SDL_Texture* loadTexture(string imgPath)
{
	return LoadTexture(imgPath, Presenter::m_mainRenderer);
}
