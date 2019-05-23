#pragma once

// 板場 温樹

#include "object_container_base.h"
#include "star.h"

//-----------------------------------------------------------------------------
// スターコンテナ
//-----------------------------------------------------------------------------
// --説明--
// スターの追加はaddStar関数を使用する( 生成失敗でreturn nullptr )
// setFall関数ですべてのスターに落下を追加する
// resetStates,setMove関数 : Starそのまま
// createStar関数 : ファイルから星の生成情報を読み込んで生成する
// addPattern関数 : 星の生成情報ファイルを追加する( 星のランダム生成時に使用する )
class StarContainer :
	public ObjectContainerBase<Star>
{
public:
	StarContainer(TaskManager* const TaskManager );
	~StarContainer();
public:
	Star* addStar(
		const Vector2& Position,
		const float Angle,
		const float SpinSpeed,
		const float SpinRate,
		const float Size
	);

    void resetStates(const float Magnification);
    void setMove(const float MoveY);

    bool createStar();
    bool createStar(const std::wstring PatternFileCSV);
    void addPattern(const std::wstring PatternFileCSV) { pattern_.push_back(PatternFileCSV); }

private:
    std::deque<std::wstring> pattern_;
};
