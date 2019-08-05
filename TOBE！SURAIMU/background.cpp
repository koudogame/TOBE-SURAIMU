
#include "background.h"

#include "textureLoder.h"
#include "task_manager.h"
#include "view_background.h"
#include "wave_background.h"




// テクスチャサイズが大きい画像を初期化時、同時に読み込んでおく
/*===========================================================================*/
const wchar_t* const kTextureFileName[]
{
    L"Texture/back_wave_purple.png",    // kPurple
    L"Texture/back_wave_blue.png",      // kBule
    L"Texture/back_wave_red.png",       // kRed
};
namespace
{
    class Loader
    {
    public:
        Loader()
        {
            TextureLoder* texture_loader = TextureLoder::getInstance();

            for (auto& file : kTextureFileName)
            {
                texture_loader->load(file);
            }
        }
    };
}

constexpr Vector2 kViewPositionInit { 190.0F, -600.0F };
constexpr float kThresholdCreateViewY = -600.0F;
constexpr float kViewOffsetY = 600.0F;
constexpr Vector2 kWavePositionInit { 0.0F, -1200.0F };


/*===========================================================================*/
Background::Background()
{

}

Background::~Background()
{

}


/*===========================================================================*/
bool Background::init()
{
    static ::Loader load_texture;   /*サイズの大きい画像を読み込んでおく*/

    // タスク登録
    TaskManager* task_manager = TaskManager::getInstance();
    task_manager->registerTask( this, TaskUpdate::kBackground );
    task_manager->registerTask( this, TaskDraw::kBackground );


    // 背景を下まで追加
    View* view = nullptr;
    Vector2 view_position = kViewPositionInit;
    while( view_position.y < getWindowHeight<float>() )
    {
        view = new View();
        if( view->init( 
            view_position, 
            BackObjectBase::Color::kPurple) == false )
        {
            return false;
        }

        view_list_.push_back( view );

        view_position.y += kViewOffsetY;
    }
    last_view_ = *view_list_.begin();


    // その他のメンバ初期化
    color_ = BackObjectBase::Color::kPurple;


    return true;
}

void Background::destroy()
{
    // リストのオブジェクトの開放処理ラムダ
    auto releaseForList = []( auto& List )
    {
        for( auto& elem : List )
        {
            elem->destroy();
            delete elem;
        }

        List.clear();
    };

    // 背景の開放
    releaseForList( view_free_list_ );
    releaseForList( view_list_ );

    // 波の開放
    releaseForList( wave_free_list_ );
    releaseForList( wave_list_ );



    // タスク解除
    TaskManager::getInstance()->unregisterObject( this );
}

void Background::update()
{
    // リストのオブジェクト更新ラムダ
    auto updateForList = []( auto& List, auto& FreeList )
    {
        for( auto itr = List.begin(), end = List.end();
            itr != end; )
        {
            // オブジェクトの更新
            (*itr)->update();

            // オブジェクトが死んでいたら、フリーリストに移動
            if( (*itr)->isAlive() == false )
            {
                FreeList.push_back(*itr);
                itr = List.erase( itr );
            }
            else
            {
                ++itr;
            }
        }
    };


    updateForList( view_list_, view_free_list_ );
    updateForList( wave_list_, wave_free_list_ );


    // 背景が切れないよう追加していく
    // 最後に追加した背景を基準に追加
    const Vector2& last_position = last_view_->getPosition();
    if( last_position.y > kThresholdCreateViewY )
    {
        View *view = nullptr;

        if( view_free_list_.size() > 0U )
        {
            view = view_free_list_.back();
            view_free_list_.pop_back();
        }
        else
        {
            view = new View();
        }

        Vector2 view_position{
            last_position.x,
            last_position.y - kViewOffsetY
        };
        if( view->init( view_position, color_ ) )
        {
            view_list_.push_back( view );
            last_view_ = view;
        }


#if 1
        Wave *wave = nullptr;
        if( wave_free_list_.size() > 0U )
        {
            wave = wave_free_list_.back();
            wave_free_list_.pop_back();
        }
        else
        {
            wave = new Wave();
        }

        if( wave->init( kWavePositionInit, color_ ) )
        {
            wave_list_.push_back( wave );
        }
#endif
    }
}

void Background::draw()
{
    // リストのオブジェクト描画ラムダ
    auto DrawForList = []( auto& List )
    {
        for( auto& elem : List )
        {
            elem->draw();
        }
    };


    DrawForList( view_list_ );
    DrawForList( wave_list_ );
}


/*===========================================================================*/
void Background::setMove( const float Offset ) 
{
    auto moveForList = [=]( auto& List )
    {
        for( auto& elem : List )
        {
            elem->setMove( Offset );
        }
    };

    moveForList( view_list_ );
    moveForList( wave_list_ );
}

void Background::changeColor()
{
    color_ = color_ != BackObjectBase::Color::kRed ?
    static_cast<BackObjectBase::Color>(static_cast<int>(color_) + 1) :
    BackObjectBase::Color::kPurple;
}

void Background::reset()
{
    color_ = BackObjectBase::Color::kPurple;
    
    // リストのオブジェクトに色をセットするラムダ式
    auto setColorForList = [this]( auto& List )
    {
        for( auto& elem : List )
        {
            elem->setColor( color_ );
        }
    };

    setColorForList( view_list_ );
    setColorForList( wave_list_ );
}
