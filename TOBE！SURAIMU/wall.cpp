
// 板場 温樹

#include "wall.h"

#include "release.h"
#include "textureLoder.h"
#include "sprite.h"
#include "task_manager.h"
#include "shape.h"

/*===========================================================================*/
constexpr float kWallWidth = 30.0F;                     // 壁横幅
constexpr float kWallHeight = 720.0F;                   // 壁縦幅
constexpr float kWallWidthHarf = kWallWidth / 2.0F;     // 壁横幅半分
constexpr float kDistanceToCenter = 320.0F;             // 中央との距離
const float kCenterX = getWindowWidth<float>() / 2.0F;  // 画面中央x座標
const float kPositionXLeft  = kCenterX - kDistanceToCenter; // 左の壁x座標
const float kPositionXRight = kCenterX + kDistanceToCenter; // 右の壁x座標

const Line kCollisionLeftWall{                              // 左の壁衝突判定範囲
	Vector2(kPositionXLeft, 0.0F), Vector2(kPositionXLeft, 2000.0F) };
const Line kCollisionRightWall{                             // 右の壁衝突判定範囲
	Vector2( kPositionXRight, 2000.0F ),Vector2( kPositionXRight, 0.0F ) };

const float kDrawPositionXLeftWall = kPositionXLeft - kWallWidthHarf;   // 左の壁描画x座標
const float kDrawPositionXRightWall = kPositionXRight - kWallWidthHarf; // 右の壁描画x座標

constexpr RECT kTrimmingLeftWall { 1280L, 0L, 1310L, 720L };// 左の壁切り取り範囲
constexpr RECT kTrimmingRightWall{ 1310L, 0L, 1340L, 720L };// 右の壁切り取り範囲



/*===========================================================================*/
Wall::Wall(TaskManager* const TaskManager) :
	ObjectBase(ObjectID::kWall, TaskManager)
{
}

Wall::~Wall()
{
	// 開放忘れ対処
	if (do_create_ == false)
	{
		destroy();
	}
}

/*===========================================================================*/
// 初期化処理
bool Wall::init()
{
	// 生成
	if (do_create_ && create() == false) { return false; }

	// タスクの登録
	task_manager_->registerTask(this, TaskUpdate::kWallUpdate);
	task_manager_->registerTask(this, TaskDraw::kWallDraw);

	// 形の設定
	myshape_[0] = kCollisionLeftWall;
	myshape_[1] = kCollisionRightWall;

    // 描画位置( xは使用しない )
    position_.x = 0.0F;
    position_.y = 0.0F;

	return true;
}
// 生成処理
bool Wall::create()
{
	do_create_ = false;


	// テクスチャ
	texture_ = TextureLoder::getInstance()->load(L"Texture/プレイ画面.png");
	if (texture_ == nullptr) { return false; }

	// 形
	myshape_ = new (std::nothrow) Line[2]();
	if (myshape_ == nullptr) { return false; }

	return true;
}

/*===========================================================================*/
// 終了処理
void Wall::destroy()
{
	do_create_ = true;


	// 形
	safe_delete_array(myshape_);

	// テクスチャ
	TextureLoder::getInstance()->release(texture_);

	// タスクの登録
	task_manager_->unregisterObject(this);
}

/*===========================================================================*/
// 更新処理
void Wall::update()
{

}

/*===========================================================================*/
// 描画処理
void Wall::draw()
{
	Sprite* const kSprite = Sprite::getInstance();

    Vector2 draw_position = position_;
    while (draw_position.y + kWallHeight > 0.0F)
    {
        // 左壁
        draw_position.x = kDrawPositionXLeftWall;
        kSprite->draw(texture_, draw_position, &kTrimmingLeftWall);

        // 右壁
        draw_position.x = kDrawPositionXRightWall;
        kSprite->draw(texture_, draw_position, &kTrimmingRightWall);

        draw_position.y -= kWallHeight;
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
