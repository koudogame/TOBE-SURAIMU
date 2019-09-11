#pragma once
class Band
{
public:
	Band();
	~Band();

public:
	bool init(int ColorID, int SizeID);
	void update();
	void draw(const Vector2& DrawPosition);
	void destroy();
	float getHeight();

private:
	std::unique_ptr<std::mt19937> rand_;
	ID3D11ShaderResourceView* texture_;
	int color_id_;
	int size_id_;

	struct BandInf
	{
		float alpha;
		float scale;
		BandInf(float Alpha, float Scale)
		{
			alpha = Alpha;
			scale = Scale;
		}
	};
	std::vector<BandInf> band_inf_;

};

