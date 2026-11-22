#include "SoundManager.h"

Mix_Chunk* SoundManager::m_backgroundMusic = nullptr;
Mix_Chunk* SoundManager::m_itemPickUp = nullptr;

SoundManager::SoundManager()
{
}

SoundManager::~SoundManager()
{
}

void SoundManager::init()
{
	fstream stream;

	string tmp, backgroundMusic, itemPickUp;

	stream.open(CONFIG_FOLDER + "soundManager.txt");

	stream >> tmp >> backgroundMusic;
	stream >> tmp >> itemPickUp;

	stream.close();

	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048) < 0)
	{
		cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << endl;
		return;
	}

	m_backgroundMusic = Mix_LoadWAV((SOUND_FOLDER + backgroundMusic).c_str());
	m_itemPickUp = Mix_LoadWAV((SOUND_FOLDER + itemPickUp).c_str());

	Mix_AllocateChannels(2);

	playSound(BACKGROUND_MUSIC);
}

void SoundManager::playSound(SOUND sound)
{
	switch (sound)
	{
	case BACKGROUND_MUSIC:
		Mix_PlayChannel(0, m_backgroundMusic, -1);

		Mix_Volume(0, 1);
		break;
	case ITEM_PICK_UP:
		Mix_PlayChannel(1, m_itemPickUp, 0);

		Mix_Volume(1, 25);
		break;
	default:
		cout << "No such sound!" << endl;
		break;
	}
}

void SoundManager::destroy()
{
	Mix_FreeChunk(m_backgroundMusic);
	Mix_FreeChunk(m_itemPickUp);

	m_backgroundMusic = nullptr;
	m_itemPickUp = nullptr;
}
