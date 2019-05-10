
#include "wall.h"

#include "release.h"
#include "textureLoder.h"
#include "sprite.h"
#include "task_manager.h"
#include "shape.h"


const Line kPositionLeftLine{
	Vector2(300.F, 0.0F), Vector2(300.0F, 720.0F)
};
const Line kPositionRightLine{
	Vector2(950.0F, 0.0F), Vector2(950.0F, 720.0F)
};


const RECT kTrimmingLeftWall{
	1280L, 0L, 1310L, 720L
};
const RECT kTrimmingRightWall{
	1310L, 0L, 1340L, 720L
};

/*===========================================================================*/
Wall::Wall(TaskManager* const TaskManager) :
	ObjectBase(ObjectID::kWall, TaskManager)
{
}

Wall::~Wall()
{
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

	myshape_[0] = kPositionLeftLine;
	myshape_[1] = kPositionRightLine;


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

	// 右
	kSprite->draw(texture_, kPositionLeftLine.start, &kTrimmingLeftWall);
	// 左
	kSprite->draw(texture_, kPositionRightLine.start, &kTrimmingRightWall);
}

/*===========================================================================*/
// 形の返却
const Line& Wall::getShape(const unsigned GetNum)
{
	// 範囲外の参照は禁止
	assert(GetNum < 2);

	return myshape_[GetNum];
}
