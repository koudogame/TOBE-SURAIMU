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
	Star();
	~Star();

public:
	//Œp³ŠÖ”
	//‰Šú‰»
	//ˆø”...‰ŠúÀ•W : ‰ŠúŠp“x : ‰ñ“]‚Ì‘¬‚³ : ‰ñ“]”{—¦ : ‘å‚«‚³
	bool init( const Vector2& Position , const float Angle , const float Spin , const float Rate , const float Size );
	void destroy() override;
	void update() override;
	void draw() override;
	bool isAlive() override;

public:
	//ŠO•”—˜—pŠÖ”
	//—‰º‚Ì‘¬‚³‚Ì“K—p
	void setFall();
	inline Line* getShape( const int GetNum ) { return &myshape_[ GetNum ]; }
	void collision( class Player* P );
	void addFreeFallParticle();
	inline void resetStatus( const float Magnification ) { magnification_ = Magnification; }
	inline float getSize() { return size_; }
	inline NameSpaceParticle::ParticleID  getColor() { return id_; }

private:
	//“à•”—˜—pŠÖ”
	void setAngle();

private:
	//ƒƒ“ƒo•Ï”
	float angle_[ kStarLineNum ];		//Šp“x
	float fall_;		    //—‰º‚Ì‘¬‚³
	float spin_;		    //‰ñ“]‘¬“x
	float size_;	        //‘å‚«‚³
	float rate_;	        //‰ñ“]”{—¦
	Line myshape_[ kStarLineNum ];

	std::unique_ptr<FreeFallParticleContainer> s_particle_container_;
	int particle_time_;
	int create_point_;
	float magnification_;
	NameSpaceParticle::ParticleID id_;

	ID3D11ShaderResourceView* overlay_texture_;
};

