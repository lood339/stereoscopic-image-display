#ifndef CVX_REFERENCE_FRAME_H
#define CVX_REFERENCE_FRAME_H 1

#include <vector>
#include "cvxImage.h"
#include "nvmath.h"
#include "stdGauge.h"

// display estimated normal direction and ground truth diretion at the same time
using std::vector;
using nv::vec3f;
class D3dStereoCamera;

class CvxReferenceFrame
{
public:
	CvxReferenceFrame();

	void loadImages(const cv::Mat &lImage, const cv::Mat &rImage);	
	~CvxReferenceFrame();

	void pushBackNormalPosition(const vec3f &n, const vec3f &p);
	void pushBackGroundTruthNormal(const vec3f &n);
	void clear();
	void clearHalf();
	
	void draw(IDirect3DDevice9* pDevice, D3dStereoCamera* pStereoCam, bool showGroundTruth);

private:
//	float m_resizeRatio;
	cv::Mat m_leftImage;
	cv::Mat m_rightImage;

	vector<vec3f> m_normals;            //estimated normal
	vector<vec3f> m_positions;
	vector<vec3f> m_groundTruthNormals; //groundTruthNormal
	StdGauge     *m_referenceGauge;     //show set gauge direction
	StdGauge     *m_groundTruthGauge;   //show the ground truth of normals
};


#endif