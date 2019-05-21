#pragma once
class AudioContainer
{
public:
	AudioContainer( std::wstring& FileName );
	~AudioContainer() = default;

public:
	enum class Mode
	{
		kDefault,
		kOneShot,
	};

public:
	void engineUpdate();
	void play( Mode PlayMode );
	void stop();
	void pause();
	void resume();
	void setPan( float Pan );
	void resetPan();
	void setPitch(float Pitch);
	void resetPitch();
	void setVolume( float Volume );
	void resetVolume();
	void allReset();

private:
	AUDIO_ENGINE_FLAGS flags_;
	std::unique_ptr<AudioEngine> engine_;
	std::unique_ptr<SoundEffect> effect_;
	std::unique_ptr<SoundEffectInstance> effect_insance_;
};

