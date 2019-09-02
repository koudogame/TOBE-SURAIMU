
#include "view_background_base.h"

#include "sprite.h"


/*===========================================================================*/
// カラーの設定
void ViewBase::setColor( const Color Color ) 
{
    BackObjectBase::setColor( Color );

    // 切り取り範囲を変更
    const long kWidth = trimming_.right - trimming_.left;
    trimming_.left = Color * kWidth;
    trimming_.right = trimming_.left + kWidth;
}


/*===========================================================================*/
// 描画( シームレス )
void ViewBase::drawSeamless( const float Depth, const float Alpha )
{
    Vector2 draw_position = position_;

    // シームレスに描画するため描画座標を画面外へ出す
    const float kHeight = trimming_.bottom - trimming_.top;
    while( draw_position.y > 0.0F )
    {
        draw_position.y -= kHeight;
    }


    // 画面が埋まるまで描画
    Sprite* const kSprite = Sprite::getInstance();
    for( ;; draw_position.y += kHeight )
    {
        kSprite->reserveDraw(
            texture_,
            draw_position,
            trimming_,
            Alpha,
            Depth
        );

        // 画面が埋まったら描画終了
        if( draw_position.y + kHeight > getWindowHeight<float>() )
        {
            break;
        }
    }
}


/*===========================================================================*/
// スクロール
void ViewBase::scroll( const float Offset )
{
    position_.y += Offset;

    // 画面外へ行ったら縦幅分戻す
    if( position_.y >= getWindowHeight<float>() )
    {
        position_.y -= (trimming_.bottom - trimming_.top);
    }
}
