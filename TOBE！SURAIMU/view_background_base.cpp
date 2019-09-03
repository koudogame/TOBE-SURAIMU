
// 板場

#include "view_background_base.h"

#include "sprite.h"

/*===========================================================================*/
// 描画( シームレス )
void ViewBase::draw( const RECT& Trimming,
                     const float Depth, 
                     const float Alpha )
{
    if( position_.y < getWindowHeight<float>() )
    {
        Sprite::getInstance()->reserveDraw(
            texture_,
            position_,
            Trimming,
            Alpha,
            Depth
        );
    }
}


/*===========================================================================*/
// スクロール
void ViewBase::scroll( const float Offset )
{
    position_.y += Offset;

    // 画面外へ行ったら、見えない範囲で戻す
    if( position_.y > getWindowHeight<float>() )
    {
        position_.y = getWindowHeight<float>();
    }
}
