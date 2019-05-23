
// 板場 温樹

#include "numbers.h"

#include "sprite.h"


/*===========================================================================*/
template <typename T>
Numbers<T>::Numbers() :
	number_(static_cast<T>(0))
{}

template <typename T>
Numbers<T>::Numbers(const T& Value) :
	number_(Value)
{}

template <typename T>
Numbers<T>::~Numbers()
{}

/*===========================================================================*/
// 数字の描画
template <typename T>
void Numbers<T>::draw(
	ID3D11ShaderResourceView* const Texture,
	const Vector2& MostTopRightPosition,
	const long NumberWidth,
	const long NumberHeight,
	const unsigned MinDigits)
{
	const T kTen = static_cast<T>(10);
	const T kZero = static_cast<T>(0);

	Vector2 position(
		MostTopRightPosition.x - NumberWidth,
		MostTopRightPosition.y);

	RECT trimming;
	trimming.top = 0L;
	trimming.bottom = NumberHeight;

	T temp = number_;

	// 1の桁から描画
	// 指定された桁数より小さいか、数値が0より大きい間描画する
	for (unsigned i = 0U; (i < MinDigits) || (temp > kZero); ++i)
	{
		trimming.left = static_cast<long>(temp % kTen) * NumberWidth;
		trimming.right = trimming.left + NumberWidth;

		Sprite::getInstance()->draw(
			Texture,
			position,
			&trimming,
			1.0F,
            1.0F
		);

		temp /= kTen;
		position.x -= NumberWidth;
	}
}


// テンプレートのインスタンス化
template Numbers<int>;
template Numbers<unsigned>;
template Numbers<long>;
template Numbers<unsigned long>;
template Numbers<long long>;
template Numbers<unsigned long long>;
