#include "audio_container.h"

//コンストラクタ
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

//AudioEngineの更新
void AudioContainer::engineUpdate()
{
	engine_.get()->Update();
}

//再生
void AudioContainer::play( Mode PlayMode,bool LoopFlag )
{
	if( PlayMode == Mode::kDefault )
		effect_insance_->Play( LoopFlag );
	else if( PlayMode == Mode::kOneShot )
		effect_->Play();
}

//停止
void AudioContainer::stop()
{
	effect_insance_->Stop();
}

//一時停止
void AudioContainer::pause()
{
	effect_insance_->Pause();
}

//再開
void AudioContainer::resume()
{
	effect_insance_->Resume();
}

//パンの設定
void AudioContainer::setPan( float Pan )
{
	if( Pan > 1.0F || Pan < -1.0F )
		return;

	effect_insance_->SetPan( Pan );
}

//パンをリセット
void AudioContainer::resetPan()
{
	effect_insance_->SetPan( 0.0F );
}

//ピッチの設定
void AudioContainer::setPitch( float Pitch )
{
	if( Pitch > 1.0F || Pitch < -1.0F )
		return;

	effect_insance_->SetPitch( Pitch );
}

//ピッチのリセット
void AudioContainer::resetPitch()
{
	effect_insance_->SetPitch( 0.0F );
}

//音量の設定
void AudioContainer::setVolume( float Volume )
{
	if( Volume < 0.0F )
		return;

	effect_insance_->SetVolume( Volume );
}

//音量のリセット
void AudioContainer::resetVolume()
{
	effect_insance_->SetVolume( 1.0F );
}

//すべてのステータスのリセット
void AudioContainer::allReset()
{
	resetPan();
	resetPitch();
	resetVolume();
}
