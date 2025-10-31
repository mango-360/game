#include "Engine.h"

SDL_Texture* LoadTexture(string imgPath, SDL_Renderer* renderer)
{
	string fullPath = IMG_FOLDER + imgPath;

	SDL_Surface* tempSurface = SDL_LoadBMP(fullPath.c_str());

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tempSurface);

    if (texture == nullptr)
    {
		cout << "Failed to load texture: " << imgPath << endl;

		tempSurface = SDL_LoadBMP((IMG_FOLDER + "Null.bmp").c_str());

		texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    }

	SDL_FreeSurface(tempSurface);

	return texture;
}

bool isMouseInRect(int2 mouseCoor, SDL_Rect rect)
{
    if (mouseCoor.x >= rect.x && mouseCoor.x <= rect.x + rect.w && 
        mouseCoor.y >= rect.y && mouseCoor.y <= rect.y + rect.h) 
        return true;

    return false;
}

bool collRectRect(SDL_Rect rect1, SDL_Rect rect2)
{
    if (rect1.x + rect1.w < rect2.x || rect1.y + rect1.h < rect2.y || 
        rect2.x + rect2.w < rect1.x || rect2.y + rect2.h < rect1.y) 
        return false;

    return true;
}
