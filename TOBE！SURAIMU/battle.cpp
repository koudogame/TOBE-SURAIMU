
// 板場 温樹

#include "battle.h"


#include "release.h"
#include "player.h"
#include "wall.h"
#include "star_container.h"


/*===========================================================================*/
constexpr unsigned kPlayerMax = 4U;
constexpr Vector2 kPlayerPosition[kPlayerMax] = {   // プレイヤーの初期位置
    { 0.0F, 0.0F }, // Player 1
    { 0.0F, 0.0F }, // Player 2
    { 0.0F, 0.0F }, // Player 3
    { 0.0F, 0.0F }, // Player 4
};
constexpr float kPlayerJump    = 800.0F;            // プレイヤージャンプ力
constexpr float kPlayerAddVol  = 0.005F;            // プレイヤー増加量
constexpr float kPlayerGravity = 5.0F;              // プレイヤー重力
constexpr float kPlayerSpeed   = 5.0F;              // プレイヤー速さ


/*===========================================================================*/
Battle::Battle()
{

}

Battle::~Battle()
{
    destroy();
}

/*===========================================================================*/
// 初期化処理
bool Battle::init()
{
    destroy();


    // プレイヤー
    for( int i = 0; i < kPlayerMax; ++i )
    {
        player_.push_back( new Player() );

        if( player_.back()->init(
            kPlayerPosition[i], kPlayerJump, kPlayerAddVol,
            kPlayerGravity, kPlayerSpeed
            ) == false )
        {
            return false;
        }
    }

    // 壁
    wall_ = new Wall();
    wall_->init();


    // スター
    star_container_ = new StarContainer();


    created_ = true;
    return true;
}

/*===========================================================================*/
// 終了処理
void Battle::destroy()
{
    if( created_ == false ) { return; }
    created_ = false;


    // 星たちの開放
    star_container_->destroy();     safe_delete(star_container_);

    // 壁の開放
    wall_->destroy();               safe_delete(wall_);

    // プレイヤーたちを開放
    for( auto player : player_ )
    {
        player->destroy();          safe_delete(player);
    }

}

/*===========================================================================*/
// 更新処理
SceneBase* Battle::update()
{


    return this;
}

/*===========================================================================*/
// 描画処理
void Battle::draw()
{

}
