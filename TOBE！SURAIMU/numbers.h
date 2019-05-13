#pragma once

//-----------------------------------------------------------------------------
// ”š‚ğˆµ‚¤ƒNƒ‰ƒX
//-----------------------------------------------------------------------------
template <typename T>
class Numbers
{
public:
	Numbers();
	~Numbers();

public:
	void draw(ID3D11ShaderResourceView* const Texture,
		const Vector2& MostTopRightPosition,
		const long NumberWidth,
		const long NumberHeight,
		const unsigned MinDigits = 1U);
	
public:
	T operator = (const T& Operand) { return number_ = Operand; }
	T operator + (const T& Operand) { return number_ + Operand; }
	T operator - (const T& Operand) { return number_ - Operand; }
	T operator * (const T& Operand) { return number_ * Operand; }
	T operator / (const T& Operand) { return number_ / Operand; }
	T operator += (const T& Operand) { return number_ += Operand; }
	T operator -= (const T& Operand) { return number_ -= Operand; }
	T operator *= (const T& Operand) { return number_ *= Operand; }
	T operator /= (const T& Operand) { return number_ /= Operand; }

	bool operator == (const T& Operand) { return number_ == Operand; }
	bool operator <  (const T& Operand) { return number_ < Operand;  }
	bool operator <= (const T& Operand) { return number_ <= Operand; }
	bool operator >  (const T& Operand) { return number_ > Operand;  }
	bool operator >= (const T& Operand) { return number_ >= Operand; }

private:
	T number_;
};
