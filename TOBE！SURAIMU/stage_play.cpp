
// 板場

#include "stage_play.h"

#include "release.h"
#include "textureLoder.h"

#include "stage.h"

// 定数
/*===========================================================================*/
// プレイヤー
static constexpr unsigned kGamePad = 0;
static constexpr float kMovableRangeTop    = getWindowHeight<float>() * 0.3F;
static constexpr float kMovableRangeBottom = getWindowHeight<float>() * 0.85F;


// Ctor, Dtor
/*===========================================================================*/
StagePlay::StagePlay( const std::wstring& StageDataName )
{
    stage_data_name_ = StageDataName;
}

StagePlay::~StagePlay()
{
}


/*===========================================================================*/
// 初期化処理
bool StagePlay::init()
{
    //ステージ初期化
    if( stage_ == nullptr ) { stage_ = new Stage(); }
//    if( stage_->init(stage_data_name_) == false ) { return false; }


    return true;
}


/*===========================================================================*/
// 終了処理
// init関数で生成、初期化した逆順に開放を行う
void StagePlay::destroy()
{
    // ステージ開放
    if( stage_ != nullptr )
    {
        stage_->destroy();
        safe_delete( stage_ );
    }
}


/*===========================================================================*/
// 更新処理
SceneBase* StagePlay::update()
{
    
    stage_->update();

    return this;
}


/*===========================================================================*/
// 描画処理
// オブジェクトの描画は、基本的にSpriteクラスが行う
void StagePlay::draw()
{
    stage_->draw();
}
