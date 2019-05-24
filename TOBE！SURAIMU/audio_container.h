#pragma once
//========================================
//CreatorName:YamadaHaruhisa

class AudioContainer
{
public:
	AudioContainer( std::wstring& FileName );
	~AudioContainer() = default;

	//定数クラス
public:
	//再生モード
	enum class Mode
	{
		kDefault,
		kOneShot,
	};

	//外部公開関数
public:
	//エンジンの更新
	void engineUpdate();
	//再生
	//引数...再生モード
	void play( Mode PlayMode , bool LoopFlag = false );
	//停止
	void stop();
	//一時停止
	void pause();
	//再開
	void resume();
	//パンの設定
	//引数...パンの量( -1.0～1.0 )
	void setPan( float Pan );
	//パンのリセット
	void resetPan();
	//ピッチの設定
	//引数...ピッチの量( -1.0～1.0 )
	void setPitch(float Pitch);
	//ピッチのリセット
	void resetPitch();
	//音量の設定
	//引数...音量( 0.0～ )
	void setVolume( float Volume );
	//音量のリセット
	void resetVolume();
	//すべてのステータスのリセット
	void allReset();

	//メンバ変数
private:
	AUDIO_ENGINE_FLAGS flags_;								//エンジンのフラグ
	std::unique_ptr<AudioEngine> engine_;				    //オーディオエンジン
	std::unique_ptr<SoundEffect> effect_;		            //サウンドエフェクト
	std::unique_ptr<SoundEffectInstance> effect_insance_;	//サウンドエフェクトのインスタンス
};

