#include "stdafx.h"
#include "audio_container.h"




AudioContainer::AudioContainer(std::wstring& FileName)
{
	flags_ = AUDIO_ENGINE_FLAGS::AudioEngine_Default;
#ifdef _DEBUG
	flags_ = flags_ | AUDIO_ENGINE_FLAGS::AudioEngine_Debug;
#endif
	engine_ = std::make_unique<AudioEngine>(flags_);
	effect_ = std::make_unique<SoundEffect>( engine_.get() , FileName.c_str() );
	effect_insance_ = effect_.get()->CreateInstance();
}

void AudioContainer::engineUpdate()
{
	engine_.get()->Update();
}

void AudioContainer::play( Mode PlayMode )
{
	if( PlayMode == Mode::kDefault )
		effect_insance_->Play();
	else if( PlayMode == Mode::kOneShot )
		effect_->Play();
}

void AudioContainer::stop()
{
	effect_insance_->Stop();
}

void AudioContainer::pause()
{
	effect_insance_->Pause();
}

void AudioContainer::resume()
{
	effect_insance_->Resume();
}

void AudioContainer::setPan( float Pan )
{
	if( Pan > 1.0F || Pan < -1.0F )
		return;

	effect_insance_->SetPan( Pan );
}

void AudioContainer::resetPan()
{
	effect_insance_->SetPan( 0.0F );
}

void AudioContainer::setPitch( float Pitch )
{
	if( Pitch > 1.0F || Pitch < -1.0F )
		return;

	effect_insance_->SetPitch( Pitch );
}

void AudioContainer::resetPitch()
{
	effect_insance_->SetPitch( 0.0F );
}

void AudioContainer::setVolume( float Volume )
{
	if( Volume < 0.0F )
		return;

	effect_insance_->SetVolume( Volume );
}

void AudioContainer::resetVolume()
{
	effect_insance_->SetVolume( 1.0F );
}

void AudioContainer::allReset()
{
	resetPan();
	resetPitch();
	resetVolume();
}
