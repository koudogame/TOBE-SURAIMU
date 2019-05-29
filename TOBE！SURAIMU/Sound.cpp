#include "Sound.h"

bool Sound::init()
{
	engine = std::make_unique<AudioEngine>();
	wb = std::make_unique<WaveBank>( engine , L"Sound/wavebank.xwb" );

	if( engine == nullptr || wb == nullptr )
		return false;

	return true;
}

void Sound::update()
{
	engine.get()->Update();
}
