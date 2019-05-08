#pragma once


using namespace DirectX;

class Key
{
private:
	Key() = default;
    Keyboard keyboard_;     // Keyboardインターフェイス

    Keyboard::State state_;
    Keyboard::KeyboardStateTracker tracker_;

public:
	static Key* getInstance() { static Key instance; return &instance; }
    void update();			// キー入力更新

    // 入力状態返却
    const Keyboard::State& getState()  { return state_; }
    // キートラッカー返却
    const Keyboard::KeyboardStateTracker& getTracker() { return tracker_; }
};