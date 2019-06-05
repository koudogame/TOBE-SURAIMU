#include "easing.h"

Easing::~Easing()
{}

float Easing::expo( const float Movement , const float Time , Mode Mode )
{
	if( Time <= 1.0F )
	{
		switch( Mode )
		{
			//“ü—Í
			case Easing::In:
				return Movement * std::pow( 2.0F , 10 * ( Time - 1 ) );
				//o—Í
			case Easing::Out:
				return Movement * ( -std::pow( 2.0F , -10 * ( Time ) ) + 1 );
				//“üo—Í
			case Easing::InOut:
				float t = Time;
				t /= 1.0F / 2.0F;
				if( t < 1.0F )
					return Movement / 2.0F * std::pow( 2.0F , 10 * ( t - 1 ) );
				t = t - 1;
				return Movement / 2.0F * ( -std::pow( 2.0F , ( -10.0F * t ) ) + 2.0F );
				break;
		}
	}
	return 0.0f;
}
