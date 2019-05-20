#include "stdafx.h"
#include "csvLoader.h"


CsvLoader::CsvLoader()
{
	comma_ = L',';
}


CsvLoader::CsvLoader(std::wstring FileName)
{
	comma_ = L',';
	load( FileName );
}

//読み込み
bool CsvLoader::load( std::wstring FileName)
{
	//ファイルの読みこみ
	std::wifstream file;
	file.open( FileName.c_str() );

	while( !file.eof() )
	{
		//1行分の文字列
		std::wstring line;
		//1行分取得
		std::getline( file , line );
		//カンマを末尾に挿入
		line.push_back( L',' );
		//null文字を末尾に挿入
		line.push_back( L'\0' );
		//各セル情報に分割して格納
		std::vector<std::wstring> cell;

		for(unsigned int i = 0 , k = 0; i < line.size(); i++ )
		{
			if( line.at( i ) == comma_ )
			{
				std::wstring s = line.substr( k , i - k );
				i++;
				k = i;
				cell.push_back( s );
			}
		}

		container_.push_back( cell );

	}

	file.close();

	return true;
}

//文字列の取得
std::wstring CsvLoader::getString( const unsigned int X , const unsigned int Y )
{
	//領域外チェック
	if( Y >= container_.size() )
		return L"";
	if( X >= container_.at( Y ).size() )
	return L"";

	return container_[ Y ][ X ];
}

//数値の取得( int型 )
int CsvLoader::getNumber( const unsigned int X , const unsigned int Y )
{
	std::wstring num = getString( X , Y );

	//サイズが0の場合エラーを返す( -1 )
	if( num.size() == 0 )
		return -1;

	//文字を数値に変換して返す
	return _wtoi( num.c_str() );
}

//数値の取得( float型 )
float CsvLoader::getNumber_f( const unsigned int X , const unsigned int Y )
{
	std::wstring num = getString( X , Y );

	//サイズが0の場合エラーを返す( -1 )
	if( num.size() == 0 )
		return -1;

	//文字を数値に変換して返す
	return static_cast< float >( _wtof( num.c_str() ) );
}
