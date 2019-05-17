#pragma once
class CsvLoader
{
public:
	CsvLoader();
	CsvLoader( std::string FileName );
	~CsvLoader() = default;

public:
	bool load( std::string );
	std::wstring getString( const unsigned int X , const unsigned int Y );
	int getNumber( const unsigned int X , const unsigned int Y );

private:
	wchar_t comma_;
	std::vector < std::vector < std::wstring >> container_;
};

