
// 板場 温樹

#include "battle.h"

#include "pad.h"
#include "release.h"
#include "space.h"
#include "player.h"
#include "wall.h"
#include "star_container.h"


/*===========================================================================*/
constexpr unsigned kPlayerMax = 4U;
constexpr Vector2 kPlayerPosition[kPlayerMax] = {   // プレイヤーの初期位置
    { 350.0F, 0.0F }, // Player 1
    { 500.0F, 0.0F }, // Player 2
    { 650.0F, 0.0F }, // Player 3
    { 800.0F, 0.0F }, // Player 4
};


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

    Pad* pad = Pad::getInstance();
    Space::getInstance();


    // プレイヤー
    for( int i = 0; i < kPlayerMax && pad->getState(i).connected; ++i )
    {
        player_.push_back( new Player() );

        if( player_.back()->init(kPlayerPosition[i], i) == false )
        {
            return false;
        }
    }

    // 壁
    wall_ = new Wall();
    wall_->init();


    // スター
    star_container_ = new StarContainer();
    star_container_->addStar(
        { 640.0F, 650.0F },
        90.0F, 5.0F, 0.2F, 120 );


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
    for( auto& player : player_ )
    {
        player->destroy();          safe_delete(player);
    }

}

/*===========================================================================*/
// 更新処理
SceneBase* Battle::update()
{
    //Space::getInstance()->collision();

    return this;
}

/*===========================================================================*/
// 描画処理
void Battle::draw()
{

}
