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
	//初期化
	bool init();
	//更新
	void update();

	//サウンド制御関数
public:
	//再生
	//引数...音源ID：ループフラグ
	void play( int ID , bool LoopFlag );
	//停止
	//引数...音源ID
	void stop( int ID );
	//ポーズ
	//引数...音源ID
	void pause( int ID );
	//再々生
	//引数...音源ID
	void resume( int ID );
	//パンの設定
	//引数...音源ID：パンの値
	void setPan( int ID , float Pan );
	//ピッチの設定
	//引数...音源ID：ピッチの値
	void setPitch( int ID , float Pitch );
	//音量の設定
	//引数...音源ID：音量の値
	void setVolume( int ID , float Volume );

private:
	std::unique_ptr<AudioEngine> engine;				//エンジン
	std::unique_ptr<WaveBank> wb;			//ウェーブバンク
	std::vector<std::unique_ptr<SoundEffectInstance>> sound_list_;	//サウンド配列
};

#define SOUND Sound::getinstance()
