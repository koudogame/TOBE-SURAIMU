#include "stdafx.h"
#include "scoring.h"

const int kComboScore = 100;
const int kDownScore = 1;
const int kTechniqueScore = 1000;
const int kRotationScore = 100;

Scoring::Scoring()
{}

Scoring::~Scoring()
{}

bool Scoring::init()
{
	score_ = 0;
	combo_ = 0;
	rotation_ = 0.0F;
	rotation_combo_ = 0;
	return true;
}

void Scoring::draw()
{}

void Scoring::addDefaultScore( float AddScore )
{
	score_ += static_cast< int >( AddScore );
}

void Scoring::addCombo()
{
	combo_++;

	score_ += combo_ * kComboScore;
}

void Scoring::resetCombo()
{
	combo_ = 0;
}

void Scoring::addtechnique()
{
	score_ += kTechniqueScore;
}

void Scoring::addDown()
{
	score_ += kDownScore;
}

void Scoring::addRotate( float Angle )
{
	rotation_ += Angle;

	if( rotation_ >= 360.0F )
	{
		rotation_ = 0;
		rotation_combo_++;
		score_ += rotation_combo_ * kRotationScore;
	}
}

void Scoring::resetRotate()
{
	rotation_ = 0;
	rotation_combo_ = 0;
}

