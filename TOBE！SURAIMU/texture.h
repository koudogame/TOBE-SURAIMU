#pragma once

class Texture
{
private:
	//コンストラクタ
	Texture() = default;
public:
	//自身の実体の返却
	static Texture* getInstance() { static Texture instance; return &instance; }
	//テクスチャの読み込み
	//引数:読み込むファイルの名前( ワイド文字 )
	//戻り値:テクスチャハンドル
    ID3D11ShaderResourceView* load( const std::wstring& FileName );
};
