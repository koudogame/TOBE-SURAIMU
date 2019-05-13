
#include "numbers.h"

#include "sprite.h"


/*===========================================================================*/
template <typename T>
Numbers<T>::Numbers() :
	number_(static_cast<T>(0))
{}

template <typename T>
Numbers<T>::~Numbers()
{}

/*===========================================================================*/
// ï`âÊ
template <typename T>
void Numbers<T>::draw(
	ID3D11ShaderResourceView* const Texture,
	const Vector2& MostTopRightPosition,
	const long NumberWidth,
	const long NumberHeight,
	const unsigned MinDigits)
{
	const T kTen = static_cast<T>(10);

	Vector2 position(
		MostTopRightPosition.x - NumberWidth,
		MostTopRightPosition.y
	);
	RECT trimming;
	trimming.top = 0L;
	trimming.bottom = NumberHeight;
	T temp = number_;

	// ï`âÊèàóù
	for (unsigned i = 0; (i < MinDigits) || (temp > 0); ++i)
	{
		trimming.left = static_cast<long>(temp % kTen) * NumberWidth;
		trimming.right = trimming.left + NumberWidth;

		Sprite::getInstance()->draw(
			Texture,
			position,
			&trimming
		);

		temp /= kTen;
		position.x -= NumberWidth;
	}
}


template Numbers<int>;
template Numbers<unsigned>;
template Numbers<long>;
template Numbers<unsigned long>;
template Numbers<long long>;
template Numbers<unsigned long long>;
