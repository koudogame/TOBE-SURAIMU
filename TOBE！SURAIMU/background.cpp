
// 板場 温樹

#include "background.h"

#include "textureLoder.h"
#include "sprite.h"
#include "task_manager.h"

const long kTextureSize = 1024L;

/*===========================================================================*/
Background::Background()
{
}

Background::~Background()
{
}

/*===========================================================================*/
// 初期化処理
// Trimming : 画像切り取り範囲
// Scroll   : y座標の1フレームでのスクロール量
// Depth    : Sprite深度値
bool Background::init(const RECT& Trimming, const float Scroll, const float Depth)
{
    // テクスチャ読み込み
    texture_ = TextureLoder::getInstance()->load(L"Texture/background.png");
    if (texture_ == nullptr) { return false; }


    // タスクの登録
    TaskManager::getInstance()->registerTask(this, TaskUpdate::kBackgroundUpdate);
    TaskManager::getInstance()->registerTask(this, TaskDraw::kBackground);


    // メンバ
    position_.x = (getWindowWidth<float>() - static_cast<float>(kTextureSize))
                  / 2.0F;
    position_.y = 0.0F;
    trimming_ = Trimming;
    magnification_ = 1.0F;
    scroll_ = Scroll;

    return true;
}

/*===========================================================================*/
// 終了処理
void Background::destroy()
{
    TaskManager::getInstance()->unregisterObject(this);

    TextureLoder::getInstance()->release(texture_);
}

/*===========================================================================*/
// 更新処理
void Background::update()
{
    position_.y += scroll_ * magnification_;

    // y座標が画面外( 下 )へ行ったら上へ戻す
    if (position_.y > getWindowHeight<float>())
    {
        position_.y = getWindowHeight<float>() - kTextureSize + scroll_;
    }
}

/*===========================================================================*/
// 描画処理
void Background::draw()
{
    Sprite* const kSprite = Sprite::getInstance();


    // 描画モード"加算合成"
    kSprite->end();
    kSprite->begin(kSprite->chengeMode());
    Vector2 draw_position = position_;

    // 画面が埋まるまで縦に描画
    while (draw_position.y + kTextureSize > 0.0F)
    {
        kSprite->draw(texture_, draw_position, &trimming_, 1.0F, depth_);

        draw_position.y -= static_cast<float>(kTextureSize);
    }
    kSprite->end();
    kSprite->begin();
    // ！描画モード"加算合成"
}
