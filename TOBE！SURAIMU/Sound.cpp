#include "Sound.h"

//初期化
bool Sound::init()
{

	AUDIO_ENGINE_FLAGS flags = AudioEngine_Default;
#ifdef _DEBUG
	flags = flags | AudioEngine_Debug; // Debug機能有効
#endif
	//エンジンの生成
	engine = std::make_unique<AudioEngine>( flags );
	//ウェーブバンクの作成
	wb = std::make_unique<WaveBank>( engine.get() , L"Sound/wavebank.xwb" );

	if( engine == nullptr || wb == nullptr )
		return false;

	//サウンド配列にサウンドすべて取り込む
	for( int i = 0; i < XACT_WAVEBANK_WAVEBANK_ENTRY_COUNT; i++ )
	{
		sound_list_.push_back( wb->CreateInstance( i ) );
	}

	return true;
}

//エンジンの更新
void Sound::update()
{
	engine.get()->Update();
}

//再生
void Sound::play( int ID , bool LoopFlag )
{
	sound_list_[ ID ]->Play( LoopFlag );

}

//停止
void Sound::stop( int ID )
{
	sound_list_[ ID ]->Stop();
}

//ポーズ
void Sound::pause( int ID )
{
	sound_list_[ ID ]->Pause();
}

//再再生
void Sound::resume( int ID )
{
	sound_list_[ ID ]->Resume();
}

//パンの設定
void Sound::setPan( int ID , float Pan )
{
	sound_list_[ ID ]->SetPan( Pan );
}

//ピッチの設定
void Sound::setPitch( int ID , float Pitch )
{
	sound_list_[ ID ]->SetPitch( Pitch );
}

//音量の設定
void Sound::setVolume( int ID , float Volume )
{
	sound_list_[ ID ]->SetVolume( Volume );
}
