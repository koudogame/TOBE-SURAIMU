#pragma once
//========================================
//CreatorName:YamadaHaruhisa

#include "object_base.h"
#include "shape.h"
#include "freeFallParticle_container.h"

const int kStarLineNum = 5;

class Star :
	public ObjectBase
{
public:
	Star(TaskManager* const Maneger );
	~Star();

public:
	//Œp³ŠÖ”
	//‰Šú‰»
	//ˆø”...‰ŠúÀ•W : ‰ŠúŠp“x : —‰º‚Ì‘¬‚³ : ‰ñ“]‚Ì‘¬‚³ : ‰ñ“]”{—¦ : ‘å‚«‚³
	bool init( const Vector2& Position , const float Angle , const float Fall , const float Spin , const float Rate , const float Size );
	void destroy() override;
	void update() override;
	void draw() override;
	bool isAlive() override;
	void setMove( const float Over )override;

public:
	//ŠO•”—˜—pŠÖ”
	//—‰º‚Ì‘¬‚³‚Ì“K—p
	inline void setFall() { fall_ = temp_fall_; }
	inline Line* getShape( const int GetNum ) { return &myshape_[ GetNum ]; }
	void collision( class Player* P );
	void addFreeFallParticle();

private:
	//“à•”—˜—pŠÖ”
	void setAngle();

private:
	//ƒƒ“ƒo•Ï”
	float angle_[ kStarLineNum ];		//Šp“x
	float fall_;		    //—‰º‚Ì‘¬‚³
	float temp_fall_;	    //ˆêŠi”[—p‚Ì—‰º‚Ì‘¬‚³
	float spin_;		    //‰ñ“]‘¬“x
	int turn_;		        //‰ñ“]•ûŒü
	float size_;	        //‘å‚«‚³
	float rate_;	        //‰ñ“]”{—¦
	Line myshape_[ kStarLineNum ];

	std::unique_ptr<FreeFallParticleContainer> s_particle_container_;
	int particle_time_;
	int create_point_;
};

