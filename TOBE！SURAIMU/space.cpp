
// 板場 温樹

#include "space.h"

#include "collision.h"


/*===========================================================================*/
constexpr unsigned kMask        = 0x03; // マスク用

constexpr RECT kRange{ 0L, -200L, 1280L, 1000L };
constexpr unsigned kDivideLevel = 3U;   // 空間分割レベル
constexpr unsigned kDivideNum   = 4U;   // 分割数
const unsigned kBlockNum =              // 空間ブロック数
    (static_cast<unsigned>(std::pow( kDivideNum, kDivideLevel + 1U)) - 1U) /
                          (kDivideNum - 1U );
const int kBlockNumLevel2 = 
    (static_cast<int>( std::pow( kDivideNum, 3)) - 1) / (kDivideNum - 1U);


int toMorton( const RECT& Range, const Vector2& Point );
int toMorton( const RECT& Range, const Vector2& TopLeft, const Vector2& BottomRight );
int bitStep ( short Num );

/*===========================================================================*/
Space::Space() :
    range_(kRange), 
    space_(kBlockNum)
{

}

Space::~Space()
{

}

/*===========================================================================*/
// オブジェクトを空間に登録
void Space::registration( ObjectBase* const Obj,       // オブジェクトのアドレス
                          const Vector2&    Origin,    // 判定範囲原点
                          const float       Radius )   // 判定範囲半径
{
    unregistration( Obj );

    const int kBlock = toMorton(
        range_, 
        Vector2{ Origin.x - Radius, Origin.y - Radius },
        Vector2{ Origin.x + Radius, Origin.y + Radius }
    );
    if( kBlock < 0 ) { return; }

    object_list_[Obj] = &space_[kBlock];
    space_[kBlock].push_back( Obj );
}

/*===========================================================================*/
// オブジェクトを空間から登録解除
void Space::unregistration( ObjectBase* const Obj )
{
    auto object = object_list_.find( Obj );
    if( object != object_list_.end() )
    {
        object->second->remove(Obj);
        object_list_.erase(Obj);
    }
}

/*===========================================================================*/
// 登録されているオブジェクトの全衝突判定
void Space::collision()
{
    Collision* kCollision = Collision::getInstance();

    // 空間を走査
    for( int child = kBlockNum - 1, parent = 0; child >= 0; --child )
    {
        // ルート空間までの親と衝突判定
        for( parent = child; ; parent >>= 2 )
        {
            // 判定処理
            for( auto& child_obj : space_[child] ){
                for( auto& parent_obj : space_[parent] )
                {
                    kCollision->collision(
                        child_obj, parent_obj );
                }
            }

            // ルート空間所属のオブジェクトとの判定を終えたら次の空間へ
            if( parent == 0 ) { break; }
        }
    }
}


/*===========================================================================*/
// 引数の座標を引数の範囲内のモートン番号へ
int toMorton( const RECT& Range, const Vector2& Point )
{
    const int kLineNum = static_cast<int>(std::pow(2, kDivideLevel));

    const long kRangeWidth  = Range.right - Range.left;
    const long kRangeHeight = Range.bottom - Range.top;


    // 範囲内でのモートン番号を求める
    const int kMortonX = static_cast<int>(
            (Point.x - Range.left)) / (kRangeWidth  / kLineNum);
    const int kMortonY = static_cast<int>(
            (Point.y - Range.top )) / (kRangeHeight / kLineNum);


    return bitStep(kMortonX) | (bitStep(kMortonY) << 1);
}
// 矩形を範囲内のモートン番号へ
int toMorton( const RECT& Range, const Vector2& TopLeft, const Vector2& BottomRight )
{
    // 2頂点から、モートン番号を求める
    if (TopLeft.x < Range.left || TopLeft.y < Range.top ||
        BottomRight.x > Range.right || BottomRight.y > Range.bottom)
    {
        // 範囲外オブジェクトは番号を与えない
        return -1;
    }

    const int kXorMorton =
        toMorton(Range, TopLeft) ^ toMorton(Range, BottomRight);


    // 所属空間の空間レベルを求める
    int level = kDivideLevel;
    if (kXorMorton != 0)
    {
        level = 0;

        for (int i = kDivideLevel - 1; i >= 0; --i)
        {
            // 所属空間が判明
            if (kXorMorton | (kMask << (i * 2)))
            {
                break;
            }
            // 空間レベルを子へ
            else
            {
                ++level;
            }
        }
    }


    // 空間へ登録
    int block = (static_cast<unsigned>(std::pow(kDivideNum, level)) - 1U) /
        (kDivideNum - 1U);
    return block += toMorton(Range, TopLeft) >> ((kDivideLevel - level) * 2);
}


// 引数の数字を1ビット飛ばしにした数値を返却
int bitStep( short Num )
{
    Num = (Num | (Num << 8)) & 0x00ff00ff;
    Num = (Num | (Num << 4)) & 0x0f0f0f0f;
    Num = (Num | (Num << 2)) & 0x33333333;
    Num = (Num | (Num << 1)) & 0x55555555;

    return Num;
}