#pragma once
struct TitleStatus
{
	Vector2 position;
	ID3D11ShaderResourceView* texture;
	RECT trim;
	float alpha = 1.0F;
};

class TitleObject
{
public:
	TitleObject();
	~TitleObject();

	//åpè≥ä÷êî
public:
	bool init( TitleStatus* TitleObjectStatus );
	void update();
	void draw();

private:
	TitleStatus* status_;
};

