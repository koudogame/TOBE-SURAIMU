#pragma once
//========================================
//CreatorName:YamadaHaruhisa


#include "score_number.h"
#include "timer.h"

class Scoring
{
public:
	Scoring();
	~Scoring();

	//外部公開関数( 必ず実行 )
public:
	bool init();
	void update();
	void draw();
	void destroy();
	//上昇量加算
	//引数...上昇量
	void addDefaultScore( const double AddScore );
	//スコアリング開始
	inline void start() { scoring_flag_ = true; timeRestart(); }
	inline void stop() { scoring_flag_ = false; timeStop(); }
	inline bool isStart() { return scoring_flag_; }
	//タイマー操作
	inline void timeRestart(){combo_timer_.restart();}
	inline void timeStop(){combo_timer_.stop();}

	//外部公開関数
public:
	//移動コンボ増加
	void addCombo();
	//テクニック加点
	void addTechnique();
	//テクニックコンボのリセット
	void resettechnique();
	//下降加点
	void addDown();
	//回転加点
	//引数...回転数( 度数法 )
	void addRotate( float Angle );
	//回転ステータスリセット
	void resetRotate();
	//距離の加点
	void addLength( const float Length );
	//レベルアップ
	void addLevel();
	inline unsigned int getLevel() { return level_; }
	//スコアの取得
	inline unsigned long long getScore() { return score_; }
	//最大コンボ数の取得
	inline unsigned int getMaxCombo() { return max_combo_; }
	//上った距離の取得
	inline double getHeight() { return height_; }


	//メンバ変数
private:
	unsigned long long score_;	                    //総スコア
	double height_;									//上った距離
	unsigned int combo_;	                        //移動コンボ
	unsigned int max_combo_;						//最大移動コンボ
	unsigned int rotation_combo_;	                //回転コンボ
	unsigned int technique_combo_;					//テクニックコンボ
	float rotation_;	                            //回転量
	bool scoring_flag_;					            //スコアリング有効フラグ
	ID3D11ShaderResourceView* texture_;	            //テクスチャ
	ID3D11ShaderResourceView* num_texture_;
	enum
	{
		kScore,
		kCombo,
		kHeight,
	};
	ID3D11ShaderResourceView* add_num_texture_[ 3 ];	//数字のテクスチャ
	float spin_combo_pitch_;						//回転コンボのピッチ( コンボがかさむごとに高くなる )
	unsigned int level_;							//ゲームレベル

	double all_height_;								//減衰までの上昇値

	std::list<std::shared_ptr<ScoreNumber>> addition_list_;			//加点リスト
	Timer<Seconds> combo_timer_;

private:
	//描画加点の生成
	//引数...描画数値：テクスチャハンドル
	void createNumber( unsigned int Num , ID3D11ShaderResourceView* Handle );
};

