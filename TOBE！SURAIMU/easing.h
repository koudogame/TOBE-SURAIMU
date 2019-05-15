#pragma once
class Easing
{
	//singleton
private:
	Easing() = default;
public:
	static Easing* getInstance() { static Easing instance; return &instance; }

public:
	~Easing();

public:
	enum Mode
	{
		In,
		Out,
		InOut
	};

	float expo( const float Movement, const float Time , Mode Mode );


};

