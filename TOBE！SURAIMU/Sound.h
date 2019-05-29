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
	inline std::unique_ptr<SoundEffectInstance> getSound( XACT_WAVEBANK_WAVEBANK ID ) { return wb.get()->CreateInstance( ID ); }

private:
	std::unique_ptr<AudioEngine> engine;
	std::unique_ptr<WaveBank> wb;
};

#define SOUND Sound::getinstance()
