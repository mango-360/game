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

static const int TILE_SIZE = 32;

struct float2
{
	float x, y;

	void reset() { x = 0; y = 0; }

	void set(float2 a) { x = a.x; y = a.y; }
	void operator=(float2 a) { x = a.x; y = a.y; }

	bool operator==(float2 a) { return (x == a.x && y == a.y); }
	bool operator!=(float2 a) { return (x != a.x || y != a.y); }

	float2 operator+(float2 a) { return { x + a.x, y + a.y }; }
	float2 operator-(float2 a) { return { x - a.x, y - a.y }; }
	float2 operator*(float2 a) { return { x * a.x, y * a.y }; }
	float2 operator/(float2 a) { return { x / a.x, y / a.y }; }

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
	SDL_Texture* texture;
	SDL_Rect rect; //dst rect
};

struct DrawableWithSrc : public Drawable
{
	SDL_Rect srcRect;
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

enum Tile
{
	AIR = 0,
	GRASSBLOCK = 1,
};