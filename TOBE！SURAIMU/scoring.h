#pragma once
//========================================
//CreatorName:YamadaHaruhisa

#include "numbers.h"

class Scoring
{
public:
	Scoring();
	~Scoring();

	//外部公開関数( 必ず実行 )
public:
	bool init();
	void draw();
	void destroy();
	//上昇量加算
	//引数...上昇量
	void addDefaultScore( float AddScore );
	//スコアリング開始
	inline void start() { scoring_flag_ = true; }

	//外部公開関数
public:
	//移動コンボ増加
	void addCombo();
	//移動コンボリセット
	void resetCombo();
	//テクニック加点
	void addTechnique();
	//下降加点
	void addDown();
	//回転加点
	//引数...回転数( 度数法 )
	void addRotate( float Angle );
	//回転ステータスリセット
	void resetRotate();
	//スコアの取得
	inline unsigned long long getScore() { return score_; }

	//メンバ変数
private:
	unsigned long long score_;	                //総スコア
	unsigned int combo_;	                        //移動コンボ
	unsigned int rotation_combo_;	            //回転コンボ
	float rotation_;	                //回転量
	bool scoring_flag_;					//スコアリング有効フラグ
	ID3D11ShaderResourceView* texture_;	//テクスチャ

	//仮描画
	Numbers<unsigned long long> sco_;
	Numbers<unsigned int> move_com_;
	Numbers<unsigned int> rota_com_;

};

