
// 板場 温樹

#include "battle.h"

#include "csvLoader.h"
#include "pad.h"
#include "release.h"
#include "space.h"
#include "player.h"
#include "wall.h"
#include "star_container.h"

#include "ai_mover.h"


/*===========================================================================*/
constexpr unsigned kPlayerMax = 4U;
constexpr Vector2 kPlayerPosition[] = {   // プレイヤーの初期位置
    { 350.0F, 0.0F }, // Player 1
    { 500.0F, 0.0F }, // Player 2
    { 650.0F, 0.0F }, // Player 3
    { 800.0F, 0.0F }, // Player 4
};
constexpr Vector2 kStarPosition[] = 
{
    { 480.0F, 650.0F },
    { 800.0F, 650.0F },
    { 700.0F, 450.0F },
    { 550.0F, 300.0F },
    { 650.0F, 80.0F },
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
    for( int i = 0; i < kPlayerMax; ++i )
    {
        player_.push_back( new AIMover() );

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
    for( auto& star_posi : kStarPosition )
    {
        star_container_->addStar(
            star_posi,
            90.0F, 5.0F, 0.2F, 120.0F )->setFall();
    }


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
    Space::getInstance()->collision();

    setPattern();
    checkAndCreateStar();

    auto itr = player_.begin();
    auto end = player_.end();
    for( ; itr != end; ++itr )
    {
        if( (*itr)->isAlive() ) { break; }
    }
    if( itr == end ) { return new Battle; }

    return this;
}

/*===========================================================================*/
// 描画処理
void Battle::draw()
{

}

/*===========================================================================*/
// スターの生成条件を満たしていたらスターを生成する
bool Battle::checkAndCreateStar()
{
    // 画面外待機しているスターが無くなったらスターの生成
    auto itr = star_container_->active().begin();
    auto end = star_container_->active().end();
    for (; itr != end; ++itr)
    {
        if ((*itr)->getPosition().y < 0.0F) { break; }
    }

    if (itr == end &&
        star_container_->createStar() == false)
    {
        return false;
    }


    return true;
}
// スター生成パターンセット
void Battle::setPattern()
{
    star_container_->resetPattern();

    std::wstring file_name;
    CsvLoader file(L"State/pattern_list.csv");
    for (int i = 0; ; ++i)
    {
        file_name = file.getString(rand() % 3, i);
        if (wcscmp(file_name.c_str(), L"") == 0)
        {
            break;
        }

        file_name.insert(0, L"State/");
        star_container_->addPattern(file_name);
    }
}
