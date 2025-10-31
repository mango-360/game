#include "SoundManager.h"

Mix_Chunk* SoundManager::m_backgroundMusic = nullptr;
Mix_Chunk* SoundManager::m_xPlace = nullptr;
Mix_Chunk* SoundManager::m_oPlace = nullptr;

SoundManager::SoundManager()
{
}

SoundManager::~SoundManager()
{
}

void SoundManager::init()
{
	fstream stream;

	string tmp, backgroundMusic, xPlace, oPlace;

	stream.open(CONFIG_FOLDER + "soundManager.txt");

	stream >> tmp >> backgroundMusic;
	stream >> tmp >> xPlace;
	stream >> tmp >> oPlace;

	stream.close();

	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048) < 0)
	{
		cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << endl;
		return;
	}

	m_backgroundMusic = Mix_LoadWAV((SOUND_FOLDER + backgroundMusic).c_str());
	m_xPlace = Mix_LoadWAV((SOUND_FOLDER + xPlace).c_str());
	m_oPlace = Mix_LoadWAV((SOUND_FOLDER + oPlace).c_str());

	Mix_AllocateChannels(3);

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
	case X_PLACE:
		Mix_PlayChannel(1, m_xPlace, 0);

		Mix_Volume(1, 50);
		break;
	case O_PLACE:
		Mix_PlayChannel(2, m_oPlace, 0);

		Mix_Volume(2, 50);
		break;
	default:
		cout << "No such sound!" << endl;
		break;
	}
}

void SoundManager::destroy()
{
	Mix_FreeChunk(m_backgroundMusic);
	Mix_FreeChunk(m_xPlace);
	Mix_FreeChunk(m_oPlace);

	m_backgroundMusic = nullptr;
	m_xPlace = nullptr;
	m_oPlace = nullptr;
}
