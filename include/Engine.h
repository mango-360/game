#pragma once

#include "defines.h"

SDL_Texture* LoadTexture(string imgPath, SDL_Renderer* renderer);
bool isMouseInRect(int2 mouseCoor, SDL_Rect rect);
bool collRectRect(SDL_Rect rect1, SDL_Rect rect2);
bool FcollRectRect(SDL_FRect rect1, SDL_FRect rect2);
bool FcollRayRect(float2 rayOrigin, float2 rayDir, SDL_FRect* rect, float2& contactPoint, float2& contactNormal, float& tHitNear);
bool DynamicRectVsRect(const SDL_FRect* r_dynamic, const float2 velocity, const SDL_FRect& r_static, float2& contact_point, float2& contact_normal, float& contact_time);
bool ResolveDynamicRectVsRect(SDL_FRect* r_dynamic, float2 velocity, SDL_FRect* r_static);