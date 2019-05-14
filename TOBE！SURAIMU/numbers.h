#pragma once

//-----------------------------------------------------------------------------
// 数クラス
//-----------------------------------------------------------------------------
// --説明--
// draw関数で値の描画を行うことができる
// 対応している型 : int, long, long long
// *各符号有り、無し両対応
template <typename T>
class Numbers
{
public:
	Numbers();
	Numbers(const T&);
	~Numbers();

public:
	void draw(
		ID3D11ShaderResourceView* const Texture,
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
