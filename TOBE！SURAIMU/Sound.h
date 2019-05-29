#pragma once
#include "Sound/xwb.h"

class Sound
{
	//singleton
private:
	Sound() = default;
public:
	static inline Sound* getinstance() { static Sound instance; return &instance; }

public:
	~Sound() = default;

public:
	bool init();
	void update();

	//ÉTÉEÉìÉhêßå‰ä÷êî
public:
	void play( int ID , bool LoopFlag );
	void stop( int ID );
	void pause( int ID );
	void resume( int ID );
	void setPan( int ID , float Pan );
	void setPitch( int ID , float Pitch );
	void setVolume( int ID , float Volume );

private:
	std::unique_ptr<AudioEngine> engine;
	std::unique_ptr<WaveBank> wb;
	std::vector<std::unique_ptr<SoundEffectInstance>> sound_list_;
};

#define SOUND Sound::getinstance()
