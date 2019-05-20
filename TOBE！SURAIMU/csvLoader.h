#pragma once
class CsvLoader
{
public:
	CsvLoader();
	CsvLoader( std::wstring FileName );
	~CsvLoader() = default;

public:
	//CSVファイルの読み込み
	bool load( std::wstring );
	//文字列情報の取得
	//引数...ファイルのX座標：ファイルのY座標
	std::wstring getString( const unsigned int X , const unsigned int Y );
	//数値の取得( int型 )
	int getNumber( const unsigned int X , const unsigned int Y );
	//数値の取得( float型 )
	float getNumber_f( const unsigned int X , const unsigned int Y );


private:
	wchar_t comma_;
	std::vector < std::vector < std::wstring >> container_;
};

