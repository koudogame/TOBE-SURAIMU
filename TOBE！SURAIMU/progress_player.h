
// 板場

#pragma once

class Player;

//-----------------------------------------------------------------------------
// 進行度用プレイヤークラス
//-----------------------------------------------------------------------------
// --説明--
// スクロールした際もきちんと座標移動が行えるよう、スクロール値を定数で使用している。
// もしスクロールの値を変えることになった時、不具合が起きたらそういうこと
class ProgressPlayer
{
public:
    ProgressPlayer();
    ~ProgressPlayer();


    bool init( Player* const Player,
               const Vector2& Position,
               const float StartLine, const float EndLine,
               const float Magnification );
    void destroy();
    void update();
    void draw();

    void setMove( const float Distance );


    void move(const Vector2& Distance) { position_ += Distance; }
    const Vector2& getPosition() const                          { return position_; }
    void           setPosition( const Vector2& Destination )    { position_ = Destination; }


private:
    float getMovementDelta();


    Player  *player_ = nullptr;

    ID3D11ShaderResourceView* texture_ = nullptr;
    Vector2 position_;
    float   start_line_ = 0.0F;
    float   end_line_   = 0.0F;
    float   magnification_ = 0.0F;
    float   last_position_y_ = 0.0F;
};
