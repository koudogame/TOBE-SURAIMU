#include "direct3d.h"
#include "error.h"
#include "texture.h"

ID3D11ShaderResourceView* Texture::load( const std::wstring& FileName )
{
    ID3D11ShaderResourceView* texture;

    if( FAILED( CreateWICTextureFromFileEx(
        Direct3D::getInstance()->getDevice(),          //  D3Dデバイスインターフェイス
        Direct3D::getInstance()->getContext(),         //  デバイスコンテキストインターフェイス
        FileName.c_str(),                              //  ファイル名( ワイド文字で指定 )
        0,                                             //  ミップマップレベル
        D3D11_USAGE_DEFAULT,                           //  使用方法
        D3D11_BIND_SHADER_RESOURCE,                    //  ハンドルの種類
        0,                                             //  CPUアクセス方法
        0,                                             //  フラグオプション
        WIC_LOADER_DEFAULT,                            //  SRGBフラグ
        NULL,                                          //  リソースデータ受取先ポインタ
        &texture ) ) )                                 //  シェーダーリソースビュー受取先ポインタ
    {
        //  エラー
        char file_name[ 256 ], str[ 256 ];
		unsigned int num;
		wcstombs_s(&num, file_name, FileName.c_str(), 255);
        sprintf_s( str, "テクスチャの読み込みに失敗(%s)", file_name );
        Error::showDialog(str);
        return NULL;
    }

    return texture;
}