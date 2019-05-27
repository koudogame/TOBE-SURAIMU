#pragma once

class ScoreNumber
{
public:
	ScoreNumber();
	~ScoreNumber();

public:
	bool init( unsigned int Num );
	void update( const float DrawPositY );
	void draw();
	bool isAlive();


private:
	unsigned int number_;
	float alpha_;
	ID3D11ShaderResourceView* texture_;
	ID3D11ShaderResourceView* puluse_;
	Vector2 position_;
};

