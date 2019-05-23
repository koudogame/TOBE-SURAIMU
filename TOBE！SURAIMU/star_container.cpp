
// 板場 温樹

#include "star_container.h"

#include "release.h"
#include "csvLoader.h"


/*===========================================================================*/
StarContainer::StarContainer(TaskManager* const TaskManager) :
	ObjectContainerBase(TaskManager)
{}

StarContainer::~StarContainer()
{
}

/*===========================================================================*/
// スターの追加
Star* StarContainer::addStar(
	const Vector2& Position, const float Angle,
	const float Spin, const float Rate, const float Size)
{
	// フリーなスターを取得
	Star* star = getFreeObjAndInsert();

	// スターの初期化処理
	star->init(
		Position,
		Angle,
		Spin,
		Rate,
		Size
	);

	return star;
}


/*===========================================================================*/
// スターのステータスに倍率をかける
void StarContainer::resetStates(const float Magnification)
{
    for (auto& star : active_list_)
    {
        star->resetStatus(Magnification);
    }
}

/*===========================================================================*/
// y座標の移動( スクロール )
void StarContainer::setMove(const float Move)
{
    for (auto& star : active_list_)
    {
        star->setMove(Move);
    }
}

/*===========================================================================*/
// ランダムなファイルから星の生成情報を読み込んで生成する
bool StarContainer::createStar()
{
    return createStar(pattern_[rand() % pattern_.size()]);
}
// 指定したファイルから星の生成情報を読み込んで生成する
bool StarContainer::createStar(const std::wstring FileCSV)
{
    CsvLoader file(FileCSV);

    Vector2 position;
    float angle;
    float spin;
    float spin_rate;
    float size;

    // 情報があるだけ生成
    int count = 1;
    while (true)
    {
        position.x     = file.getNumber_f(0, count);
        if (position.x == -1) { break; }
        position.y     = file.getNumber_f(1, count);
        angle          = file.getNumber_f(2, count);
        spin           = file.getNumber_f(3, count);
        spin_rate      = file.getNumber_f(4, count);
        size           = file.getNumber_f(5, count);
        position.y     -= 720.0F;   // 画面外へ

        // 星をリストに追加して、落下を設定する
        addStar(
            position, angle, spin, spin_rate, size)->setFall();
        ++count;
    }
    return true;
}