#include "Sound.h"

bool Sound::init()
{

	AUDIO_ENGINE_FLAGS flags = AudioEngine_Default;
#ifdef _DEBUG
	flags = flags | AudioEngine_Debug; // Debug‹@”\—LŒø
#endif

	engine = std::make_unique<AudioEngine>( flags );
	wb = std::make_unique<WaveBank>( engine.get() , L"Sound/wavebank.xwb" );

	if( engine == nullptr || wb == nullptr )
		return false;

	for( int i = 0; i < XACT_WAVEBANK_WAVEBANK_ENTRY_COUNT; i++ )
	{
		sound_list_.push_back( wb->CreateInstance( i ) );
	}

	return true;
}

void Sound::update()
{
	engine.get()->Update();
}

void Sound::play( int ID , bool LoopFlag )
{
	sound_list_[ ID ]->Play( LoopFlag );
}

void Sound::stop( int ID )
{
	sound_list_[ ID ]->Stop();
}

void Sound::pause( int ID )
{
	sound_list_[ ID ]->Pause();
}

void Sound::resume( int ID )
{
	sound_list_[ ID ]->Resume();
}

void Sound::setPan( int ID , float Pan )
{
	sound_list_[ ID ]->SetPan( Pan );
}

void Sound::setPitch( int ID , float Pitch )
{
	sound_list_[ ID ]->SetPitch( Pitch );
}

void Sound::setVolume( int ID , float Volume )
{
	sound_list_[ ID ]->SetVolume( Volume );
}
