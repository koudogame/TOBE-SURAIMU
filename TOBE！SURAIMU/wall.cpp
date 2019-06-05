
// 板場 温樹

#include "wall.h"

#include "release.h"
#include "textureLoder.h"
#include "sprite.h"
#include "task_manager.h"
#include "shape.h"

/*===========================================================================*/
constexpr float kWindowWidth  = getWindowWidth<float>();
constexpr float kWindowHeight = getWindowHeight<float>();

constexpr float kWallWidth        = 32.0F;                        // 壁横幅
constexpr float kWallHeight       = 88.0F;                        // 壁縦幅
constexpr float kWallWidthHarf    = kWallWidth / 2.0F;            // 壁横幅半分
constexpr float kDistanceToCenter = 320.0F;                       // 中央との距離
const float kCenterX              = kWindowWidth / 2.0F;          // 画面中央x座標
const float kPositionXLeft        = kCenterX - kDistanceToCenter; // 左の壁x座標
const float kPositionXRight       = kCenterX + kDistanceToCenter; // 右の壁x座標

const Line kCollisionLeftWall{                                    // 左の壁衝突判定範囲
	Vector2(kPositionXLeft, 0.0F), Vector2(kPositionXLeft, 1000.0F) };
const Line kCollisionRightWall{                                   // 右の壁衝突判定範囲
    Vector2(kPositionXRight, 1000.0F),Vector2(kPositionXRight, 0.0F) };

const float kDrawPositionXLeftWall  = kPositionXLeft - kWallWidthHarf;  // 左の壁描画x座標
const float kDrawPositionXRightWall = kPositionXRight - kWallWidthHarf; // 右の壁描画x座標

constexpr RECT kTrimmingWall { 0L, 0L, 32L, 88L };     // 壁切り取り範囲


/*===========================================================================*/
Wall::Wall()
{}

Wall::~Wall()
{
	destroy();
}

/*===========================================================================*/
// 初期化処理
bool Wall::init()
{
    destroy();
    created_ = true;

	// テクスチャ
	texture_ = TextureLoder::getInstance()->load(L"Texture/wall.png");
	if (texture_ == nullptr) { return false; }

	// 形
	myshape_ = new Line[2]();



	// タスクの登録
    TaskManager* kManager = TaskManager::getInstance();
	kManager->registerTask(this, TaskUpdate::kWallUpdate);
	kManager->registerTask(this, TaskDraw::kObject);

	// 形の設定
	myshape_[0] = kCollisionLeftWall;
	myshape_[1] = kCollisionRightWall;

    // 描画位置( xは使用しない )
    position_.x = 0.0F;
    position_.y = 0.0F;

	return true;
}

/*===========================================================================*/
// 終了処理
void Wall::destroy()
{
    if( created_ == false ) { return; }
    created_ = false;

	// 形
	safe_delete_array(myshape_);

	// テクスチャ
	TextureLoder::getInstance()->release(texture_);

	// タスク
	TaskManager::getInstance()->unregisterObject(this);
}

/*===========================================================================*/
// 更新処理
void Wall::update()
{
    position_.y += 1.3F;
    if( position_.y > 0.0F ) { position_.y -= kWallHeight; }
}

/*===========================================================================*/
// 描画処理
void Wall::draw()
{
	Sprite* const kSprite = Sprite::getInstance();

    Vector2 draw_position;
    draw_position.y = position_.y;

    // 上から下へシームレスに描画
    while (draw_position.y <= kWindowHeight)
    {
        // 左壁
        draw_position.x = kDrawPositionXLeftWall;
		kSprite->draw( texture_ , draw_position , &kTrimmingWall , 1.0F , 1.0F );

        // 右壁( 左右反転 )
        draw_position.x = kDrawPositionXRightWall;
        kSprite->draw(texture_, draw_position, &kTrimmingWall,
            1.0F, 1.0F, Vector2(1.0F, 1.0F), 0.0F, Vector2::Zero,
            SpriteEffects_FlipHorizontally );

        draw_position.y += static_cast<long>(kWallHeight);
    }
}

/*===========================================================================*/
// 形の返却
Line* Wall::getShape(const unsigned GetNum)
{
	// 範囲外の参照は禁止
	assert(GetNum < 2);

	return &myshape_[GetNum];
}
