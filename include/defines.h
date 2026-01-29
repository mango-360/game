#pragma once

#include <iostream>
#include <fstream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>

using namespace std;

static const string IMG_FOLDER = "img\\";
static const string CONFIG_FOLDER = "config\\";
static const string SOUND_FOLDER = "sound\\";
static const string FONT_FOLDER = "font\\";
static const string TEXT_FIELD_FOLDER = "textField\\";
static const string TILES_FOLDER = "tiles\\";

static const int TILE_SIZE = 32;
static const int MAP_WIDTH = 320;
static const int MAP_HEIGHT = 240;

struct float2
{
	float x, y;

	void reset() { x = 0; y = 0; }

	void set(float2 a) { x = a.x; y = a.y; }
	void operator=(float2 a) { x = a.x; y = a.y; }

	bool operator==(float2 a) { return (x == a.x && y == a.y); }
	bool operator!=(float2 a) { return (x != a.x || y != a.y); }

	float2 operator+(float a) { return { a + x, a + y }; }
	float2 operator-(float a) { return { a - x, a - y }; }
	float2 operator*(float a) { return { a * x, a * y }; }
	float2 operator/(float a) { return { a / x, a / y }; }

	float2 operator+(float2 a) const { return { x + a.x, y + a.y }; }
	float2 operator-(float2 a) const { return { x - a.x, y - a.y }; }
	float2 operator*(float2 a) const { return { x * a.x, y * a.y }; }
	float2 operator/(float2 a) const { return { x / a.x, y / a.y }; }

	void operator+=(float a) { x += a; y += a; }
	void operator-=(float a) { x -= a; y -= a; }
	void operator*=(float a) { x *= a; y *= a; }
	void operator/=(float a) { x /= a; y /= a; }

	void operator+=(float2 a) { x += a.x; y += a.y; }
	void operator-=(float2 a) { x -= a.x; y -= a.y; }
	void operator*=(float2 a) { x *= a.x; y *= a.y; }
	void operator/=(float2 a) { x /= a.x; y /= a.y; }
};

struct int2
{
	int x, y;

	void reset() { x = 0; y = 0; }

	void set(int2 a) { x = a.x; y = a.y; }
	void operator=(int2 a) { x = a.x; y = a.y; }

	bool operator==(int2 a) { return (x == a.x && y == a.y); }
	bool operator!=(int2 a) { return (x != a.x || y != a.y); }

	int2 operator+(int2 a) { return { x + a.x, y + a.y }; }
	int2 operator-(int2 a) { return { x - a.x, y - a.y }; }
	int2 operator*(int2 a) { return { x * a.x, y * a.y }; }
	int2 operator/(int2 a) { return { x / a.x, y / a.y }; }

	void operator+=(int2 a) { x += a.x; y += a.y; }
	void operator-=(int2 a) { x -= a.x; y -= a.y; }
	void operator*=(int2 a) { x *= a.x; y *= a.y; }
	void operator/=(int2 a) { x /= a.x; y /= a.y; }

	void operator*=(float2 a) { x *= a.x; y *= a.y; }
};

struct Drawable
{
	string img;
	SDL_Texture* texture;
	SDL_Rect rect; //dst rect
};

struct FDrawable
{
	string img;
	SDL_Texture* texture;
	SDL_FRect rect; //dst rect
};

struct DrawableWithSrc : public Drawable
{
	SDL_Rect srcRect;
};

struct FDrawableWithSrc : public FDrawable
{
	SDL_FRect srcRect;
};

struct Camera_Rect
{
	float x, y, w, h;
};

enum SOUND
{
	BACKGROUND_MUSIC,
	X_PLACE,
	O_PLACE,
};

enum GAME_STATE
{
	NONE = 0,
	TITLE_SCREEN = 1,
	GAME = 2,
	WIN_SCREEN = 3,
};

enum COLOR
{
	WHITE = 1,
	BLACK = 2,
	RED = 3,
	ORANGE = 4,
};

enum TILE_TYPE
{
	NONE_TYPE = 0,
	AIR = 1,
	GRASSBLOCK = 2,
};

inline istream& operator>>(istream& is, SDL_Rect& rect)
{
	is >> rect.x >> rect.y >> rect.w >> rect.h;
	return is;
}

inline ostream& operator<<(ostream& os, SDL_Rect& rect)
{
	os << rect.x << " " << rect.y << " " << rect.w << " " << rect.h;
	return os;
}

inline istream& operator>>(istream& is, SDL_FRect& rect)
{
	is >> rect.x >> rect.y >> rect.w >> rect.h;
	return is;
}

inline ostream& operator>>(ostream& is, SDL_FRect& rect)
{
	is << rect.x << rect.y << rect.w << rect.h;
	return is;
} 

inline istream& operator>>(istream& is, int2& vec)
{
	is >> vec.x >> vec.y;
	return is;
}

inline istream& operator>>(istream& is, float2& vec)
{
	is >> vec.x >> vec.y;
	return is;
}

inline istream& operator>>(istream& is, Drawable& dr)
{
	is >> dr.img >> dr.rect;
	return is;
}

inline ostream& operator<<(ostream& os, int2& vec)
{
	os << vec.x << " " << vec.y;
	return os;
}

inline ostream& operator<<(ostream& os, const SDL_Rect& v) {
	os << "(" << v.x << ", " << v.y << ", " << v.w << ", " << v.h << ")";
	return os;
}

inline float2 operator+(float a, const float2& v) {
	return { a + v.x, a + v.y };
}
inline float2 operator-(float a, const float2& v) {
	return { a - v.x, a - v.y };
}
inline float2 operator*(float a, const float2& v) {
	return { a * v.x, a * v.y };
}
inline float2 operator/(float a, const float2& v) {
	return { a / v.x, a / v.y };
}

inline bool operator==(const float2& v, float value) {
	return v.x == value && v.y == value;
}
inline bool operator==(float value, const float2& v) {
	return v == value && v == value;
}