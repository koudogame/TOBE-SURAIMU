
// 板場

#include "stage_play.h"

#include "release.h"

#include "stage_data.h"
#include "player.h"
#include "star_container.h"
#include "wall.h"
#include "fail_wall.h"

// 定数
/*===========================================================================*/
// プレイヤー
static constexpr Vector2 kPlayerInitPosition { 640.0F, 565.0F };
static constexpr unsigned kPlayerNo = 0;


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
    // ステージデータの読み込み
    if( data_ == nullptr ) { data_ = new StageData(); }
    if( data_->load( stage_data_name_ ) == false ) { return false; }

    // プレイヤー初期化
    if( player_ == nullptr ) { player_ = new Player(); }
    if( player_->init(kPlayerInitPosition, kPlayerNo) == false ) { return false; }

    // スターコンテナ初期化
    if( stars_ == nullptr ) { stars_ = new StarContainer(); }
    if( stars_->createStar(data_->star_pattern_file_name) == false ) { return false; }

    // 壁初期化
    if( wall_ == nullptr ) { wall_ = new Wall(); }
    if( wall_->init() == false ) { return false; }

    // 炎初期化
    if( fire_ == nullptr ) { fire_ = new FailWall(); }
    if( fire_->init() == false ) { return false; }

    return true;
}


/*===========================================================================*/
// 終了処理
// init関数で生成、初期化した逆順に開放を行う
void StagePlay::destroy()
{
    // 炎開放
    if( fire_ != nullptr )
    {
        fire_->destroy();
        safe_delete( fire_ );
    }

    // 壁開放
    if( wall_ != nullptr )
    {
        wall_->destroy();
        safe_delete( wall_ );
    }

    // スターコンテナ開放
    if( stars_ != nullptr )
    {
        stars_->destroy();
        safe_delete( stars_ );
    }

    // プレイヤー開放
    if( player_ != nullptr )
    {
        player_->destroy();
        safe_delete( player_ );
    }

    // ステージデータ開放
    if( data_ != nullptr )
    {
        safe_delete( data_ );
    }
}


/*===========================================================================*/
// 更新処理
SceneBase* StagePlay::update()
{
    return this;
}


/*===========================================================================*/
// 描画処理
void StagePlay::draw()
{

}
