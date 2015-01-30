#ifndef IMAGE_MODEL_H
#define IMAGE_MODEL_H 1

//recover 3d position, normal from images (depth map, normal map)
#include "cvxImage.h"
#include "cvxD3d9.h"
#include "nvmath.h"
#include <string>
#include <iostream>
#include <vector>
#include "SampleItem.h"
#include "RandomTemplate.h"
using nv::vec3f;
using namespace std;

class ImageModel;

struct SamplePoint
{
	int x;
	int y;
	float gt_theta;  //ground truth of rotation angle (azimuth angle)
	float gt_phi;    //groudn truth of slant angle
	float r_theta;   //result of estimated rotation angle
	float r_phi;     //result of estimated slant angle

	SamplePoint()
	{
		x = y = 0;
		gt_theta = gt_phi = r_theta = r_phi = 0.0f;
	}
};

//input configuration of scene
class SceneConfig
{

public:
	string m_leftName;
	string m_rightName;
	string m_normalName;
	string m_depthName;
	string m_alphaName;
	float m_depthOffset;
	float m_depthResize;
	float m_focalLength;
	float m_sensorWidth;
	std::vector<SamplePoint> m_samplePoints;

public:
	SceneConfig()
	{
		m_depthOffset = -1.0f;
		m_depthResize = -1.0f;
	}

	// read from a file
	friend ostream & operator<<(ostream & os, const SceneConfig &sc)
	{
		assert(os.good());
		os<<"left_name "<<sc.m_leftName<<endl;
		os<<"right_name "<<sc.m_rightName<<endl;
		os<<"normal_map "<<sc.m_normalName<<endl;
		os<<"depth_map "<<sc.m_depthName<<endl;
		os<<"alpha_map "<<sc.m_alphaName<<endl;
		os<<"depth_info "<<sc.m_depthOffset<<" "<<sc.m_depthResize<<" "<<"#offset_and_resize"<<endl;
		os<<"camera_info "<<sc.m_focalLength<<" "<<sc.m_sensorWidth<<" "<<"#focal_length_and_sensorWidth"<<endl;		
		assert(os.good());
		if(sc.m_samplePoints.size() != 0)
		{
			os<<sc.m_samplePoints.size()<<endl;
			for(unsigned int i = 0; i<sc.m_samplePoints.size(); ++i)
			{
				os<<sc.m_samplePoints[i].x<<" "<<sc.m_samplePoints[i].y<<" " \
					<<sc.m_samplePoints[i].gt_theta<<" "<<sc.m_samplePoints[i].r_theta<<" " \
					<<sc.m_samplePoints[i].gt_phi<<" "<<sc.m_samplePoints[i].r_phi<<endl;
			}
		}
		assert(os.good());		
		return os;
	}

	//write to file
	friend istream & operator>>(istream & is, SceneConfig &sc)
	{
		string dump;
		is>>dump>>sc.m_leftName;
		is>>dump>>sc.m_rightName;
		is>>dump>>sc.m_normalName;
		is>>dump>>sc.m_depthName;
		is>>dump>>sc.m_alphaName;
		is>>dump>>sc.m_depthOffset>>sc.m_depthResize>>dump;
		is>>dump>>sc.m_focalLength>>sc.m_sensorWidth>>dump;
		assert(is.good());
		if(!is.eof())
		{
			int n = 0;
			is>>n;
			if(is.eof())
			{
				return is;
			}			
			assert(is.good());
			for(int i = 0; i<n; ++i)
			{
				int x, y;
				is>>x>>y;
				SamplePoint sp;
				sp.x = x;
				sp.y = y;
				sc.m_samplePoints.push_back(sp);
			}
		}
		return is;
	}

	bool operator<(const SceneConfig &otherConfig)const
	{
		return m_leftName<otherConfig.m_leftName;
	}
};

// Image model contain information from one image condition
// It also generates random order of the trials
class ImageModel
{
	Mat m_mid;        //middle image
	Mat m_depth;
	Mat m_normal;
	Mat m_alpha;
	
	SceneConfig m_scene;
	TwoDimensionSample *m_sampleEng;

	vec3f m_vNormal;    // current estimated normal direction
	vec3f m_vPosition;  // current position of the center of the gauge (fixed)

public:
	ImageModel();
	~ImageModel();
	//
	//load images from file
	void loadConfig(const string &fileName);
	void resample();

	//calculate normal and 3D position for a particualr position in image
	vec3f calcNormalFromImage(int x, int y, bool fullScreen);
	vec3f calcPositionFromImage(int x, int y, bool fullScreen);

	//sample an test point
	bool getNextSampleItem(SampleItem &item);	
	void saveItem(SampleItem &item);
	void getNumbers(int &restNum, int &totalNum);    //get the number of rest sampling numbers

	//
	void printCurrentResult(bool slant, bool rotation);
	void saveResult(const string &fileName);
	void saveResult(ostream & os);

private:
	bool getARandomSamplePoint(SampleItem &item);

};

#endif