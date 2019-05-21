#pragma once
#include "audio_container.h"

class AudioLoader
{
	//singleton
private:
	AudioLoader();
public:
	static AudioLoader* getInstance() { static AudioLoader instance; return &instance; }

	//デストラクタ
public:
	~AudioLoader() = default;

	//外部公開関数
public:
	void allUpdate();
	AudioContainer* getSound( std::wstring FileName );
	void desetroySound( std::wstring FileName );
	void allDestroy();

	//メンバ変数
private:
	std::unordered_map<std::wstring , std::unique_ptr<AudioContainer>> container_map_;
};

