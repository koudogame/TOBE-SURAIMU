
// 板場

#pragma once

class Player;
class FailWall;
class ProgressPlayer;

//-----------------------------------------------------------------------------
// 進行度用炎クラス
//-----------------------------------------------------------------------------
// --説明--
// プレイヤーとの座標の差を使って、自身の座標を決定している
// なのでsetMove, setPositionなどの座標移動関数はない
class ProgressFailWall
{
public:
    ProgressFailWall();
    ~ProgressFailWall();

    bool init( FailWall* const FailWall,
               Player* const Player,
               ProgressPlayer* const ProgressPlayer,
               const Vector2& Position,
               const float Magnification );
    void destroy();
    void update();
    void draw();



private:
    float getDistanceProgressPlayer() const;

    Player   *player_ = nullptr;;
    FailWall *fail_wall_ = nullptr;
    ProgressPlayer *progress_player_ = nullptr;

    ID3D11ShaderResourceView* texture_ = nullptr;
    Vector2 position_;
    float magnification_ = 0.0F;
};
