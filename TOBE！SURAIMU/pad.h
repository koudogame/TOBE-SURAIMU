#pragma once


class Pad
{
private:
	Pad() = default;
    GamePad pad_;                // ゲームパッドクラス
    GamePad::State state_[4];    // 入力格納構造体
    GamePad::ButtonStateTracker tracker_[4]; // パッドトラッカー

    bool flag_;                  // 振動フラグ
    float second_;               // 振動継続時間
    float count_;                // 振動時間カウント
    float vib_right_;            // 右振動値
    float vib_left_;             // 左振動地

	int input_no_;               // 入力状態

public:
	static Pad* getInstance() { static Pad instance; return &instance; }
    void update();   // 更新処理

    // 入力構造体を返す
    const GamePad::State& getState(int PlayerNum = 0) { return state_[PlayerNum]; };
    // トラッカー返却
    const GamePad::ButtonStateTracker& getTracker(int PlayerNum = 0) { return tracker_[PlayerNum]; }

    // 振動
    void Vibration( float second, float Right, float Left );
};
