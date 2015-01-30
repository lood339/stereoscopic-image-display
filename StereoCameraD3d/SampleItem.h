#ifndef SAMPLE_ITEM_H
#define SAMPLE_ITEM_H 1


// the position and normal directions of current gauge
#include "nvmath.h"
#include <string>
#include <iostream>
#include <vector>
using nv::vec3f;

class SampleItem
{
	int m_x;    // gauge center position in the image space
	int m_y;

	vec3f m_gdNormal;    //ground truth of normal direction
	vec3f m_gdPosition;  //ground truth of gauge center position

	float m_estimatedSlantAngle;    // estimated slant angle
	float m_estimatedRotationAngle;	// estimated rotation (azimuth) angle

	float m_gdSlantAngle;     //slant angle from ground truth normal direction
	float m_gdRotationAngle;  //rotation angle (azimuth) angle from ground truth normal direction

public:
	SampleItem();
	void setSampleCenter(int x, int y, bool fullScreen);
	void setGroundtruthNormal(const vec3f &n);
	void setGroundtruthPosition(const vec3f &p);
	
	//input from user, mouse position	
	void flipSlant();
	void adjustSlantByMouseMove(int mx, int my);
	void setRotationAngle(int mx, int my, bool fullScreen); //mouse position in x, y
	void setNormalByMouseMove(int deltaX, int deltaY);      //move mouse to set normal direction

	//output
	vec3f getGroundtruthNormal(){ return m_gdNormal;}
	vec3f getGroundtruthPosition() { return m_gdPosition;}
	float getEstimatedSlantAngle() {return m_estimatedSlantAngle;}
	float getEstimatedRotationAngle() {return m_estimatedRotationAngle;}
	float getGdSlantAngle(){return m_gdSlantAngle;}
	float getGdRotationAngle(){return m_gdRotationAngle;}
	float getAngularDifferenceIn3D();                     //get the angular difference between the current estimated angle and groud truth in 3D, assumen both angle is controlable

	int getXpos(){return m_x;};
	int getYpos(){return m_y;};

	void printAngles(bool slant, bool rotation);

	//sltant angle and rotat angle, reverse process of  ImageModel calcNormal
	vec3f normal();                      //get normal from current estimated slant and rotate 
	vec3f normalWithSlant();             //normal with true slant angle
	vec3f normalWithRotate();            //rotate angle set to groud truth

private:
	void addSlant();
	void decreaseSlant();
	
};

#endif
