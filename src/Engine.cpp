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
    return (rect1.x < rect2.x + rect2.w && rect1.x + rect1.w > rect2.x &&
			rect1.y < rect2.y + rect2.h && rect1.y + rect1.h > rect2.y);
}

bool FcollRectRect(SDL_FRect rect1, SDL_FRect rect2)
{
   return (rect1.x < rect2.x + rect2.w && rect1.x + rect1.w > rect2.x &&
           rect1.y < rect2.y + rect2.h && rect1.y + rect1.h > rect2.y);
}

bool FcollRayRect(float2 rayOrigin, float2 rayDir, SDL_FRect* rect, float2& contactPoint, float2& contactNormal, float& tHitNear)
{
	contactNormal = { 0,0 };
	contactPoint = { 0,0 };

	// Cache division
	float2 invdir = 1.0f /rayDir;

	// Calculate intersections with rectangle bounding axes
	float2 t_near = { (rect->x - rayOrigin.x) * invdir.x, (rect->y - rayOrigin.y) * invdir.y };
	float2 t_far = { (rect->x + rect->w - rayOrigin.x) * invdir.x, (rect->y + rect->h - rayOrigin.y) * invdir.y };

	if (isnan(t_far.y) || isnan(t_far.x)) return false;
	if (isnan(t_near.y) ||isnan(t_near.x)) return false;

	// Sort distances
	if (t_near.x > t_far.x) swap(t_near.x, t_far.x);
	if (t_near.y > t_far.y) swap(t_near.y, t_far.y);

	// Early rejection		
	if (t_near.x > t_far.y || t_near.y > t_far.x) return false;

	// Closest 'time' will be the first contact
	tHitNear = max(t_near.x, t_near.y);

	// Furthest 'time' is contact on opposite side of target
	float t_hit_far = min(t_far.x, t_far.y);

	// Reject if ray direction is pointing away from object
	if (t_hit_far < 0) return false;

	// Contact point of collision from parametric line equation
	contactPoint = tHitNear * rayDir;
	contactPoint += rayOrigin;

	if (t_near.x > t_near.y)
		if (invdir.x < 0)
			contactNormal = { 1, 0 };
		else
			contactNormal = { -1, 0 };
	else if (t_near.x < t_near.y)
		if (invdir.y < 0)
			contactNormal = { 0, 1 };
		else
			contactNormal = { 0, -1 };

	// Note if t_near == t_far, collision is principly in a diagonal
	// so pointless to resolve. By returning a CN={0,0} even though its
	// considered a hit, the resolver wont change anything.

	return true;
}

bool DynamicRectVsRect(const SDL_FRect* r_dynamic, const float2& velocity, const SDL_FRect& r_static,
	float2& contact_point, float2& contact_normal, float& contact_time)
{
	// Check if dynamic rectangle is actually moving - we assume rectangles are NOT in collision to start
	if (velocity == 0) return false;

	// Expand target rectangle by source dimensions
	SDL_FRect expanded_target;
	expanded_target.x = r_static.x - r_dynamic->w / 2;
	expanded_target.y = r_static.y - r_dynamic->h / 2;
	expanded_target.w = r_static.w + r_dynamic->w;
	expanded_target.h = r_static.h + r_dynamic->h;

	float2 r_dynamic_center = { r_dynamic->x + r_dynamic->w / 2, r_dynamic->y + r_dynamic->h / 2 };

	if (FcollRayRect(r_dynamic_center, velocity, &expanded_target, contact_point, contact_normal, contact_time))
	{
		return (contact_time >= 0.0f && contact_time < 1.0f);
	}
	else
		return false;
}

bool ResolveDynamicRectVsRect(SDL_FRect* r_dynamic, float2& velocity, SDL_FRect* r_static)
{
	float2 contact_point, contact_normal;
	float contact_time = 0.0f;
	if (DynamicRectVsRect(r_dynamic, velocity, *r_static, contact_point, contact_normal, contact_time))
	{
		velocity += (contact_normal * float2({ fabsf(velocity.x), fabsf(velocity.y) })) * (1.0f - contact_time);
		
		return true;
	}

	return false;
}