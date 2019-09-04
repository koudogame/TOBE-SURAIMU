
// 板場

#include "progress_stage.h"

#include "textureLoder.h"
#include "sprite.h"


// 定数
/*===========================================================================*/
static constexpr wchar_t kTextureFile[] = L"Texture/progress.png";
static constexpr RECT    kTrimming = { 0L, 0L, 70L, 2160L };
static constexpr float   kDrawDepth = 10.0F;


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

}

/*===========================================================================*/
// 描画処理
void ProgressStage::draw()
{
    Sprite::getInstance()->reserveDraw(
        texture_,
        position_,
        kTrimming,
        1.0F, // alpha
        kDrawDepth
    );
}
