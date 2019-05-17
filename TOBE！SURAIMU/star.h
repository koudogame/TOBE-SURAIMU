#pragma once
//========================================
//CreatorName:YamadaHaruhisa

#include "object_base.h"
#include "shape.h"
#include "stayParticle_container.h"

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

public:
	//ŠO•”—˜—pŠÖ”
	//—‰º‚Ì‘¬‚³‚Ì“K—p
	inline void setFall() { fall_ = temp_fall_; }
	inline Line* getShape( const int GetNum ) { return &myshape_[ GetNum ]; }
	void collision( class Player* P );
	void addStayParticle();

private:
	//“à•”—˜—pŠÖ”
	void setAngle();

private:
	//ƒƒ“ƒo•Ï”
	float angle_[ 5 ];		//Šp“x
	float fall_;		    //—‰º‚Ì‘¬‚³
	float temp_fall_;	    //ˆêŠi”[—p‚Ì—‰º‚Ì‘¬‚³
	float spin_;		    //‰ñ“]‘¬“x
	int turn_;		        //‰ñ“]•ûŒü
	float size_;	        //‘å‚«‚³
	float rate_;	        //‰ñ“]”{—¦
	Line myshape_[ 5 ];

	std::unique_ptr<StayParticleContainer> s_particle_container_;
	int particle_time_;
	int create_point_;
};

