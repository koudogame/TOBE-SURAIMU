#pragma once
//********************************************
//Class
//********************************************
class Container
{
private:
	ID3D11ShaderResourceView* texture_;		//テクスチャハンドル
	int ref_cnt_;							//参照数

public:
	//コンストラクタ
	//引数:テクスチャハンドル
	Container(ID3D11ShaderResourceView* Texture);
	//デストラクタ
	~Container();
	//参照数増加
	void addRef() { (++ref_cnt_); }
	//参照数減少
	//戻り値:0以下( True )
	bool defRef() { return (--ref_cnt_ <= 0); }
	//テクスチャハンドルの取得
	//戻り値:テクスチャハンドル
	ID3D11ShaderResourceView* getTexture() { return texture_; }

};
