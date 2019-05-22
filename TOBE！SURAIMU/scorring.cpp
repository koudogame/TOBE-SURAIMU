#include "stdafx.h"
#include "scorring.h"

const int kComboScore = 100;
const int kDownScore = 1;
const int kTechniqueScore = 1000;
const int kRotationScore = 100;

Scorring::Scorring()
{}

Scorring::~Scorring()
{}

bool Scorring::init()
{
	score_ = 0;
	combo_ = 0;
	rotation_ = 0.0F;
	rotation_combo_ = 0;
	return true;
}

void Scorring::draw()
{}

void Scorring::addDefaultScore( float AddScore )
{
	score_ += static_cast< int >( AddScore );
}

void Scorring::addCombo()
{
	combo_++;

	score_ += combo_ * kComboScore;
}

void Scorring::resetCombo()
{
	combo_ = 0;
}

void Scorring::addtechnique()
{
	score_ += kTechniqueScore;
}

void Scorring::addDown()
{
	score_ += kDownScore;
}

void Scorring::addRotate( float Angle )
{
	rotation_ += Angle;

	if( rotation_ >= 360.0F )
	{
		rotation_ = 0;
		rotation_combo_++;
		score_ += rotation_combo_ * kRotationScore;
	}
}

void Scorring::resetRotate()
{
	rotation_ = 0;
	rotation_combo_ = 0;
}

