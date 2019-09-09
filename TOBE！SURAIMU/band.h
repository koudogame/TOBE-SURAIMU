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

private:
	std::random_device rand_;
	ID3D11ShaderResourceView* texture_;
	int color_id_;
	int size_id_;

	struct BandInf
	{
		float alpha;
		float scale;
		int signal_alpha;
		int signal_scale;
		BandInf(float Alpha, float Scale,int SignalAlpha,int SignalScale)
		{
			alpha = Alpha;
			scale = Scale;
			signal_alpha = SignalAlpha;
			signal_scale = SignalScale;
		}
	};
	std::vector<BandInf> band_inf_;

};

