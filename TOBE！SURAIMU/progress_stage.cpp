
// 板場

#include "progress_stage.h"

#include "textureLoder.h"
#include "sprite.h"


// 定数
/*===========================================================================*/
static constexpr wchar_t kTextureFile[] = L"Texture/progress.png";
static constexpr RECT    kTrimming = { 0L, 0L, 70L, 2160L };
static constexpr float   kDrawDepth = 10.0F;
static constexpr float   kOneStageHeight = 720.0F;


// Ctor, Dtor
/*===========================================================================*/
ProgressStage::ProgressStage()
{

}

ProgressStage::~ProgressStage()
{
    destroy();
}


/*===========================================================================*/
// 初期化処理
bool ProgressStage::init( const Vector2& Position )
{
    // テクスチャ読み込み
    if( texture_ == nullptr )
    {
        texture_ = TextureLoder::getInstance()->load( kTextureFile );
        if( texture_ == nullptr ) { return false; }     // エラー 
    }

    // 引数の値をメンバにセット
    position_ = Position;


    return true;
}

/*===========================================================================*/
// 終了処理
void ProgressStage::destroy()
{
    // テクスチャ開放
    if( texture_ != nullptr )
    {
        TextureLoder::getInstance()->release( texture_ );
        texture_ = nullptr;
    }
}

/*===========================================================================*/
// 更新処理
void ProgressStage::update()
{
    if( position_.y > getWindowHeight<float>() )
    {
        position_.y -= kOneStageHeight * 3.0F;
    }
}

/*===========================================================================*/
// 描画処理
void ProgressStage::draw( const Vector2& Offset )
{
    Sprite* const kSprite = Sprite::getInstance();

    Vector2 draw_position = position_ + Offset;

    kSprite->reserveDraw(
        texture_,
        position_ + Offset,
        kTrimming,
        1.0F, // alpha
        kDrawDepth
    );

    // 上が空白なら連ねて描画
    if( position_.y > 0.0F )
    {
        draw_position.y -= kOneStageHeight * 3.0F;

        kSprite->reserveDraw(
            texture_,
            draw_position,
            kTrimming,
            1.0F, // alpha
            kDrawDepth
        );
    }
    // 下が空白( ゲーム開始直後に下へ落下した場合のみ )
    else if( position_.y + (kOneStageHeight * 3.0F) < getWindowHeight<float>() )
    {
        draw_position.x -= 1.0F;
        draw_position.y += kOneStageHeight * 3.0F;
        kSprite->reserveDraw(
            texture_,
            draw_position,
            {210L, 0L, 280L, 700L },
            1.0F,
            kDrawDepth
        );

    }
}
